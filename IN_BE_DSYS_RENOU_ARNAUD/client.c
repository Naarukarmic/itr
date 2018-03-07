#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "isocket.h"

#define BUF_SIZE 10


int main(int argc, char ** argv) {
  if(argc < 3){
    printf("Usage: client [SERVER_IP] [PORT]");
    printf("\n");

    printf("Server ip: IP adress of the server 127.0.0.1 if local\n\n");
    printf("Port: port on which to connect for the server\n\n");

    return 0;
  }

  char* server_ip = argv[1];

  // Initialize random
  srand(time(NULL));

  int tcp_port = atoi(argv[2]);

  // Create socket
  int s = i_socket();

  // Handshake
  printf("Wait for server...\n");
  while(i_connect(s, server_ip, tcp_port) < 0) {
    sleep(1);
  }

  // Send random value to the server on request
  printf("Wait for request...\n");
  while(1){
    char request[BUF_SIZE];

    while(read(s, &request, BUF_SIZE) > 0) {
      printf("Received request: ");
      printf("%s", request);
      printf("\n");

      // Send random value
      int random_value = rand();
      printf("Sending %d\n", random_value);
      write(s, &random_value, sizeof(int));
    }
  }

  // Close connection
  close(s);

  return 0;
}
