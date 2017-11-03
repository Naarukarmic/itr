/* LIB ISAE SOCKET */

#include "isocket.h"

static struct sockaddr_in portname;

/*
 * Ouverture de la socket
 */

int i_socket_proto(char* proto_name)
{
  int sz = 1;
  int fd;

  fd = socket (PF_INET, SOCK_STREAM, getprotobyname(proto_name)->p_proto);
  setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &sz, 4);
  return (fd);
}

int i_socket()
{
  int sz = 1;
  int fd;

  fd = socket (PF_INET, SOCK_STREAM, 0);
  setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &sz, 4);
  return (fd);
}


/*
 * Attibuer un numéro de port la socket
 * Et prépare la socket à recevoir des connexions
 * Renvoie 0 si OK , -1 sinon
 */

int i_bind(int s, int port)
{
  memset (&portname, 0, sizeof portname);
  /* appel identique à :
   * bzero (&portname, sizeof portname);
   */
  portname.sin_port = htons (port);
  portname.sin_family = AF_INET;
  portname.sin_addr.s_addr = INADDR_ANY;

  /* Association de la socket */
  if (bind (s, (struct sockaddr *) &portname, sizeof portname) != 0)
    return (-1);

  listen (s, 4);
  return (0);
}

/*
 * Accepte une connexion ou se bloque dans l'attente d'une connexion
 * retourne 1 si OK, -1 sinon
 */
int i_accept(int s)
{
  struct sockaddr_in from;
  unsigned int size = 0;

  return (accept (s, (struct sockaddr *) &from, &size));
}

/*
 * Se connecte à un port connu 
 * retourne 0 si OK, -1 sinon
 */
int i_connect(int s, char * host, int port)
{
  struct hostent *hp; /* pour l'adresse du serveur distant */
  //int i;

  /* Traduction d'un nom en adresse IP */
  hp = gethostbyname (host);
  if (hp == NULL)
    return (-1);

  /* Spécifie le port */
  bzero (&portname, sizeof portname);
  portname.sin_port = htons (port);
  portname.sin_family = AF_INET;

  /* Tente la connexion */
   
   memcpy(&portname.sin_addr.s_addr, hp->h_addr_list[0], sizeof(portname.sin_addr.s_addr));
   
   /* L'appel ci-dessus est identique à
	memcpy(&portname.sin_addr.s_addr, hp->h_addr, sizeof(portname.sin_addr.s_addr));
      ou à (attention fonction périmée ajourd'hui mais fonctionne toujours) :	
	bcopy (hp->h_addr_list[0], &portname.sin_addr.s_addr, hp->h_length);
        bcopy (hp->h_addr, &portname.sin_addr.s_addr, hp->h_length);
   */
    
   /* On peut aussi parcourir le tableau :

    for (i = 0; hp->h_addr_list[i] != NULL; i++)
    {
      bcopy (hp->h_addr_list[i], &portname.sin_addr.s_addr, hp->h_length);

      if (connect (s, (struct sockaddr *) &portname, sizeof portname) == 0)
	return (0);
    }
    */


   if (connect (s, (struct sockaddr *) &portname, sizeof portname) == 0)
	return (0);

  return (-1);
}
