#include "inet.h"
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <mysql/mysql.h>

#define	BUFFMAX	1024
#define TEMP_BUFFER 255
#define OK	1
#define NOTOK	1

int     log_fd = 0;

char  	* env_db_hostname = "db_hostname",
	* env_db_login = "db_login",
	* env_db_login_pw = "db_login_pw",
	* env_db_database = "db_database",
	* env_db_connect_sleep = "db_connect_sleep";
	
char 	* db_hostname_fromenv = NULL,
	* db_login_fromenv = NULL,
	* db_login_pw_fromenv = NULL,
	* db_database_fromenv = NULL,
	* db_connect_sleep_fromenv = NULL;

char 	* db_hostname_label = "Database Hostname : ",
	* db_login_label = "Database Login : ",
	* db_login_pw_label = "Database Login PW : ",
	* db_database_label = "Database database :", 
	* db_connect_sleep_label = "Database retry sleep :"; 

MYSQL * db_connection = NULL;


void chomp(s)
char *s;
{
   if (*s && s[strlen(s)-1]=='\n') 
  {
    s[strlen(s)-1]=0 ;
    Write_Logfile(&log_fd,"** chomping - YUMMY! **");
  }

}

void Get_Env_Vars()
{
   char temp_ptr[TEMP_BUFFER];

   Write_Logfile(&log_fd,"** (start) Database Parameter Info **");

   db_hostname_fromenv=(char *)getenv(env_db_hostname);
   sprintf(temp_ptr,"%s%s",db_hostname_label,db_hostname_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   db_login_fromenv=(char *)getenv(env_db_login);
   sprintf(temp_ptr,"%s%s",db_login_label,db_login_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   db_login_pw_fromenv=(char *)getenv(env_db_login_pw);
   sprintf(temp_ptr,"%s%s",db_login_pw_label,db_login_pw_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   db_database_fromenv=(char *)getenv(env_db_database);
   sprintf(temp_ptr,"%s%s",db_database_label,db_database_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   db_connect_sleep_fromenv=(char *)getenv(env_db_connect_sleep);
   sprintf(temp_ptr,"%s%s",db_connect_sleep_label,db_connect_sleep_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   Write_Logfile(&log_fd,"** (end) Database Parameter Info **");
}

MYSQL * Connect_Database()
{
  MYSQL_RES *results;
  MYSQL_ROW row;
  MYSQL *connection, mysql;
  int state;

  mysql_init(&mysql);
  connection=mysql_real_connect(&mysql,db_hostname_fromenv,db_login_fromenv,db_login_pw_fromenv,db_database_fromenv,0,0,0);

  if (connection == NULL)
  { printf(mysql_error(&mysql)); }

  return connection;
}

char * Now()
{
  time_t t;
  struct tm * timenow = NULL;
  char * nowis = NULL;
  nowis=(char *)malloc(24);

  t=time(NULL);
  timenow=localtime(&t);
  sprintf(nowis,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d => \0",(timenow->tm_mon+1),timenow->tm_mday,(timenow->tm_year+1900), timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
  return(nowis);
}

void Close_Logfile(fd)
int * fd;
{
  close(*fd);
}

int Open_Logfile(file_path)
char * file_path;
{
  int fd=-1;
  fd=open(file_path,O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR);
  return fd;
}

int Write_Logfile(fd, text)
int * fd;
char * text;
{
  char * logrec = NULL;
  time_t t;
  struct tm * timenow = NULL;
  char timeis[24];
  t=time(NULL);
  timenow=localtime(&t);
  sprintf(timeis,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d => \0",(timenow->tm_mon+1),timenow->tm_mday,(timenow->tm_year+1900), timenow->tm_hour,timenow->tm_min,timenow->tm_sec);

  logrec=(char *)malloc((size_t)(strlen(timeis)+strlen(text)+2));
  memset(logrec,'\0',sizeof(logrec));
  strcpy(logrec,timeis);
  strcat(logrec,text);
  strcat(logrec,"\n");

  if ( write(*fd, logrec, strlen(logrec)) == -1 )
  { 
    printf("%s\n",strerror(errno)); 
    free(logrec);
    return NOTOK;
  }
  else
  {
    free(logrec);
    return OK;
  }

}

int create_listener(srv_name, listen_limit)
char * srv_name;
int *listen_limit;
{
  /*
   * VARS
   */

   char temp_ptr[TEMP_BUFFER];
   struct servent * srv_info = NULL;
   char * srv_proto = "tcp";
   struct sockaddr_in srv;
   int turn_on = 1;
   int base_listen_port=0;
   int listen_port=0;
   int listener=0;
   struct linger solinger = { 1,0 };

  srv_info=(struct servent *)calloc(1,(size_t)sizeof(struct servent));  /* allocate "srv_info" storage */
  srv_info = (struct servent *)getservbyname(srv_name,srv_proto);	/* get service info from "srv_name" */
  base_listen_port=listen_port=htons(srv_info->s_port);			/* convert port string info */

  memset(temp_ptr,'\0',sizeof(temp_ptr));

  /* create socket */
  if (  (listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
  {
    sprintf(temp_ptr,"Socket() failed, errno=%d (%s)",errno,strerror(errno));
    Write_Logfile(&log_fd,temp_ptr);
/*    printf("Socket() failed, errno=%d (%s)\n",errno,strerror(errno)); */
    exit(errno);
  }
  else
  {
    sprintf(temp_ptr,"socket() creating fd=%d ok",listener);
    Write_Logfile(&log_fd,temp_ptr);
/*    printf("\nsocket() creating fd=%d ok",listener); */
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
    sprintf(temp_ptr,"fcntl failed,errno=%d (%s)",errno,strerror(errno));
    Write_Logfile(&log_fd,temp_ptr);
/*    printf("\nfcntl failed,errno=%d (%s)",errno,strerror(errno)); */
    exit(errno);
  }
  else
  {
    sprintf(temp_ptr,"fcntl() ok on fd=%d to O_NONBLOCK",listener);
    Write_Logfile(&log_fd,temp_ptr);
/*    printf("\nfcntl() ok on fd=%d to O_NONBLOCK\n",listener); */
  }

  /* bind socket */
/*  if ( bind(listener, (struct sockaddr_in *) &srv, sizeof(srv)) < 0 ) */
  if ( bind(listener, (const struct sockaddr *) &srv, sizeof(srv)) < 0 )
  {
     sprintf(temp_ptr,"bind() failed, errno=%d (%s)",errno,strerror(errno));
     Write_Logfile(&log_fd,temp_ptr);
/*     printf("bind() failed, errno=%d (%s)\n",errno,strerror(errno)); */
     exit(errno);
  }
  else
  {
     sprintf(temp_ptr,"bind() ok on fd=%d",listener);
     Write_Logfile(&log_fd,temp_ptr);
/*     printf("bind() ok on fd=%d\n",listener); */
  }

  /* set socket to listen */
  if ( listen(listener,*listen_limit) < 0 )
  {
     sprintf(temp_ptr,"listen() failed, errno=%d (%s)",errno,strerror(errno));
     Write_Logfile(&log_fd,temp_ptr);
/*     printf("listen() failed, errno=%d (%s)\n",errno,strerror(errno)); */
     exit(errno);
  }
  else
  {
     sprintf(temp_ptr,"listen() ok on fd=%d",listener);
     Write_Logfile(&log_fd,temp_ptr);
/*     printf("listen() ok on fd=%d\n",listener); */
  }
  
  return(listener);		/* return listener handle to caller */

}

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
  char temp_ptr[TEMP_BUFFER];


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
 *  BUILD LISTENER AS NEEDED
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
        sprintf(temp_ptr,"maxfdp1=%d",maxfdp1);
        Write_Logfile(&log_fd,temp_ptr);
/*        printf("maxfdp1=%d\n",maxfdp1); */
      }
    }

    if (listener_is_listening)
    {
      listen_select_expire.tv_sec=(long)1;
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
      {
        sprintf(temp_ptr,"listener select() error, errno=%d",errno);
	Write_Logfile(&log_fd,temp_ptr);
/*        printf("listener select() error, errno=%d\n",errno); */
      }

      if ( debug && listener_found == 0)
      {
        sprintf(temp_ptr,"listener select() empty");
	Write_Logfile(&log_fd,temp_ptr);
/*        printf("listener select() empty\n"); */
      }

      if (listener_found > 0)
      {
        sprintf(temp_ptr,"listener select() triggered");
	Write_Logfile(&log_fd,temp_ptr);
/*        printf("listener select() triggered\n"); */
        if ( FD_ISSET(listener,&fd_listen_readers) )  /* check readers when listener socket connects to client */
        {
          if ( client_connected_count < (*listen_limit) )		
          {
            if ( (clients[client_index]=accept(listener,(struct sockaddr *)&clt,&addr_len)) < 0 )
            {
              sprintf(temp_ptr,"accept() failed, errno=%d, errstr=%s",errno,strerror(errno));
              Write_Logfile(&log_fd,temp_ptr);
/*              printf("accept() failed, errno=%d, errstr=%s\n",errno,strerror(errno)); */
            }
            else
            {  
              sprintf(temp_ptr,"Client %d connected at fd %d",client_index,clients[client_index]);
 	      Write_Logfile(&log_fd,temp_ptr);
/*              printf("Client %d connected at fd %d\n",client_index,clients[client_index]); */
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
  	    sprintf(temp_ptr,"Connected clients limit : %d ",*listen_limit);
 	    Write_Logfile(&log_fd,temp_ptr);
  	    sprintf(temp_ptr,"Connected clients : %d ",client_connected_count);
 	    Write_Logfile(&log_fd,temp_ptr);
/*
  	    printf("Connected clients limit : %d \n",*listen_limit);
  	    printf("Connected clients : %d \n",client_connected_count);
*/
 	    listener_is_listening=0;
  	    sprintf(temp_ptr,"listener at fd=%d closed",listener);
 	    Write_Logfile(&log_fd,temp_ptr);
/*  	    printf("listener at fd=%d closed\n",listener); */
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
/*	debug && printf("FD_SETting fd %d\n",clients[i]); */
	debug && sprintf(temp_ptr,"FD_SETting fd %d",clients[i]) && Write_Logfile(&log_fd,temp_ptr);
      }

      client_select_expire.tv_sec=(long)5;
      client_select_expire.tv_usec=(long)0;

      maxcltfdp1=find_max_fd(clients, client_index);
      maxcltfdp1++;
/*      debug && printf("maxcltfdp1=%d\n",maxcltfdp1); */
      debug && sprintf(temp_ptr,"maxcltfdp1=%d",maxcltfdp1) && Write_Logfile(&log_fd,temp_ptr);
      clients_found=select(maxcltfdp1,&fd_client_readers,NULL,NULL,&client_select_expire); 
  
      if (debug && clients_found == 0)
      {
/*        printf("clients select() empty\n"); */
        sprintf(temp_ptr,"clients select() empty"); 
        Write_Logfile(&log_fd,temp_ptr);
      }
  
      if (clients_found > 0)
      {
        j=0;
        client_index_temp=client_index;
        sprintf(temp_ptr,"client select() triggered");
	Write_Logfile(&log_fd,temp_ptr);
/*        printf("client select() triggered\n"); */
        for (i=0; i<client_index; i++)
        {
          retry=1;
          if ( FD_ISSET(clients[i],&fd_client_readers) )  
          {
 	    bzero((char *)&buff,sizeof(buff));
	    read(clients[i],buff,BUFFMAX);
	    chomp(buff);
	    sprintf(temp_ptr,"Data length from client #%d fd %d : %d",i,clients[i],strlen(buff));
	    Write_Logfile(&log_fd,temp_ptr);
  	    sprintf(temp_ptr,"Data recv from client #%d fd %d : %s",i,clients[i],buff);
	    Write_Logfile(&log_fd,temp_ptr);
/*
	    printf("Data length from client #%d fd %d : %d\n",i,clients[i],strlen(buff));
  	    printf("Data recv from client #%d fd %d : %s\n",i,clients[i],buff);
*/
            if (strlen(buff) == 0)	/* true, when remote client disconnects */
	    {
	      client_connected_count--;
              client_index_temp--;  
              retry=0;
	      shutdown(clients[i],2);
	      close(clients[i]); 
	      FD_CLR(clients[i],&fd_client_readers);
  	      sprintf(temp_ptr,"client #%d fd %d FD_CLRed.",i,clients[i]);
	      Write_Logfile(&log_fd,temp_ptr);
/*  	      printf("client #%d fd %d FD_CLRed.\n",i,clients[i]); */
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

int find_max_fd(clients,client_index)
int *clients;
int client_index;
{
  int i;
  int max_fd=0;

  for (i=0; i<client_index; i++)
    if (clients[i] > max_fd)
      max_fd=clients[i];

  return max_fd;
}

main()
{
  int listen_limit=5;
  int listen_quan=0;
  int sockc[listen_limit];

  char * logfile = "/tmp/hub.log";
  log_fd=Open_Logfile(logfile);

  Get_Env_Vars();

  Write_Logfile(&log_fd,"** trying connect to Database **");
  db_connection=Connect_Database();
  while (db_connection == NULL )
  {  
    Write_Logfile(&log_fd,"** retrying connecting to Database **");
    db_connection=Connect_Database();
  }
  Write_Logfile(&log_fd,"** successful connect to Database **");

  while (listen_quan < listen_limit)
  {
    conn_mgr("test",sockc,&listen_limit);
  }
  Close_Logfile(&log_fd);
}
