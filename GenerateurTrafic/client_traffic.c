#define _POSIX_C_SOURCE 199309L
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>

#include "dataPacket.h"

#define TRANSFERT_RATE_TCP 100.0 //KBytes/s
#define TRANSFERT_RATE_UDP 100.0 //KBytes/s

void tcp_traffic(int port , int packet_size);
void udp_traffic(int port , int packet_size);

char *generate_random_string(int len);

int main(int argc, char **argv){

	// Check arguments to find out the protocol of communication
	if (argc != 4){
		printf ("Usage : %s | arg1 = [PORT number]\n",argv[0] );
		printf ("Usage : %s | arg2 = [TCP/tcp or UDP/udp]\n",argv[0] );
		printf ("Usage : %s | arg3 = [Packet size to send]\n",argv[0] );
		exit(1);
	}

	// Port number to send informations
	const int port = atoi(argv[1]); 
	// TCP or UDP choice -> By default it will be CP
	const char *tcp_udp = argv[2];
	// packet size to transmit
	const int packet_size =  atoi(argv[3]);

	// Initialize random generator
	srandom(time(NULL));

	// If udp traffic ask then create udp server else create tcp server by default
	if (strcmp(tcp_udp , "udp") == 0 || strcmp(tcp_udp , "UDP") == 0){
		udp_traffic(port,packet_size);
	}else{
		tcp_traffic(port,packet_size);
	}

	return EXIT_SUCCESS;
}

void tcp_traffic(int port, int packet_size){
	struct sockaddr_in server;
	int sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1){
		printf("Could not create socket\n");
		exit(1);
	}

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	bzero(&(server.sin_zero), 8);

	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
		printf("Connection failed. Error\n");
		exit(1);
	}
	// To measure time
	struct timespec start;

	// Init packet
	packetData packet;
	packet.id = 0;
	packet.data_size = packet_size;

	uint32_t final_packet_size = get_packet_size(&packet);
	int time_sleep = (int) ((((float)final_packet_size)/TRANSFERT_RATE_TCP)*1000);
	printf("DELAY BETWEEN CONNECTION (us) = %d\n",time_sleep);

	while(1){
		packet.data = generate_random_string(packet_size);
		packet.id +=1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		packet.time_sec = (start.tv_sec + start.tv_nsec/1000000000.0);
		char *buffer = packet_to_buffer(&packet);
		if (send(sock, buffer, final_packet_size, 0) <= 0){
			exit(1);
		}
		//print_packet(&packet);
		free(buffer);
		free(packet.data);
		usleep(time_sleep);
	}
	close(sock);
}

void udp_traffic(int port , int packet_size){
	struct sockaddr_in server;
	socklen_t addr_size;
	int sock = socket(AF_INET , SOCK_DGRAM , 0);
	if (sock == -1){
		printf("Could not create socket\n");
		exit(1);
	}

	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	addr_size = sizeof(server);

	// To measure time
	struct timespec start;

	// Init packet
	packetData packet;
	packet.id = 0;
	packet.data_size = packet_size;

	uint32_t final_packet_size = get_packet_size(&packet);

	//Sleep parameter for UDP traffic
	double time_sleep = ((((float)final_packet_size)/TRANSFERT_RATE_UDP)*1000.0);
	printf("DELAY FOR SELECT (us) = %d\n",(int) time_sleep);

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = (int) time_sleep;

	while(1){
		select(0, NULL, NULL, NULL, &tv);
		packet.data = generate_random_string(packet_size);
		packet.id +=1;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		packet.time_sec = (start.tv_sec + start.tv_nsec/1000000000.0);
		char *buffer = packet_to_buffer(&packet);
		if(sendto(sock,buffer,final_packet_size,0,(struct sockaddr *)&server,addr_size) <= 0){
			exit(1);
		}
		//print_packet(&packet);
		free(packet.data);
		free(buffer);
	}
	close(sock);
}

char *generate_random_string(int len){
	int i;
	char *res = malloc((len+1) * sizeof(char));
	for (i=0;  i<len ;){
		unsigned char c = (unsigned int)(rand()%(122-33) + 33 );
		res[i] = c;
		++i;
	}
	res[len] = '\0';
	return res;
}