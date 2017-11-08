#define _POSIX_C_SOURCE 199309L
#define _BSD_SOURCE

/* Standard C Headers */
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

#define MAX_BUFFER_SIZE 1024
#define MAX_SAMPLE_DATA 5000

void tcp_traffic();
void udp_traffic();
void save_data();

packetData **savedData;
double final_time[MAX_SAMPLE_DATA];

int main(int argc, char **argv){

	// Check arguments to find out the protocol of communication
	if (argc != 2){
		printf ("Usage : %s | arg1 = [TCP/tcp or UDP/udp]\n",argv[0] );
		exit(1);
	}

	// TCP or UDP choice -> By default it will be CP
	const char *tcp_udp = argv[1];

	//Init savedData
	savedData = malloc(MAX_SAMPLE_DATA * sizeof(packetData* ));

	// If udp traffic ask then create udp server else create tcp server by default
	if (strcmp(tcp_udp , "udp") == 0 || strcmp(tcp_udp , "UDP") == 0){
		udp_traffic();
	}else{
		tcp_traffic();
	}

	// Print extracted informations
	save_data();

	//Free all memories allocated
	int incr;
	for (incr=0 ; incr < MAX_SAMPLE_DATA ; incr++){
		free(savedData[incr]);
	}
	free(savedData);

	return EXIT_SUCCESS;
}

void tcp_traffic(){
	int number_sample = 0;
	// Socket avriables
	int one = 1;
	struct sockaddr_in svr_addr, from;
	socklen_t sin_len = sizeof(from);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		perror("Error opening socket\n");
		exit(1);
	}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

	// Initialize socket structure
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(0); // Check the first available port

	// Now bind the host address 
	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1){
		perror("Error on binding\n");
		close(sock);
		exit(1);
	}

	// Now start listening for the clients, 
	// here process will go in sleep mode and will wait for the incoming connection
	listen(sock ,  5);

	//Print to port used by the server for the client
	getsockname(sock, (struct sockaddr *) &from, &sin_len);
	printf("***********************************************\n");
	printf("TCP CLIENT PORT MUST BE ::  %d\n", ntohs(from.sin_port) );
	printf("***********************************************\n\n");

	// Get client file descriptor by accepting connection
	int client_fd = accept(sock, (struct sockaddr *) &from, &sin_len);
	if (client_fd < 0){
		perror("Accept failed !\n");
		exit(1);
	}

	// To measure time
	struct timespec start;

	while(number_sample < MAX_SAMPLE_DATA){
		char *buffer = malloc(MAX_BUFFER_SIZE *  sizeof(char));
		if( recv(client_fd, buffer, MAX_BUFFER_SIZE, 0) <= 0){
			perror("Reception failed ! \n");
			free(buffer);
			buffer = NULL;
			continue;
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		savedData[number_sample] = buffer_to_packet(buffer);
		final_time[number_sample] = (start.tv_sec + start.tv_nsec/1000000000.0);
		//print_packet(savedData[number_sample]);
		free(buffer);
		buffer =  NULL;
		number_sample++;
	}
	close(client_fd);
	close(sock);
}

void udp_traffic(){
	int number_sample = 0;

	// Socket avriables
	struct sockaddr_in svr_addr, from;
	struct sockaddr_storage serverStorage;
	socklen_t sin_len = sizeof(from);
	socklen_t stor_len = sizeof(serverStorage);

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0){
		perror("Error opening socket\n");
		exit(1);
	}

	// Initialize socket structure
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = INADDR_ANY;
	svr_addr.sin_port = htons(0); // Check the first available port

	// Now bind the host address 
	if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1){
		perror("Error on binding\n");
		close(sock);
		exit(1);
	}

	//Print to port used by the server for the client
	getsockname(sock, (struct sockaddr *) &from, &sin_len);
	printf("***********************************************\n");
	printf("UDP CLIENT PORT MUST BE ::  %d\n", ntohs(from.sin_port) );
	printf("***********************************************\n\n");

	// To measure time
	struct timespec start;

	while(number_sample < MAX_SAMPLE_DATA){
		char *buffer = malloc(MAX_BUFFER_SIZE *  sizeof(char));
		if( recvfrom(sock, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &serverStorage, &stor_len)<0){
			perror("Reception failed ! \n");
			free(buffer);
			buffer = NULL;
			continue;
		}
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		savedData[number_sample] = buffer_to_packet(buffer);
		final_time[number_sample] = (start.tv_sec + start.tv_nsec/1000000000.0);
		//print_packet(savedData[number_sample]);
		free(buffer);
		buffer = NULL;
		number_sample++;
	}
	close(sock);
}

void save_data(){
	double mean_delay = 0;
	int incr;
	for(incr =0; incr<MAX_SAMPLE_DATA;incr++){
		mean_delay+= final_time[incr]- savedData[incr]->time_sec;
	}

	mean_delay /= MAX_SAMPLE_DATA;
	double mean_transfert_rate= (MAX_SAMPLE_DATA * get_packet_size(savedData[MAX_SAMPLE_DATA-1]));
	mean_transfert_rate /= (final_time[MAX_SAMPLE_DATA-1]-final_time[0]);
	
	int total_packet_lost = 0;
	for(incr=1; incr<MAX_SAMPLE_DATA; incr++){
		if(savedData[incr]->id - savedData[incr-1]->id > 1){
			total_packet_lost+= savedData[incr]->id - savedData[incr-1]->id;
		}
	}
	printf("************************************************************\n");
	printf("***********************  RESULTATS  ************************\n");
	printf("************************************************************\n");
	printf("Duree de simulation = %f s\n\n",final_time[MAX_SAMPLE_DATA-1]-final_time[0]);
	printf("Temps de reponse moyen  = %f ms\n\n" , (mean_delay * 1000));
	printf("Taux de transfert moyen = %f Bytes/s\n\n", mean_transfert_rate);
	printf("Nombre total packet perdu = %d\n\n", total_packet_lost);
	printf("Soit %f pourcent perdu\n\n", (((float)total_packet_lost)/MAX_SAMPLE_DATA)*100);
	printf("Nombre moyen perdu  = %f Bytes/s\n\n",total_packet_lost*get_packet_size(savedData[MAX_SAMPLE_DATA-1])/(final_time[MAX_SAMPLE_DATA-1]-final_time[0]));
	printf("************************************************************\n");
	printf("************************************************************\n");
}