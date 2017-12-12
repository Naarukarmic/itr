#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#include "periodic_task.h"
#include "utils.h"

/*****************************************************************************/
void *periodic_task_body (void *parameters);

void *periodic_task_body (void *parameters) {
  struct timespec trigger;                      /* Stores next dispatch time */
  struct timespec period;                              /* Period of the task */
  sem_t timer;
  int s;
  task_parameters *my_parameters = (task_parameters *) parameters;

  period = (*my_parameters).period;
  s = sem_init(&timer, 0, 0);
  CHECK_NZ(s);
  
  clock_gettime(CLOCK_REALTIME, &trigger);
  while(1) {
    add_timespec(&trigger, &trigger, &period);
    sem_timedwait(&timer, &trigger);
    (*my_parameters).job();
  }

  free(parameters);
}

/*****************************************************************************/
void create_periodic_task (int priority, struct timespec period, 
                           void (*job) (void)) {
  pthread_t tid;
  pthread_attr_t attr;
  int s;
  task_parameters *parameters = malloc (sizeof (task_parameters));

  parameters->period = period;
  parameters->job = job;
  s = pthread_attr_init(&attr);
  CHECK_NZ(s);

  s = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  CHECK_NZ(s);
  struct sched_param param;
  param.sched_priority = priority;
  s = pthread_attr_setschedparam(&attr, &param);
  CHECK_NZ(s);

  s = pthread_create(&tid, &attr, periodic_task_body, parameters);
  CHECK_NZ(s);
  s = pthread_attr_destroy(&attr);
  CHECK_NZ(s);
}
/*****************************************************************************/
#ifdef __TEST__
void dummy (void) {
  printf ("o< \n");
}

int main (void) {
  struct timespec period;
  period.tv_nsec = 250 * 1000 * 1000;
  period.tv_sec  = 0 ;

  create_periodic_task (0, period, dummy);
  printf ("Task created\n");
  while (1);
  return 0;
}
#endif /* __TEST__ */
