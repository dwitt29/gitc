#include "inet.h"
#include <fcntl.h>
#include <errno.h>

int create_listener(srv_name, listen_limit)
char * srv_name;
int *listen_limit;
{
  /*
   * VARS
   */

   extern int log_fd;
   char *log_record;
   int buffer_size=1024;
   struct servent * srv_info = NULL;
   char * srv_proto = "tcp";
   struct sockaddr_in srv;
   int turn_on = 1;
   int base_listen_port=0;
   int listen_port=0;
   int listener=0;
   struct linger solinger = { 1,0 };

  log_record=(char *)malloc(buffer_size,sizeof(char));

  srv_info=(struct servent *)calloc(1,(size_t)sizeof(struct servent));  /* allocate "srv_info" storage */
  srv_info = (struct servent *)getservbyname(srv_name,srv_proto);	/* get service info from "srv_name" */
  base_listen_port=listen_port=htons(srv_info->s_port);			/* convert port string info */

  /* create socket */
  if (  (listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
  {
    printf("Socket() failed, errno=%d (%s)\n",errno,strerror(errno));
    sprintf(log_record,"Socket() failed, errno=%d (%s)",errno,strerror(errno));
    write_logfile(&log_fd,log_record);
    exit(errno);
  }
  else
  {
    printf("\nsocket() creating fd=%d ok",listener);
    sprintf(log_record,"socket() creating fd=%d ok",listener);
    write_logfile(&log_fd,log_record);
  }

  /* set socket options */
  setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&turn_on,sizeof(turn_on));
  setsockopt(listener,SOL_SOCKET,SO_LINGER,&solinger,sizeof(solinger));  	/* when socket closes */

  /* fill in socket address info */
  bzero((char *) &srv, sizeof(srv));
  srv.sin_family = AF_INET;
  srv.sin_addr.s_addr=htonl(INADDR_ANY);
  srv.sin_port=htons(listen_port);

  /* set NON-blocking socket */
  if ( fcntl(listener, F_SETFL, O_NONBLOCK) < 0 )
  {
    printf("\nfcntl failed,errno=%d (%s)",errno,strerror(errno));
    sprintf(log_record,"fcntl failed,errno=%d (%s)",errno,strerror(errno));
    write_logfile(&log_fd,log_record);
    exit(errno);
  }
  else
  {
    printf("\nfcntl() ok on fd=%d to O_NONBLOCK\n",listener);
    sprintf(log_record,"fcntl() ok on fd=%d to O_NONBLOCK",listener);
    write_logfile(&log_fd,log_record);
  }

  /* bind socket */
  if ( bind(listener, (struct sockaddr_in *) &srv, sizeof(srv)) < 0 )
  {
    printf("bind() failed, errno=%d (%s)\n",errno,strerror(errno));
    sprintf(log_record,"bind() failed, errno=%d (%s)",errno,strerror(errno));
    write_logfile(&log_fd,log_record);
    exit(errno);
  }
  else
  {
    printf("bind() ok on fd=%d\n",listener);
    sprintf(log_record,"bind() ok on fd=%d",listener);
    write_logfile(&log_fd,log_record);
  }

  /* set socket to listen */
  if ( listen(listener,*listen_limit) < 0 )
  {
     printf("listen() failed, errno=%d (%s)\n",errno,strerror(errno));
     sprintf(log_record,"listen() failed, errno=%d (%s)",errno,strerror(errno));
     write_logfile(&log_fd,log_record);
     exit(errno);
  }
  else
  {
     printf("listen() ok on fd=%d\n",listener);
     sprintf(log_record,"listen() ok on fd=%d",listener);
     write_logfile(&log_fd,log_record);
  }
  
  return(listener);		/* return listener handle to caller */

}
