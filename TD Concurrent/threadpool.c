/* Standard C Headers */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/* Thread pool header file */
#include "utils.h"
#include "threadpool.h"

/******************************************************************************/
/* Private types */

/**
 * @brief Here is the definition of a structure representing a task to be
 * executed by a thread pool.
 *
 * @attribute routine The funtion to be called when the task is executed.
 * @attribute data    The data to be sent to the callback function.
 * @attribute next    The next task in the queue that stores the tasks.
 */
typedef struct _pthread_task {
  void (*routine)();
  void *data;
  struct _pthread_task *next;
} pthread_task_t;

/**
 * @brief Here is the definition of a structure representing a synchronized
 * queue where pending tasks are stored before being executed by the thread
 * pool.
 *
 * @attribute mutex The mutex used to synchronize the access to the task queue.
 * @attribute cond  The condition to be used by the threads to wait for tasks.
 * @attribute first The first task in the queue.
 * @attribute last  The last task in the queue.
 * @attribute terminated flag, to say whether the execution has been terminated.
 */

typedef struct _pthread_queue {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_task_t *first;
  pthread_task_t *last;
  bool terminated;
} pthread_queue_t;

/**
 * @brief Here is the definition of a structure representing a thread pool.
 *
 * @attribute mutex   A mutex used to synchronize the access to the thread pool.
 * @attribute queue   The instance of pthread_queue_t used to store the pending
 * tasks.
 * @attribute threads The vector storing the list of threads.
 * @attribute thread_init The number of initialized threads.
 * @attribute thread_count The number of threads in the thread pool.
 */
struct _pthread_pool {
  pthread_mutex_t mutex;
  pthread_queue_t queue;
  pthread_t *threads;
  size_t thread_init;
  size_t thread_count;
};

/******************************************************************************/
/* Private functions */

/*****************************/
/* Task management functions */
/*****************************/

int pthread_task_init(pthread_task_t *task, void (*routine)(), void *data);
/* Initialize a pthread_task_t, can be used also to duplicate safely an entity */

int pthread_task_exec(pthread_task_t *task);
/* Core execution routine */

void *pthread_pool_routine(void *data);
int pthread_async_exec(void (*function)(), void *data);

/******************************/
/* Queue management functions */
/******************************/

int pthread_queue_init(pthread_queue_t *queue);
int pthread_queue_push(pthread_queue_t *queue, const pthread_task_t *task);
int pthread_queue_pop(pthread_queue_t *queue, pthread_task_t *task);
void pthread_queue_shutdown (pthread_queue_t *queue);

/******************************************************************************/
int pthread_task_init(pthread_task_t *task, void (*routine)(), void *data) {
  task->routine = routine;
  task->data = data;
  task->next = NULL;

  return EXIT_SUCCESS;
}

/******************************************************************************/
void pthread_task_destroy(pthread_task_t *task) {

}

/******************************************************************************/
int pthread_task_exec(pthread_task_t *task) {
  assert (task != NULL);
  DEBUG_PRINTF("[EXEC]", "Call user code\n");

  if (task->data == NULL)
    task->routine();
  else
    task->routine(task->data);

  return EXIT_SUCCESS;
}

/******************************************************************************/
int pthread_queue_init(pthread_queue_t *queue) {
  DEBUG_PRINTF("[INIT]", "Queue\n");

  CHECK_NZ (pthread_mutex_init (&(queue->mutex), NULL));
  CHECK_NZ (pthread_cond_init (&(queue->cond), NULL));

  queue->first = NULL;
  queue->last = NULL;
  queue->terminated = false;

  return EXIT_SUCCESS;
}

/******************************************************************************/
int pthread_queue_push(pthread_queue_t *queue, const pthread_task_t *task) {
  pthread_task_t *qtask;

  assert ((queue != NULL) && (task != NULL));
  if ((qtask = malloc(sizeof(*qtask))) == NULL)
    return EXIT_FAILURE;

  /* Duplicate task job, so that it can be safely pushed to the queue */
  CHECK_NZ (pthread_task_init(qtask, task->routine, task->data));

  pthread_mutex_lock(&(queue->mutex));

  /* Queue management: enqueue a job in the FIFO */
  if (queue->last == NULL) {
    queue->first = qtask;
    queue->last = qtask;
  } else {
    queue->last->next = qtask;
    queue->last = qtask;
  }

  /* Signal one pending task */
  pthread_cond_signal(&(queue->cond));
  pthread_mutex_unlock(&(queue->mutex));

  return EXIT_SUCCESS;
}

