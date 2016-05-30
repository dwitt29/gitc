#include <stdio.h>

int open_log_file();
void write_logfile();
void close_log_file();

main()
{
  char * pathname="/tmp/test.out";
  int log_fd;
  int sleep_time=0;
  char * str_sleep_time="5";
  char * msgtxt = NULL;
  char * logmsg = NULL; 
  short int port = 0;
  char * str_port = "10000";  /* get from env */
  int socket1 = 0;
  
  port=(short int)atoi(str_port);
  sleep_time=atoi(str_sleep_time);
  
  log_fd=open_log_file(pathname);

  msgtxt="Building listener at port=";
  logmsg=(char *)malloc((strlen(msgtxt)+strlen(str_port)+1)); 
  strcpy(logmsg,msgtxt);
  strcat(logmsg,str_port);
  write_logfile(&log_fd,logmsg);  
  free(logmsg);

  socket1=build_server(port); 

  msgtxt="Sleep(secs)=";
  logmsg=(char *)malloc((strlen(msgtxt)+strlen(str_sleep_time)+1)); 
  strcpy(logmsg,msgtxt);
  strcat(logmsg,str_sleep_time);
  write_logfile(&log_fd,logmsg);  
  free(logmsg);

  sleep(sleep_time);

  msgtxt="Closing listener at port=";
  logmsg=(char *)malloc((strlen(msgtxt)+strlen(str_port)+1)); 
  strcpy(logmsg,msgtxt);
  strcat(logmsg,str_port);
  write_logfile(&log_fd,logmsg);  
  free(logmsg);

  close(socket1); 
  close_log_file(&log_fd);
}
