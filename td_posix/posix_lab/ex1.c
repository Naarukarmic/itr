/*****************************************************************************/
/* POSIX Lab Session -- Exercise #1                                          */
/*****************************************************************************/

/* In this exercise, one has to
 * - create N threads
 * - wait for the completion of these N threads
 *
 * this exercise will use phtread_create(), pthread_join(), pthread_exit()
 *
 * Refer to man pages for more details, e.g. issue in a terminal
 *   "man pthread_create"
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

#define MAXTHREADS 256 /* Maximum number of threads in the program           */

void *thread_job(void *no_argument);

/*****************************************************************************/
/* Simple job executed by a thread:                                          */
/* - prints some information on the running thread                           */
/* - do some useless iterations                                              */

void *thread_job(void *no_argument) {
  pthread_t my_task_id = pthread_self();
  int i = 0, iter = labs (random())/10;

  printf("Thread %p : Start processing, %d iterations\n", my_task_id, iter);
  while(i < iter) i++;

  printf("Thread %p : End of processing\n", my_task_id);

  TODO("Add proper termination");
}

/*****************************************************************************/
/* Main entrypoint for this program                                          */

int main(int argc, char *argv[]) {
   int i;
   int nThreads;
   pthread_t thr_main = pthread_self();
   pthread_t Threads[MAXTHREADS];

   /* Check command-line parameters                                          */
  if (argc != 2){
    printf(" Usage : %s [number-of-threads]\n", argv[0]);
    exit(1);
  }

  nThreads = atoi(argv[1]);
  if (nThreads >= MAXTHREADS) {
    printf(" Too many threads, should be <= %d\n", MAXTHREADS);
    exit(2);
  }

  /* Initialize parameters                                                   */
  srandom(time(NULL));  /* Initialize generator of random numbers            */

  TODO("Create threads");

  TODO("Wait for completion of all threads");

  printf("environment thread (%p) finalized\n", thr_main);
  return EXIT_SUCCESS;
}
