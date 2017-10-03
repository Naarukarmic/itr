#include "isocket.h"

#define BUF_SIZE 1500

int main() {
   int s, s2;
   int cnt, size = BUF_SIZE;
   char buf[BUF_SIZE];

   s = i_socket();

   if(i_bind(s, 8080) < 0) {
	fprintf(stderr, "bind failed\n");
	exit(1);
   }

   for(;;) {
	s2 = i_accept(s);
	fprintf(stderr, "connexion client: %d\n", s2);
        while((cnt=read(s2, buf, size)) > 0) {
	    	write(s2, buf, cnt);
	}
        close(s2);
	fprintf(stderr, "déconnexion\n");
   }
}
