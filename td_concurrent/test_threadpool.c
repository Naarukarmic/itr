/* Standard C Headers */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Thread pool header file */

#include "threadpool.h"
#include "utils.h"

/******************************************************************************/
void routine (void) {
  printf ("**** Hello World ****\n");
  fflush (stdout);
}

/******************************************************************************/

int main (int argc, char **argv) {
  pthread_pool_t *thread_pool = pthread_pool_init(10);

  /* Initialize a thred pool of size 10 */

  printf ("\x1B[32m %s \x1B[0m%s", "[MAIN]", "Start processing\n");

  /* Leave some time for the threadpool to finish its initialization */
  sleep (1);

  /* Feed the queue with 20 jobs */
  for (int i = 0; i < 20; ++i)
    pthread_pool_exec (thread_pool, routine, NULL);

  /* Shutdown the queue */
  pthread_pool_exec
    (thread_pool, pthread_pool_destroy, (void *) thread_pool);

  printf ("About to sleep\n");
  sleep (10);
  printf ("End of main()\n");

  return EXIT_SUCCESS;
}
