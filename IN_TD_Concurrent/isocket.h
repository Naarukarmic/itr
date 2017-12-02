#ifndef _ISOCKET_H_
#define _ISOCKET_H_ 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

int i_socket();
int i_bind(int s, int port);
int i_accept(int s);
int i_connect(int s, char * host, int port);

#endif /* _ISOCKET_H_ 1 */
