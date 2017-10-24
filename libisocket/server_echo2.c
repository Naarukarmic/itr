#include "isocket.h"

#define BUF_SIZE 500

int main() {
	int s, s2;
    char response[] = "HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8\r\n\r\n"
		"<!DOCTYPE html><html><head><title>Mon serveur web</title>"
		"<body bgcolor=lightgreen><h1>Hello from my own web server !</h1>"
		"<iframe width='560' height='315' src='https://www.youtube.com/embed/dQw4w9WgXcQ' frameborder='0' allowfullscreen></iframe></body></html>\r\n";

	s = i_socket();

	if(i_bind(s, 8080) < 0) {
	    fprintf(stderr, "bind failed\n");
	    exit(1);
    }

    for(;;) {
	    s2 = i_accept(s);
		fprintf(stderr, "connexion client: %d\n", s2);

	    write(s2, response, sizeof(response));
	
        close(s2);
	fprintf(stderr, "déconnexion\n");
   }
}