#include "isocket.h"

#define BUF_SIZE 200

int main() {
  int s;
  int cnt, n, size = BUF_SIZE;
  char buf[BUF_SIZE];

  s = i_socket();

  if(i_connect(s, "localhost", 8080) < 0) {
	  fprintf(stderr, "connection refused\n");
	  exit(1);
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
