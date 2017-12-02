#ifndef __UTILS_H__
#define __UTILS_H__

#define THREADPOOL_DEBUG

/******************************************************************************/
/* Helper macros */

#define CHECK_NZ(X) if ((X) != 0) { printf ("%s %d:\n", __FILE__, __LINE__); perror ("[ERROR]"); }
/* Return an error if X is not null, e.g. a return from a syscall */

#ifdef THREADPOOL_DEBUG
#define DEBUG_PRINTF(X,Y) printf ("\x1B[32m %s \x1B[0m%s", X, Y)
#else
#define DEBUG_PRINTF(...)
#endif

#endif /* __UTILS_H__ */
