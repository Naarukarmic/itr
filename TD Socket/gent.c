/*
** Simple timer avec select()
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "isocket.h"

#define BUF_SIZE 1500


char randomletter() { 
  char res;
  res = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random () % 26];
  return res;
}

int deploy_sock() {
  int s, i, n;
  int cnt, size = BUF_SIZE;
  char buf[BUF_SIZE];

  s = i_socket();
  i = 0;
  n = 0;

  if(i_connect(s, "localhost", 8080) < 0) {
    fprintf(stderr, "connection refused\n");
    exit(1);
  }
  while(i < size) {
    buf[i] = randomletter();
    i++;
  }

  n = 0;
    while((cnt=read(s, buf, size)) > 0) {
      printf("%s\n", buf);
      n += cnt;
      if(n >= 12) break;
  }

  close(s);
  return(0);
}

int main (void) {
  struct timeval tv;
  int i = 5;

  tv.tv_sec = 1;
  tv.tv_usec = 500000;          // 1,5 sec

  while (i)
    {
      select (0, NULL, NULL, NULL, &tv);
      tv.tv_sec = 0;
      tv.tv_usec = 100000;      // 100 ms
      printf ("Timer: %d.\n", i);

      deploy_sock();
      i--;
    }
  return 0;
}