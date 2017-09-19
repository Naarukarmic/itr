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
  printf ("Train %p created , direction = %d\n" , pthread_self () , dir );

  n = T[dir];
  T[dir]++;


  if (n == 0)       /* comment #1 */
    sem_wait (&G);
  else
    sem_wait (&F[dir]);

  printf ("The train %p is moving! %d\n" , pthread_self (), dir);
  sleep (3);

  T[dir]--;

  printf ("Train %p leaving the track ! %d\n" , pthread_self (), dir);

  if (T[dir] == 0) {	/* comment #2 */
    /* see questions */
  }

  return NULL;
}

/******************************************************************************/
/* Main function */
int main (int argc, char **argv) {
  int err;
  for (int dir = 0; dir <= 1; dir ++) {
    err = sem_init (&F[dir], .., ..); /* see questions */
    T[dir] = 0;
  }


  sem_init (&G, .., ..); /* see questions */

  /* Thread creation */
  for (int i = 0; i < 10; i++) {
    TODO("Create threads");
    /* see questions */
  }

  /* Thread creation */
  for (int i = 0; i < 10; i++) {
    /* Wait for the termination of all threads */
    /* see questions */
  }

  return EXIT_SUCCESS;

}
