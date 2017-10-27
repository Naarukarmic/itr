/******************************************************************************/
/* Model of a single-track rail system , protected by a semaphore ..
   and a few other concurrency constructs */

#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"debug.h"

/******************************************************************************/
/* Global variables and type definition */
int T[2];            /* number of trains waiting on each portion of the track */
sem_t F[2], G;
pthread_t trains [10];

/* ****************************************************************************/
void *train_job (void *useless_parameter) {
  int n;
  int dir = random() % 2;
  int next;
  printf ("Train %p created , direction = %d\n" , pthread_self () , dir );

  n = T[dir];
  T[dir]++;


  if (n == 0)       /* If the train is the first one to arrive, wait on the critical section */
    sem_wait (&G);
  else
    sem_wait (&F[dir]);

  printf ("The train %p is moving! %d\n" , pthread_self (), dir);
  sleep (1);

  T[dir]--;

  printf ("Train %p leaving the track ! %d\n" , pthread_self (), dir);
  // printf("dir %d next %d Tdir %d Tnext %d\n", dir, next, T[dir], T[next]);
  if (T[dir] == 0) {	/* Switch side if one is empty */
    sem_post(&G);
    sem_post(&F[1 - dir]);
  } else {
    sem_post(&F[dir]);
  }

  return NULL;
}

/******************************************************************************/
/* Main function */
int main (int argc, char **argv) {
  int err;
  for (int dir = 0; dir <= 1; dir ++) {
    err = sem_init (&F[dir], 0, 0); /* FIFO for the arriving trains */
    T[dir] = 0;
  }


  sem_init (&G, 0, 1); /* Only 1 train on the critical track */

  /* Thread creation */
  for (int i = 0; i < 10; i++) {
    pthread_create(&trains[i], NULL, &train_job, NULL);
  }

  /* Thread destruction */
  for (int i = 0; i < 10; i++) {
    pthread_join(trains[i], NULL);
  }

  return EXIT_SUCCESS;

}