/******************************************************************************/
int pthread_queue_pop(pthread_queue_t *queue, pthread_task_t *task) {
  pthread_task_t *qtask;

  assert ((queue != NULL) && (task != NULL));
  CHECK_NZ(pthread_mutex_lock(&(queue->mutex)));

  while (queue->first == NULL) {
    DEBUG_PRINTF ("[POP]", "Anything to do ?\n");

    /* Note, we use a while in case of races to fetch a job between
       pending task and a running task that just completed its
       execution
    */

    if (queue->terminated) {
      pthread_mutex_unlock(&(queue->mutex));
      DEBUG_PRINTF("[POP]", "Terminating\n");
      return -1;
    }

    DEBUG_PRINTF ("[POP]", "Waiting for job\n");
    CHECK_NZ (pthread_cond_wait(&(queue->cond), &(queue->mutex)));
  }

  DEBUG_PRINTF ("[POP]", "Processing a job item\n");

  /* Queue management: dequeue a job from the FIFO */
  qtask = queue->first;
  if (queue->first == queue->last)
    queue -> last = NULL;
  queue->first = queue->first->next;

  pthread_mutex_unlock(&(queue->mutex));

  CHECK_NZ (pthread_task_init(task, qtask->routine, qtask->data));
  pthread_task_destroy(qtask);
  free(qtask);

  return EXIT_SUCCESS;
}

/******************************************************************************/
int pthread_pool_exec(pthread_pool_t *thread_pool, void (*function)(), void *data) {
  pthread_task_t *task;
  int err;

  assert ((thread_pool != NULL) && (function != NULL));
  task = malloc (sizeof (pthread_task_t));

  DEBUG_PRINTF ("[EXEC]", "Pushing new job to execute\n");
  CHECK_NZ (pthread_task_init(task, function, data));

  err = pthread_queue_push(&(thread_pool->queue), task);
  pthread_task_destroy(task);

  return err;
}

/******************************************************************************/
void *pthread_pool_routine(void *data) {
  DEBUG_PRINTF ("[POOL]", "Starting\n");

  pthread_pool_t *thread_pool = (pthread_pool_t *)data;
  pthread_task_t task;

  pthread_mutex_lock(&(thread_pool->mutex));
  thread_pool->thread_init += 1;
  pthread_mutex_unlock(&(thread_pool->mutex));

  while (pthread_queue_pop(&(thread_pool->queue), &task) == 0) {
    DEBUG_PRINTF ("[POOL]", "Executing one job\n");
    pthread_task_exec(&task);
  }

  pthread_mutex_lock(&(thread_pool->mutex));
  thread_pool->thread_init -= 1;
  pthread_mutex_unlock(&(thread_pool->mutex));
  DEBUG_PRINTF ("[POOL]", "exit\n");

  return NULL;
}

/******************************************************************************/
pthread_pool_t *pthread_pool_init (int nbThreads) {
  DEBUG_PRINTF ("[INIT]", "Thread pool\n");
  pthread_pool_t *thread_pool = malloc (sizeof (struct _pthread_pool));

  thread_pool->thread_init = 0;
  thread_pool->thread_count = 0;
  CHECK_NZ (pthread_queue_init (&(thread_pool->queue)));
  CHECK_NZ (pthread_mutex_init (&(thread_pool->mutex), NULL));
  thread_pool->threads = malloc (nbThreads * sizeof (pthread_t));

  DEBUG_PRINTF ("[INIT]", "Threads\n");

  for (int i = 0; i <nbThreads; i++) {
    /* Create threads, they will process the pthread_pool_routine()
       function, the last argument is passed to this routine as an
       opaque pointer, to be cast in the corresponding type later.
    */

    CHECK_NZ (pthread_create
	      (&thread_pool->threads[i], NULL, pthread_pool_routine, (void *) thread_pool));
    thread_pool->thread_count++;
  }

  return thread_pool;
}

/******************************************************************************/
void pthread_pool_destroy (void *data) {
  pthread_pool_t *thread_pool = (pthread_pool_t *)data;
  pthread_queue_shutdown (&(thread_pool->queue));
  pthread_mutex_destroy (&(thread_pool->mutex));
}

/******************************************************************************/
void pthread_queue_shutdown (pthread_queue_t *queue) {
  CHECK_NZ (pthread_mutex_lock(&(queue->mutex)));
  queue->terminated = true;
  CHECK_NZ (pthread_cond_broadcast (&(queue->cond)));
  CHECK_NZ (pthread_mutex_unlock(&(queue->mutex)));
  DEBUG_PRINTF ("[QUEUE]", "terminated\n");
}
