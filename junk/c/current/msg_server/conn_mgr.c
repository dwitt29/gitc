#include "inet.h"
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFMAX 1024

int conn_mgr(srv_name,clients,listen_limit)
char * srv_name;
int *clients;
int *listen_limit;
{
  /*
   *  1-socket()
   *  2-bind()
   *  3-listen()
   */

  char	buff[BUFFMAX];
  int i=0;
  int j=0;
  int maxcltfdp1=0;
  int client_index=0;
  int client_index_temp=0;
  int listener_is_listening=0;
  struct sockaddr_in clt;
  int listener=0;
  int listener_found = 0;
  int clients_found = 0;
  int addr_len = 0;
  int client_connected_count = 0;
  int debug=1;
  int maxfdp1=0;
  struct linger solinger = { 1,0 };
  int sockt[(*listen_limit)];
  int retry=0;


  /*
   * create vars for select() to use when looking for writable, readable, or exceptional fd's
   */
   fd_set fd_listen_readers,
          fd_listen_writers,
          fd_listen_excepts;

   fd_set fd_client_readers,
          fd_client_writers,
          fd_client_excepts;

  /*
   * structure to hold select() timeout period 
   * listen_select_expire == NULL (wait forever)
   * listen_select_expire->tv_sec==0 && select_expire->tv_usec==0 (don't wait at all)
   * listen_select_expire->tv_sec!=0 || select_expire->tv_usec!=0 (wait given sec & usec)
   * listen_select_expire structure values must be reset afer each call to select()
   */
  struct timeval listen_select_expire;
  struct timeval client_select_expire;

/* 
 *  ACTIVATE LISTENER AS NEEDED
 */

  for (;;)
  {
    if ( ! listener_is_listening )
    {
      if ( client_connected_count < *listen_limit )
      {
        listener=create_listener(srv_name,listen_limit);
        maxfdp1=listener;
        maxfdp1++;
        listener_is_listening=1;
        printf("maxfdp1=%d\n",maxfdp1);
      }
    }

    if (listener_is_listening)
    {
      listen_select_expire.tv_sec=(long)0;
      listen_select_expire.tv_usec=(long)0;

      /*
       * Initialize bits in the fd_sets for reading, writing, & execptional fds to zero
       * Set before each looped call to select() 
       */
      FD_ZERO(&fd_listen_readers);
      FD_ZERO(&fd_listen_writers);
      FD_ZERO(&fd_listen_excepts);

      /*
       * Prepare the listener to pop select() when ready to read, write, or handle execptions
       * Set before each looped call to select() 
       */
      FD_SET(listener,&fd_listen_readers);
      FD_SET(listener,&fd_listen_writers);
      FD_SET(listener,&fd_listen_excepts);
  
      listener_found=select(maxfdp1,&fd_listen_readers,&fd_listen_writers,&fd_listen_excepts,&listen_select_expire); 
  
      /* error for select() */
      if (listener_found < 0)
        printf("listener select() error, errno=%d\n",errno);

      if ( debug && listener_found == 0)
        printf("listener select() empty\n");

      if (listener_found > 0)
      {
        printf("listener select() triggered\n");
        if ( FD_ISSET(listener,&fd_listen_readers) )  /* check readers when listener socket connects to client */
        {
          if ( client_connected_count < (*listen_limit) )		
          {
            if ( (clients[client_index]=accept(listener,&clt,&addr_len)) < 0 )
            {
              printf("accept() failed, errno=%d, errstr=%s\n",errno,strerror(errno));
            }
            else
            {  
              printf("Client %d connected at fd %d\n",client_index,clients[client_index]);
              client_index++;
  	      client_connected_count++;
	      setsockopt(clients[client_index],SOL_SOCKET,SO_LINGER,&solinger,sizeof(solinger)); 
            }
          }
  
          /* stop listening when clients reach listen_limit */ 
          if ( client_connected_count == *listen_limit )
          {
    	    close(listener);
	    FD_CLR(listener,&fd_listen_readers);
	    FD_CLR(listener,&fd_listen_writers);
	    FD_CLR(listener,&fd_listen_excepts);
  	    printf("Connected clients limit : %d \n",*listen_limit);
  	    printf("Connected clients : %d \n",client_connected_count);
 	    listener_is_listening=0;
  	    printf("listener at fd=%d closed\n",listener);
    	  }
        }
      }
    }

 /*
  *  HANDLE CLIENT CONNECTIONS
  */

    /* when something is connected */
    if ( client_index > 0)
    {
      FD_ZERO(&fd_client_readers);

      for (i=0; i<client_index; i++)
      {
        FD_SET(clients[i],&fd_client_readers);
	debug && printf("FD_SETting fd %d\n",clients[i]);
      }

      client_select_expire.tv_sec=(long)5;
      client_select_expire.tv_usec=(long)0;

      maxcltfdp1=find_max_fd(clients, client_index);
      maxcltfdp1++;
      debug && printf("maxcltfdp1=%d\n",maxcltfdp1);
      clients_found=select(maxcltfdp1,&fd_client_readers,NULL,NULL,&client_select_expire); 
  
      if (debug && clients_found == 0)
        printf("clients select() empty\n");
  
      if (clients_found > 0)
      {
        j=0;
        client_index_temp=client_index;
        printf("client select() triggered\n");
        for (i=0; i<client_index; i++)
        {
          retry=1;
          if ( FD_ISSET(clients[i],&fd_client_readers) )  
          {
 	    bzero((char *)&buff,sizeof(buff));
	    read(clients[i],buff,BUFFMAX);
	    printf("Data length from client #%d fd %d : %d\n",i,clients[i],strlen(buff));
  	    printf("Data recv from client #%d fd %d : %s\n",i,clients[i],buff);
            if (strlen(buff) == 0)	/* true, when remote client disconnects */
	    {
	      client_connected_count--;
              client_index_temp--;  
              retry=0;
	      shutdown(clients[i],2);
	      close(clients[i]); 
	      FD_CLR(clients[i],&fd_client_readers);
  	      printf("client #%d fd %d FD_CLRed.\n",i,clients[i]);
	    }
          }

          if (retry)
          {
            sockt[j]=clients[i];
            j++;
          }
        }

        if (j)
          clients=sockt;

        client_index=client_index_temp;
      }
    }
  }
}
