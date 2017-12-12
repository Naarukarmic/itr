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
  struct timespec now;
  sem_t timer;
  task_parameters *my_parameters = (task_parameters *) parameters;
  int s;

  period = (*my_parameters).period;
  if (sem_init(&timer, 0, 0) == -1) {
    perror("sem_init");
    exit(EXIT_FAILURE);
  }
  
  clock_gettime(CLOCK_REALTIME, &now);
  add_timespec(&trigger, &now, &period);

  printf("main() about to call sem_timedwait()\n");
  while((s = sem_timedwait(&timer, &trigger)) == -1) {
    (*my_parameters).job();
    continue;
  }

  exit((s == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

/*****************************************************************************/
void create_periodic_task (struct timespec period, void (*job) (void)) {
  pthread_t tid;
  task_parameters *parameters = malloc (sizeof (task_parameters));

  parameters->period = period;
  parameters->job = job;

  pthread_create(&tid, NULL, periodic_task_body, parameters);
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

  create_periodic_task (period, dummy);
  printf ("Task created\n");
  while (1);
  return 0;
}
#endif /* __TEST__ */
