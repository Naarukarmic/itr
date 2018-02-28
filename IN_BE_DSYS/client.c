/************* UDP CLIENT CODE *******************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  if(argc != 2) {
    printf("\n Use: ./client <id> \n");
    return 1;
  }

  int clientSocket, portNum, nBytes, v;
  char buffer[1024];
  char value[32];
  char id[32];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*Create UDP socket*/
  portNum = 7891;
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);    

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverAddr;
  strcpy(id, argv[1]);

  while(1) {
    v = rand() % 100;
    sprintf(value, "%d", v);

    strcpy(buffer, id);
    strcat(buffer, ":");
    strcat(buffer, value);

    nBytes = strlen(buffer) + 1;
    
    /*Send message to server*/
    sendto(clientSocket,
           buffer,
           nBytes,
           0,
           (struct sockaddr *) &serverAddr,
           addr_size);

    /*Receive message from server
    nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);

    printf("Received from server: %s\n",buffer);*/
    sleep(1);
  }

  return 0;
}