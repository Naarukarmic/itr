/*****************************************************************************/
/* POSIX Lab Session -- Exercise #3                                          */
/*****************************************************************************/

/* In this exercise, we implement a readers/writer lock mechanism
 * using a mutex and condition variables.
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "debug.h"

#define MAXTHREADS 256 /* Maximum number of threads in the program           */

/*---------------------------------------------------------------------------*/
/* Definition of a rw_lock                                                   */
typedef struct {
  pthread_mutex_t lock;       /* Lock                                        */
  pthread_cond_t read_cond;   /* Cond. var. to signal readers                */
  pthread_cond_t write_cond;  /* Cond. var. to signal writers                */
  int writers_waiting;        /* Number of writers waiting for the w_lock    */
  int n_readers;              /* Number of readers who got the r_lock        */
  /* The semantics is the following:
     -1 : a writer got the lock
      0 : the lock is free
  n > 0 : n readers got the lock
  */
} rw_lock_t;

/* R/W lock primitives */
void r_lock   (rw_lock_t *rw_lock);
void r_unlock (rw_lock_t *rw_lock);
void w_lock   (rw_lock_t *rw_lock);
void w_unlock (rw_lock_t *rw_lock);

/*---------------------------------------------------------------------------*/
/* Shared data and associated lock                                           */
long shared_data;
rw_lock_t the_rw_lock;

/*---------------------------------------------------------------------------*/
/* Reader and writer job functions                                           */
void reader_job  (void *args);
void writer_job (void *args);

#define TEMPO_READER  50000
#define TEMPO_WRITER 150000
#define MAXVALUE         20

/* The reader and writer jobs will loop and acquire
 * the_rw_lock. After each loop, they will pause for a multiple of
 * TEMPO_x. Both jobs quits when shared_data reaches MAXVALUE.
 *
 * The following structure is used to pass parameters to both jobs:          */

typedef struct {
  rw_lock_t *Synchro;  /* pointer to the rw_lock                             */
  long sleep_interval; /* time to sleep between two lock acquisition         */
  long increment;      /* increment step for writer threads                  */
} job_parameters_t;

/*****************************************************************************/
/* Main entrypoint for this program                                          */

int main(int argc, char **argv) {
  pthread_t threads[MAXTHREADS];
  job_parameters_t Args[MAXTHREADS];
  int i, n_readers, n_writers;

  if (argc != 3) {
    printf (" Usage : %s #readers #writers\n", argv[0]);
    exit(1);
  }

  n_readers  = atoi(argv[1]);
  n_writers = atoi(argv[2]);

  /* Initialize the shared data */
  if (n_writers == 0)
    shared_data = MAXVALUE;
  else
    shared_data = 1;

  /* Initialize the R/W lock  */
  TODO("initialize all members of the_rw_lock");

  if (n_writers > 0)   /* Create writer threads                              */
    for (i = 0; i < n_writers ; i++){
      Args[i].sleep_interval= TEMPO_WRITER + i * 400000;
      Args[i].increment = i+1;
      pthread_create (&threads[i], NULL, (void *)writer_job, &Args[i]);
    }

  if (n_readers > 0)   /* Create reader threads                              */
    for (i = n_writers; i < n_writers + n_readers; i++) {
      Args[i].sleep_interval = TEMPO_READER + (i - n_writers)* 200000;
      pthread_create (&threads[i], NULL, (void *)reader_job, &Args[i]);
    }

  /* Wait for all threads to finish */
  for (i = 0; i < n_writers + n_readers ; i++)
    pthread_join (threads[i], NULL);

  return (0);
}

/*****************************************************************************/
/* Reader job                                                                */

void reader_job (void *args){
  job_parameters_t *Ptr = (job_parameters_t *)args;
  pthread_t Tid = pthread_self();
  int local_copy;

  do {
    r_lock (&the_rw_lock);
    local_copy = shared_data;
    usleep (Ptr->sleep_interval);
    printf ("* Reader (Tid :%p) : reads = %d\n", Tid, local_copy);
    r_unlock (&the_rw_lock);
    usleep (Ptr->sleep_interval);
  } while (local_copy < MAXVALUE);

  printf ("Reader (Tid :%p): RIP.\n", Tid);
}

/*****************************************************************************/
/* Writer job                                                                */

void writer_job (void *args){
  job_parameters_t *Ptr = (job_parameters_t *)args;
  pthread_t Tid = pthread_self();

  while (shared_data < MAXVALUE) {
    w_lock (&the_rw_lock);
    shared_data = shared_data + Ptr->increment;
    usleep (Ptr->sleep_interval);
    printf ("+ Writer (Tid :%p): adds %ld -> %ld\n",
	    Tid, Ptr->increment, shared_data);
    w_unlock (&the_rw_lock);
    usleep (10 * Ptr->sleep_interval);
  }

  printf ("Writer %p: RIP.\n", pthread_self());
}

/*****************************************************************************/
/* Read lock functions                                                       */

void r_lock (rw_lock_t *rw_lock){
  pthread_t Tid = pthread_self();


  printf ("r_lock : Reader (Tid :%p) w_w =%d, n_readers = %d \n",
	  Tid, rw_lock->writers_waiting, rw_lock->n_readers );

  /* We block the reader function iff there are already writers
     waiting for the lock or if one already got the lock */
  TODO("Complete this part");

  printf ("r_lock : Reader (Tid :%p) unblocked\n",  Tid);
}

/*---------------------------------------------------------------------------*/
void r_unlock (rw_lock_t *rw_lock){
  pthread_t Tid = pthread_self();


  /* If the thread exiting is the last reader, and writers are
     waiting, free one writer */
  TODO("Complete this part");

  printf ("r_unlock : Reader (Tid :%p) unlocking resource\n",  Tid);
}

/*****************************************************************************/
/* Write lock functions                                                      */

void w_lock (rw_lock_t *rw_lock){
  pthread_t Tid = pthread_self();


  printf ("w_lock :Writer (Tid :%p) w_w =%d, n_readers = %d \n",
	  Tid, rw_lock->writers_waiting,rw_lock->n_readers);

  /* We loop as long as at least one reader or a writer has the lock */
  TODO("Complete this part");
}

/*---------------------------------------------------------------------------*/
void w_unlock (rw_lock_t *rw_lock) {


  printf ("w_unlock : Writer (Tid :%p) w_w =%d, n_readers = %d \n",
	  pthread_self(), rw_lock->writers_waiting,rw_lock->n_readers );

  /* If there is no writer waiting, we can free all readers, else free
     one writer */
  TODO("Complete this part");
}
