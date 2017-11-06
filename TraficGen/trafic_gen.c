#include <sys/time.h>
#include "isocket.h"

#define BUF_SIZE 200

char* gen_packet(int size) {
  int i = 0;
  char* msg = malloc(size);

  while(i < size) {
    msg[i] = 'x';
    i++;
  }

  return msg;
}

void udp_traffic(char* host, int port, int packet_size){
  int s;
  char* packet;

  // char req[32] = "    GET / HTTP/1.1 \r\n";
  // char response[] = "HTTP/1.1 200 OK\r\n"
  //   "Content-Type: text/html; charset=UTF-8\r\n\r\n"
  //   "<!DOCTYPE html><html><head><title>Mon serveur web</title>"
  //   "<body bgcolor=lightgreen><h1>Hello from my own web server !</h1></body></html>\r\n";

  s = i_socket_proto("udp");

  if(i_bind(s, 1234) < 0) {
    fprintf(stderr, "tr_gen bind failed\n");
    exit(1);
  }

  packet = gen_packet(packet_size);

  /*
  n = 0;
  while((cnt=read(s, buf, size)) > 0) {
    printf("%s\n", buf);
    n += cnt;
    if(n >= 12) break;
  }
  close(s);
  */
}

void tcp_traffic(char* host, int port, int packet_size){
  int s;
  char* packet;
  char* ack[BUF_SIZE];
  struct timeval begin, end;
  float time_spent;

  s = i_socket_proto("tcp");

  if(i_connect(s, host, port) < 0) {
    fprintf(stderr, "connection refused\n");
    exit(1);
  }

  packet = gen_packet(packet_size);

  gettimeofday(&begin, 0);
  int tx = send(s, packet, packet_size, 0);
  if (tx < 0) {
    perror("send");
  }
  
  int rx = recv(s, ack, BUF_SIZE, 0);
  if (rx < 0) {
    perror("recv");
  }
  gettimeofday(&end, 0);  

  time_spent = (end.tv_sec - begin.tv_sec) * 1000.0f + (end.tv_usec - begin.tv_usec) / 1000.0f;

  close(s);
  printf("TCP: Sent %d bytes in %.2f ms\n", packet_size, time_spent);
}

int main(int argc, char **argv) {
  if (argc != 3){
    printf("    Usage: web_client PROTOCOL PACKET_SIZE\n"
      "       PROTOCOL = 0 (UDP) or 1 (TCP)\n"
      "       PACKET_SIZE = n (packet size to send)\n");
    return -1;
  }

  if(atoi(argv[1]) > 1 || atoi(argv[1]) < 0){
    printf("    Error: PROTOCOL = 0 (UDP) or 1 (TCP)\n");
    return -1;
  }
  if (atoi(argv[2]) > 1500 || atoi(argv[2]) < 64) {
    printf("    Error: 64 <= PACKET_SIZE <= 1500\n");
    return -1;
  }

  switch(atoi(argv[1])) {
    case 0:
      udp_traffic("localhost", 8080, atoi(argv[2]));
      break;
    case 1:
      tcp_traffic("localhost", 8080, atoi(argv[2]));
      break;
  }

  return 0;
}
