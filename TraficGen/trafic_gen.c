#include "isocket.h"

#define BUF_SIZE 200

void trafic_generator(char* host, int port, int proto_id, int packet_size){
  int s, s2;
  char* proto;

  char response[] = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html><html><head><title>Mon serveur web</title>"
    "<body bgcolor=lightgreen><h1>Hello from my own web server !</h1></body></html>\r\n";

  switch (proto_id) {
    case 0: 
      proto = "udp";
      break;
    case 1: 
      proto = "tcp";
      break;
  }

  s = i_socket_proto(proto);

  if (proto_id == 0) {
    if(i_bind(s, 1234) < 0) {
      fprintf(stderr, "tr_gen bind failed\n");
      exit(1);
    }
  } else {
    if(i_connect(s, host, port) < 0) {
      fprintf(stderr, "connection refused\n");
      exit(1);
    }
  }
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

  trafic_generator("localhost", 8080, atoi(argv[1]), atoi(argv[2]));
  return 0;
}
