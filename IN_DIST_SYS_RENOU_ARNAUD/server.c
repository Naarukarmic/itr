#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "isocket.h"

#define BUF_SIZE 10
#define TCP_PORT 8080
#define CLIENT_BASE_PORT 8081


void* thread_job(void *no_argument) {
  pthread_t my_task_id = pthread_self();
  int i = 0, iter = labs (random())/10;

  printf("Thread %p : Start processing, %d iterations\n", my_task_id, iter);
  while(i < iter) i++;

  printf("Thread %p : End of processing\n", my_task_id);

  pthread_exit(NULL);
}

int main(int argc, char ** argv) {
  if(argc < 2){
    printf("Usage: server [NB_CLIENTS]");
    printf("\n");

    printf("nb_clients: number of expected clients, each client must connect to a port starting from 8081\n\n");

    return 0;
  }

  char buf[BUF_SIZE];
  int nb_clients = atoi(argv[1]);
  int s_bind[nb_clients];
  int s[nb_clients];

  printf("Size of buffer : %lu\n", sizeof(buf));

  strcpy(buf, "GET value");

  // Handshake with each client
  int i = 0;
  for(i = 0; i < nb_clients; i++)
  {
    s_bind[i] = i_socket();

    if(i_bind(s_bind[i], CLIENT_BASE_PORT + i) < 0) {
      fprintf(stderr, "bind failed\n");

      return 1;
    }
  }

  // Accept connections
  for(i = 0; i < nb_clients; i++)
  {
    printf("Waiting for connection from client %d...\n", i + 1);
    s[i] = i_accept(s_bind[i]);
    printf("Client %d connected\n", i + 1);
  }

  // Main loop
  while(1){
    // Send a request to each client
    for(i = 0; i < nb_clients; i++)
    {
      write(s[i], buf, sizeof(buf));
    }

    int response[nb_clients];

    // Reading the answers of each client
    for(i = 0; i < nb_clients; i++)
    {
      read(s[i], &(response[i]), sizeof(int));
      printf("Message received from client %d: %d\n", i + 1, response[i]);
    }

    // Compute minimum value
    int min = response[0];
    for(i = 1; i < nb_clients; i++)
    {
      if(response[i] < min)
        min = response[i];
    }
    printf("Minimum: %d\n\n", min);

    sleep(1);
  }

  return(0);
}
