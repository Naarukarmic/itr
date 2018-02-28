/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define N_PROCESSUS 4

int main(){
  int udpSocket;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  int i;
  int processusValues[N_PROCESSUS];

  const char s[2]=":";
  char *token;
  int id;
  int minimum = INT_MAX;
  int old_minimum = INT_MAX;

  /*Initiate processus values*/
  for (i=0 ; i< N_PROCESSUS ; i++)
  {
    processusValues[i]=INT_MAX;
  }

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;

  while(1)
  {
    minimum = INT_MAX;
    /* Try to receive any incoming UDP datagram. Address and port of
      requesting client will be stored on serverStorage variable */
    recvfrom(udpSocket,
             buffer,
             1024,
             0,
             (struct sockaddr *)&serverStorage,
             &addr_size);

    /*Parse the incoming string*/
    token = strtok(buffer,s);
    id = atoi(token);
    while(token != NULL)
    {
        /*printf("%s\n", token);*/
        token = strtok(NULL,s);
        if(token != NULL)
        {
            processusValues[id]=atoi(token);
        }
    
    }

    /*Compute minimum*/
    for(i = 0; i< N_PROCESSUS ; i++)
    {
        if(processusValues[i]<minimum)
        {
            minimum = processusValues[i];
        }
    }

    if(old_minimum!=minimum)
    {
        printf("Current minimum : %d\n",minimum);
    }
   
    old_minimum = minimum;
  }

  return 0;
}