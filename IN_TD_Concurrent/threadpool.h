/**
 * Implementation of a threadpool based on the POSIX API
 *
 * Note: it is based on the following article, contributed to Intel
 * https://software.intel.com/fr-fr/articles/Thread-pool
 *
 * Let me note the code was wrong in many aspects:
 * - incorrect queue management for the dequeue case, causing segfaults
 * - inconsistencies in the termination algorithm, leading to deadlocks
 *
 */

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

/* Standard C Headers */
#include <pthread.h>
#include <stdbool.h>

/**
 * Thread pool API
 */

typedef struct _pthread_pool pthread_pool_t;
/** Opaque structure representing a thread pool, see .c for details */

pthread_pool_t *pthread_pool_init (int nbThreads);
/** Initialize a thread pool with nbThreads */

int pthread_pool_exec(pthread_pool_t *thread_pool, void (*function)(), void *data);

/** Have the threadpool execute function(), passing data as optional
    parameter.

    WARNING: data cannot be a variable allocated on the stack, it has
    to be allocated through malloc().

 */

void pthread_pool_destroy (void *data);
//void pthread_pool_destroy (pthread_pool_t *thread_pool);
/** Destroy threadpool, handling graceful termination of the tasks */

#endif /* __THREADPOOL_H__ */
