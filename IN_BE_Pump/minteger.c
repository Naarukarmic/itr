#include <stdlib.h>
#include <pthread.h>

#include "minteger.h"
#include "utils.h"

/*****************************************************************************/
m_integer MI_init(int priority) {
  m_integer m;
  m = (m_integer) malloc(sizeof(struct s_m_integer));
  (*m).value = 0;
  /* Configure mutex, using a concurrency control policy */
  pthread_mutex_init(&m->mutex, NULL);
  return m;
}

/*****************************************************************************/
void MI_write(m_integer m, int v) {
  pthread_mutex_lock(&m->mutex);
  (*m).value = v;
  pthread_mutex_unlock(&m->mutex);
}

/*****************************************************************************/
int MI_read(m_integer m) {
  int v;

  pthread_mutex_lock(&m->mutex);
  v=(*m).value;
  pthread_mutex_unlock(&m->mutex);
  return v;
}

/*****************************************************************************/
#ifdef __TEST__
int main (void) {
  m_integer my_minteger = MI_init (150);
  MI_write (my_minteger, 42);
  printf ("Read %d\n", MI_read (my_minteger));
  return 0;
}
#endif /* __TEST__ */
