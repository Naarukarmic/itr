/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define N_PROCESSUS 4

int main(){
  int udpSocket, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
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
    processusValues[i]=0;
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
    /* Try to receive any incoming UDP datagram. Address and port of
      requesting client will be stored on serverStorage variable */
    nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

    /*Convert message received to uppercase*/
    for(i=0;i<nBytes-1;i++)
      buffer[i] = toupper(buffer[i]);

    /*Parse the incoming string*/
    token = strtok(buffer,s);
    id = atoi(token);
    while(token != NULL)
    {
        //printf("%s\n", token);
        token = strtok(NULL,s);
        if(token != NULL)
        {
            processusValues[id]=atoi(token);
        }
    }

    /*Compute minimum*/
    if(processusValues[id]< minimum)
    {
        minimum = processusValues[id];
    }
    
    if(old_minimum!=minimum)
    {
        printf("Current minium : %d\n",minimum);
    }
    /*Send uppercase message back to client, using serverStorage as the address*/
    sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);

    old_minimum = minimum;
  }

  return 0;
}