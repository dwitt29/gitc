
#include "inet.h"
#include <string.h>


int build_server(port)
short int port;
{
  /*
   *  1-socket()
   *  2-bind()
   *  3-listen()
   */

  int sock1 = 0;
  struct sockaddr_in srv;
  int q = 5;

  sock1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  bzero((char *) &srv, sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_addr.s_addr=htonl(INADDR_ANY);
  srv.sin_port=htons(port);

  bind(sock1, (struct sockaddr_in *) &srv, sizeof(srv));

  listen(sock1,q);

  return(sock1);
}



