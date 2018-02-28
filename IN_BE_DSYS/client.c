/************* UDP CLIENT CODE *******************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define N_PROCESSUS 4

int main(int argc, char** argv){

  if(argc != 2 || atoi(argv[1])>N_PROCESSUS -1  || atoi(argv[1])<0) {
    printf("\n Use: ./client <id> \n id belongs to [0,N_PROCESSUS] \n N_PROCESSUS is a #define in server.c and client.c \n");
    return(1);
  }

  int clientSocket, portNum, nBytes, v, t;
  char buffer[1024];
  char value[32];
  char* id;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  portNum = 7891;

  /*Create UDP socket*/
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(portNum);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverAddr;
  id = argv[1];

  while(1){
    v = rand() % 100;
    sprintf(value, "%d", v);

    strcpy(buffer, id);
    strcat(buffer,":");
    strcat(buffer, value);
    printf("%s",buffer);
    nBytes = strlen(buffer) + 1;
    
    /*Send message to server*/
    sendto(clientSocket,
           buffer,
           nBytes,
           0,
           (struct sockaddr *) &serverAddr,addr_size);

    /*Receive message from server
    nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);
    printf("Received from server: %s\n",buffer);*/
    t = rand() % 10;
    sleep(t);
  }

  return 0;
}