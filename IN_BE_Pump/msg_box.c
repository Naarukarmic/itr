/**
 * msg_box.c: implement a basic message box of size 1, with message
 *            overwriting
*/

#include <stdlib.h>
#include <string.h>
#include "assert.h"

#include "msg_box.h"
#include "utils.h"

/*****************************************************************************/
msg_box msg_box_init(const unsigned elt_size) {
    msg_box mbox;

    /* Allocate message box */
    if (!(mbox=(msg_box)malloc(sizeof(struct s_msg_box))))
      return 0;

    /* Allocate buffer */
    if (!((*mbox).buf=(char *)malloc(elt_size)))
      return 0;

    (*mbox).empty = true;
    (*mbox).elt_size=elt_size;

    /* Create mutex and cond. var. */
    pthread_mutex_init(&mbox->mutex, NULL);
    pthread_cond_init(&mbox->not_empty, NULL);

    return mbox;
}

/*****************************************************************************/
int msg_box_receive(msg_box mbox, char *buf) {

  /* Wait until the message box has a message */
  pthread_mutex_lock(&mbox->mutex);
  while ((*mbox).empty) {
    pthread_cond_wait(&mbox->not_empty, &mbox->mutex);
  }
  /* Copy the message */
  memcpy(buf,(*mbox).buf,(*mbox).elt_size);
  (*mbox).empty = true;

  pthread_mutex_unlock(&mbox->mutex);
  return (*mbox).elt_size;
}

/*****************************************************************************/
int msg_box_send(msg_box mbox, const char *buf) {

  pthread_mutex_lock(&mbox->mutex);
  /* Copy the message */
  memcpy((*mbox).buf,buf,(*mbox).elt_size);
  (*mbox).empty=false;

  pthread_cond_signal(&mbox->not_empty);
  pthread_mutex_unlock(&mbox->mutex);
  return (*mbox).elt_size;
}

/*****************************************************************************/
void msg_box_delete(msg_box mbox) {
  pthread_cond_destroy(&(*mbox).not_empty);
  pthread_mutex_lock(&(*mbox).mutex);
  free((*mbox).buf);
  pthread_mutex_unlock(&(*mbox).mutex);
  pthread_mutex_destroy(&(*mbox).mutex);
  free(mbox);
}

/*****************************************************************************/
#ifdef __TEST__
int main (void) {
  char c = 'a', d;
  msg_box mbox = msg_box_init (sizeof (char));
  int e = msg_box_send (mbox, &c);
  printf ("msg_box_send: %d\n", e);
  e = msg_box_receive (mbox, &d);
  printf ("Read %c\n", d);
  return 0;
}
#endif /* __TEST__ */
