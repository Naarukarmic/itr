/*****************************************************************************/
/* POSIX Lab Session -- Exercise #2                                          */
/*****************************************************************************/

/* In this exercise, we revisit the termination pattern from the
 * previous exercise: the environment thread may wait for any thread,
 * and prints information about the terminated thread.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

#define MAXTHREADS 256 /* Maximum number of threads in the program           */

typedef struct {
  pthread_cond_t   cond_var;          /* signaled when a thread terminates   */
  pthread_mutex_t  mutex;             /* mutex for protecting this state     */
  pthread_t        thread_id;         /* id of the threads that terminates   */
  unsigned int     completed_threads; /* # of terminated threads             */
} shared_state_t;

shared_state_t shared_state;   /* shared state that controls the termination */
sem_t ack_semaphore;           /* semaphore to synchronize threads & env.    */

void *thread_job(void *no_argument);

/*****************************************************************************/
/* Simple job executed by a thread:                                          */
/* - prints some information on the running thread                           */
/* - do some useless iterations                                              */
/* - notify the environment thread that this job has completed its mission   */

void *thread_job(void *no_argument) {
  pthread_t my_task_id = pthread_self();
  int i = 0, iter = labs (random())/10;

  printf("Thread %p : Start processing, %d iterations\n", my_task_id, iter);
  while(i < iter) i++;

  /* Notify the environment thread that this thread is now completed         */
  sem_wait(&ack_semaphore);
  shared_state.thread_id = my_task_id;
  shared_state.completed_threads++;
  sem_post(&ack_semaphore);

  pthread_cond_signal(&shared_state.cond_var);

  printf("Thread %p : End of processing\n", my_task_id);
  pthread_exit(NULL);
}

/*****************************************************************************/
/* Main entrypoint for this program                                          */

int main(int argc, char *argv[]) {
  pthread_t thr_main = pthread_self();
  pthread_t Threads[MAXTHREADS];
  int i, ack_threads, nThreads;

   /* Check command-line parameters                                          */
  if (argc != 2){
    printf(" Usage : %s [number-of-threads]\n", argv[0]);
    exit(1);
  }

  nThreads = atoi(argv[1]);
  if (nThreads > MAXTHREADS){
    printf(" Too many threads, should be <= %d\n", MAXTHREADS);
    exit(2);
  }

  /* Initialize parameters                                                   */
  srandom(time(NULL));  /* Initialize generator of random numbers            */

  ack_threads = 0;
  shared_state.completed_threads = 0;
  pthread_cond_init(&shared_state.cond_var, NULL);
  pthread_mutex_init(&shared_state.mutex, NULL);
  sem_init(&ack_semaphore, 0, 0);

  /* Create threads */
  while (i < nThreads) {
    pthread_create(&Threads[i], NULL, &thread_job, NULL);
    i++;
  }

  /* Wait for completion of all threads */
  sem_post(&ack_semaphore);
  while (ack_threads != nThreads) {
    pthread_mutex_lock(&shared_state.mutex);

    pthread_cond_wait(&shared_state.cond_var, &shared_state.mutex);
    ack_threads++;

    pthread_mutex_unlock(&shared_state.mutex);
  }

  /* End of the program, we print some log and finish the program            */
  printf("environment thread (Tid %p) : end,  %d/%d/%d ",
         thr_main, ack_threads, shared_state.completed_threads, nThreads);

  /* The test is a success iff all created threads performed the notification*/
  if ((ack_threads == shared_state.completed_threads)
      && (ack_threads == nThreads)) {
    printf ("[SUCCESS]\n");
    return EXIT_SUCCESS;
  }
  else {
    printf ("[FAIL]\n");
    return EXIT_FAILURE;
  }
}
