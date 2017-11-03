#include "isocket.h"

#define BUF_SIZE 200

int main() {
    int s, s2;
    int cnt, size = BUF_SIZE;
    char buf[BUF_SIZE];

    char response[] = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n\r\n"
    "<!DOCTYPE html><html><head><title>Mon serveur web</title>"
    "<body bgcolor=lightgreen><h1>Hello from my own web server !</h1></body></html>\r\n";

    // Ouverture de la socket s
    s = i_socket_proto('tcp');

    // Attribution du port 1234 à la socket
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
