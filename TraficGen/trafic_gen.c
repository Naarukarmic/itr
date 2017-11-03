#include "isocket.h"

#define BUF_SIZE 200

void trafic_generator(int proto_id, int packet_size){
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

  /*if(i_connect(s, "localhost", 8080) < 0) {
    fprintf(stderr, "connection refused\n");
    exit(1);
  }
   
  n = 0;
  while((cnt=read(s, buf, size)) > 0) {
    printf("%s\n", buf);
    n += cnt;
    if(n >= 12) break;
  }
  close(s);*/
  // Attribution du port 8080 à la socket
  if(i_bind(s, 1234) < 0) {
    fprintf(stderr, "tr_gen bind failed\n");
    exit(1);
  }

  for(;;) {
    s2 = i_accept(s);
    fprintf(stderr, "connexion client: %d\n", s2);
      
    write(s2, response, sizeof(response));
      
    close(s2);
    fprintf(stderr, "déconnexion\n");
  }
}

int main(int argc, char **argv) {
    if (argc != 3){
      printf("    Usage: web_client SCENARIO nCLIENT\n"
        "       SCENARIO = 0 (UDP) or 1 (TCP)\n"
        "       packet_size = n (packet size to send)\n");
      return -1;
    }

    if(atoi(argv[2]) > 1500 || atoi(argv[2]) < 64){
      printf("    Error: 64 <= packet size <= 1500\n");
      return -1;
    }

    trafic_generator(atoi(argv[1]), atoi(argv[2]));
    return 0;
}


