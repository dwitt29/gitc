#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mntent.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <mcheck.h>


/* Declare funcs */
void Get_Env_Vars();
void Check_Free_Space();
void Check_Free_Space_New();
void Check_Mounted_Filesystems();
int Open_Logfile();
void Write_Logfile();
void Close_Logfile();
char * Now();
char * Get_Logfile_Name();
void Create_Pipe_Fifo();
FILE * Create_Pipe_Popen();
void Open_Pipe_Fifo();
void Close_Pipe_Fifo();
void Close_Pipe_Popen();
void Run_DF();
void Client_Connect();

/* Declare Global Vars */
#define DF_LINE_LEN	132
#define SAME	0
#define TEMP_BUFFER	255

char * df_cmd = NULL,
     * df_format = NULL,
     * fstab_path = NULL;

int     warn_limit = 0;
int     severe_limit = 0;
int     danger_limit = 0;
int     fatal_limit = 0;
int 	listener_portname = 0;
int     log_fd = 0;
int     df_cmd_sleep_time = 0;
int 	listener_fd = 0;

char * env_df_cmd = "df_cmd",
     * env_df_cmd_sleep_time = "df_cmd_sleep_time",
     * env_fstab_path = "fstab_path",
     * env_warn_limit = "warn_limit",
     * env_severe_limit = "severe_limit",
     * env_danger_limit = "danger_limit",
     * env_fatal_limit = "fatal_limit",
     * env_listener_portname = "listener_portname",
     * env_listener_portnum = "listener_portnum",
     * env_listener_hostname = "listener_hostname",
     * env_listener_protocol = "listener_protocol",
     * env_df_format = "df_format";

char * warn_limit_fromenv = NULL,
     * severe_limit_fromenv = NULL,
     * danger_limit_fromenv = NULL,
     * fatal_limit_fromenv = NULL,
     * fstab_path_fromenv = NULL,
     * listener_portname_fromenv = NULL,
     * listener_portnum_fromenv = NULL,
     * listener_hostname_fromenv = NULL,
     * listener_protocol_fromenv = NULL,
     * df_cmd_sleep_time_fromenv = NULL;

FILE * cmd_pipe = NULL;

FILE * fstab_path_file = NULL;

char * logfile = "/tmp/dave.log";
char * pipe_file = "/tmp/df_pipe";
/* const char * pipe_file_mode = "r";  */
const char * pipe_file_mode = "re"; 
/*
char * thishost = NULL;
int thishost_len = 0;
*/

char * temp = NULL, 
     * check_mount = NULL,
     * cmd_for_system = NULL,
     * percent = "%",
     * msg_listener = NULL,
     * fstab_path_label = "FSTAB path = ",
     * df_label = "df command = ",
     * df_cmd_sleep_time_label = "df Sleep Time = ",
     * listener_portname_label = "Send Alerts : Port name = ",
     * listener_portnum_label = "Send Alerts : Port # = ",
     * listener_protocol_label = "Send Alerts : Protocol = ",
     * listener_hostname_label = "Send Alerts : Hostname = ",
     * listener_ip_label = "Send Alerts : IP = ",
     * warn_label = "Warning Alert = ",
     * severe_label = "Severe Alert = ",
     * danger_label = "Danger Alert = ",
     * fatal_label = "Fatal Alert = ";

struct mntent * fstab_record = NULL;

char * logrec = NULL;


void Get_Env_Vars()
{
   char temp_ptr[TEMP_BUFFER];

   Write_Logfile(&log_fd," :: ** FSTAB Info **");
   fstab_path=(char *)getenv(env_fstab_path);
   sprintf(temp_ptr," :: %s%s",fstab_path_label,fstab_path);
   Write_Logfile(&log_fd,temp_ptr);
   Write_Logfile(&log_fd," :: ** DF Info **");
   df_cmd=(char *)getenv(env_df_cmd);
   df_format=(char *)getenv(env_df_format);
   df_cmd_sleep_time_fromenv=(char *)getenv(env_df_cmd_sleep_time);
   df_cmd_sleep_time=atoi( df_cmd_sleep_time_fromenv );
   sprintf(temp_ptr," :: %s%s",df_label,df_cmd);
   Write_Logfile(&log_fd,temp_ptr);

/*
   temp=(char *)calloc((size_t)(strlen(df_label)+strlen(df_cmd)),(size_t)sizeof(char));
   strcpy(temp,df_label);
   strcat(temp,df_cmd);
   Write_Logfile(&log_fd,temp);
   free(temp);
*/

   sprintf(temp_ptr," :: %s%s",df_cmd_sleep_time_label,df_cmd_sleep_time_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   Write_Logfile(&log_fd," :: ** Alert Levels **");

   warn_limit_fromenv=(char *)getenv(env_warn_limit);
   warn_limit=atoi( warn_limit_fromenv );

   severe_limit_fromenv=(char *)getenv(env_severe_limit);
   severe_limit=atoi( severe_limit_fromenv );

   danger_limit_fromenv=(char *)getenv(env_danger_limit);
   danger_limit=atoi( danger_limit_fromenv );

   fatal_limit_fromenv=(char *)getenv(env_fatal_limit);
   fatal_limit=atoi( fatal_limit_fromenv );

   listener_hostname_fromenv=(char *)getenv(env_listener_hostname);

   listener_portname_fromenv=(char *)getenv(env_listener_portname);

   listener_protocol_fromenv=(char *)getenv(env_listener_protocol);

   sprintf(temp_ptr," :: %s%s%s",warn_label,warn_limit_fromenv,percent);
   Write_Logfile(&log_fd,temp_ptr);

   sprintf(temp_ptr," :: %s%s%s",severe_label,severe_limit_fromenv,percent);
   Write_Logfile(&log_fd,temp_ptr);

   sprintf(temp_ptr," :: %s%s%s",danger_label,danger_limit_fromenv,percent);
   Write_Logfile(&log_fd,temp_ptr);

   sprintf(temp_ptr," :: %s%s%s",fatal_label,fatal_limit_fromenv,percent);
   Write_Logfile(&log_fd,temp_ptr);

   sprintf(temp_ptr," :: %s%s",listener_hostname_label,listener_hostname_fromenv);
   Write_Logfile(&log_fd,temp_ptr);

   sprintf(temp_ptr," :: %s%s/%s",listener_portname_label,listener_portname_fromenv,listener_protocol_fromenv);
   Write_Logfile(&log_fd,temp_ptr);
}

void Check_Free_Space()
{
  int size = 0;
  FILE *cmd_pipe=NULL;

  cmd_pipe=Create_Pipe_Popen(df_cmd, pipe_file_mode);
  while ( ! cmd_pipe )
  {
    cmd_pipe=Create_Pipe_Popen(df_cmd, pipe_file_mode);
    sprintf(temp,"Sleeping %d secs",df_cmd_sleep_time);
    Write_Logfile(&log_fd,temp);
    sleep(df_cmd_sleep_time);
  }

  Write_Logfile(&log_fd,"** Checking Free Space **");

  char * fssize = (char *)calloc(DF_LINE_LEN,sizeof(char));
  char * fsname = (char *)calloc(DF_LINE_LEN,sizeof(char));
  char * df_info = (char *)calloc(DF_LINE_LEN, sizeof(char)); 
 
  fgets(df_info,DF_LINE_LEN,cmd_pipe);   /* read header */
  memset(df_info,'\0',sizeof(df_info));   /* skip it */

  while (  fgets(df_info,DF_LINE_LEN,cmd_pipe) )
  {
   sscanf(df_info,"%*s%*s%*s%*s%s%s",fssize,fsname);
   fssize[strlen(fssize)-1] = '\0'; /* null terminate */ 

   sprintf(temp," :: %s is %d%%",fsname,atoi(fssize));

   size=atoi(fssize);
   if ( size > fatal_limit )
      sprintf(temp," :: FATAL :: %s is %d%% full",fsname,size);
   else if ( size > danger_limit )
      sprintf(temp," :: DANGER :: %s is %d%% full",fsname,size);
   if ( size > severe_limit )
      sprintf(temp," :: SEVERE :: %s is %d%% full",fsname,size);
   else if ( size > warn_limit )
      sprintf(temp," :: WARNING :: %s is %d%% full",fsname,size);
   else 
      sprintf(temp," :: %s is %d%%",fsname,atoi(fssize));

   Write_Logfile(&log_fd,temp);
  }

  pclose((FILE *)cmd_pipe);
  
  free(fssize);
  free(fsname);
  free(df_info);
  
} 

void Check_Free_Space_New(fsname,fssize)
char * fssize;
char * fsname;
{
  char temp_ptr[TEMP_BUFFER];
  int size = 0;

  memset(temp_ptr,'\0',sizeof(temp_ptr));
  size=atoi(fssize);
  if ( size > fatal_limit )
     sprintf(temp_ptr," :: FATAL :: %s is %d%% full",fsname,size);
  else if ( size > danger_limit )
     sprintf(temp_ptr," :: DANGER :: %s is %d%% full",fsname,size);
  else if ( size > severe_limit )
     sprintf(temp_ptr," :: SEVERE :: %s is %d%% full",fsname,size);
  else if ( size > warn_limit )
     sprintf(temp_ptr," :: WARNING :: %s is %d%% full",fsname,size);
  else
     sprintf(temp_ptr," :: %s is %d%%",fsname,atoi(fssize));

  Write_Logfile(&log_fd,temp_ptr);

  if ( write(listener_fd, temp_ptr, strlen(temp_ptr)) == -1 )
  { printf("%s\n",strerror(errno)); }
}

void Check_Mounted_Filesystems()
{
  char temp_ptr[TEMP_BUFFER];
  char * fssize = (char *)calloc(DF_LINE_LEN,sizeof(char));
  char * fsname = (char *)calloc(DF_LINE_LEN,sizeof(char));
  char * df_info = (char *)calloc(DF_LINE_LEN, sizeof(char)); 
  char * cmd = NULL;
  FILE * cmd_pipe = NULL; 

  Write_Logfile(&log_fd," :: ** Checking Mounted Filesystems **");

  fstab_path_file = setmntent(fstab_path,"r");

  fstab_record = getmntent(fstab_path_file);

  while ( fstab_record != NULL )
  {
    temp=fstab_record->mnt_type;

    if (strcmp(temp,"ext3") == SAME) 
    {
      cmd=(char *)malloc((sizeof(df_cmd)+sizeof((fstab_record->mnt_dir))+1));
      sprintf(cmd,"%s %s",df_cmd,fstab_record->mnt_dir);

      cmd_pipe=Create_Pipe_Popen(cmd, pipe_file_mode);
      while ( ! cmd_pipe )
      {
        cmd_pipe=Create_Pipe_Popen(df_cmd, pipe_file_mode);
        sprintf(temp_ptr," :: Sleeping %d secs",df_cmd_sleep_time);
        Write_Logfile(&log_fd,temp_ptr);
        sleep(df_cmd_sleep_time);
      }

      fgets(df_info,DF_LINE_LEN,cmd_pipe);    /* header */
      memset(df_info,'\0',sizeof(df_info));   /* skip it */
      fgets(df_info,DF_LINE_LEN,cmd_pipe); 
      sscanf(df_info,"%*s%*s%*s%*s%s%s",fssize,fsname);

      temp=fstab_record->mnt_dir;
 
      if (strcmp(fsname, (fstab_record->mnt_dir))!=SAME)
      { 
        sprintf(temp_ptr," :: ERROR :: %s is NOT MOUNTED ",fstab_record->mnt_dir);
        Write_Logfile(&log_fd,temp_ptr);
        if ( write(listener_fd, temp_ptr, strlen(temp_ptr)) == -1 )
        { printf("%s\n",strerror(errno)); }
      }
      else
      {
        sprintf(temp_ptr," :: %s is MOUNTED ",fstab_record->mnt_dir);
        Write_Logfile(&log_fd,temp_ptr);
        if ( write(listener_fd, temp_ptr, strlen(temp_ptr)) == -1 )
        { printf("%s\n",strerror(errno)); }
        Check_Free_Space_New(fsname,fssize);
      }
      pclose(cmd_pipe);
    }

    fstab_record = getmntent(fstab_path_file);
  } 

 endmntent(fstab_path_file);

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

FILE * Create_Pipe_Popen(cmd, mode)
char * cmd;
const char * mode;
{
  FILE *fp = NULL;

  temp=(char *)calloc((size_t)132,(size_t)sizeof(char));
  sprintf(temp," :: Running popen(%s)",cmd);
  Write_Logfile(&log_fd,temp);
  free(temp);

  fp=popen(cmd, mode); 

  if ( !fp )
  {
    temp=(char *)calloc((size_t)80,(size_t)sizeof(char));
    sprintf(temp," :: ERROR :: %s popen(%s) FAILED",strerror(errno),cmd);
    Write_Logfile(&log_fd,temp);
    free(temp);
  }
  else
  {
    temp=(char *)calloc((size_t)80,(size_t)sizeof(char));
    sprintf(temp," :: OK :: %s popen(%s)",strerror(errno),cmd);
    Write_Logfile(&log_fd,temp);
    free(temp);
  }

  return(fp);
}

void Close_Pipe_Popen(handle)
FILE * handle;
{
  if ( pclose(handle) < 0 )
  {
/*    temp=(char *)calloc((size_t)80,(size_t)sizeof(char)); */
    temp=(char *)malloc((size_t)80);
    sprintf(temp," :: ERROR :: %s pclose() FAILED",strerror(errno));
    Write_Logfile(&log_fd,temp);
    free(temp);
  }
  else
  {
/*    temp=(char *)calloc((size_t)80,(size_t)sizeof(char)); */
    temp=(char *)malloc((size_t)80);
    sprintf(temp," :: OK :: %s pclose()",strerror(errno));
    Write_Logfile(&log_fd,temp);
    free(temp);
  }
}

void Write_Logfile(fd, text)
int * fd;
char * text;
{ 
/*
  char * timeis = NULL;
  timeis = (char *)Now();  
*/
  size_t thishost_len = 50;
  char thishost[thishost_len];
  char * logrec = NULL;
  time_t t;
  struct tm * timenow = NULL;
  char timeis[24];
  t=time(NULL);
  timenow=localtime(&t);
  sprintf(timeis,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d => \0",(timenow->tm_mon+1),timenow->tm_mday,(timenow->tm_year+1900), timenow->tm_hour,timenow->tm_min,timenow->tm_sec);

  gethostname(thishost,thishost_len);
/*  logrec=(char *)calloc((size_t)(strlen(timeis)+strlen(text)+2),(size_t)sizeof(char));  */
  logrec=(char *)malloc((size_t)(strlen(thishost)+strlen(timeis)+strlen(text)+2)); 
  memset(logrec,'\0',sizeof(logrec));
  strcpy(logrec,timeis);
  strcat(logrec,thishost);
  strcat(logrec,text);
  strcat(logrec,"\n");

  if ( write(*fd, logrec, strlen(logrec)) == -1 )
  { printf("%s\n",strerror(errno)); }

/*
  free(timeis);
*/
  free(logrec);

}

void Client_Connect(fd)
int *fd;
{
  char temp_ptr[TEMP_BUFFER];
  int 	msgsrvfd = 0;
  int 	myfd = 0;
  int   msgsrvlen = 0;
  int   mylen = 0;
  char **msgsrv_ip = NULL;
  struct in_addr *msgsrv_ip_ptr = NULL;
  struct in_addr *msgsrv_ip_ptr_save = NULL;
  struct sockaddr_in msgsrv_addr;
  struct sockaddr_in my_addr;
  struct hostent * msgsrv_host = NULL;
  struct servent * msgsrv_port = NULL;

  memset(temp_ptr,'\0',sizeof(temp_ptr));
  msgsrvfd = socket(AF_INET, SOCK_STREAM, 0);

  msgsrv_host = (struct hostent *)gethostbyname(listener_hostname_fromenv); 
  if (msgsrv_host == NULL)
  { printf("gethostbyname() failed %d %s\n",h_errno,(const char *)hstrerror(h_errno)); }

  printf("hostname=%s\n", msgsrv_host->h_name);

  msgsrv_ip=msgsrv_host->h_addr_list;
  while (msgsrv_ip_ptr = (struct in_addr *)*msgsrv_ip++)
  {
    printf("IP address : %s\n", inet_ntoa(*msgsrv_ip_ptr));
    msgsrv_ip_ptr_save=msgsrv_ip_ptr;
  }

  sprintf(temp_ptr," :: %s%s",listener_ip_label,inet_ntoa(*msgsrv_ip_ptr_save));
  Write_Logfile(&log_fd,temp_ptr);

  msgsrv_port = (struct servent *)getservbyname(listener_portname_fromenv, listener_protocol_fromenv); 
  if ( msgsrv_port == NULL )
  {
    sprintf(temp_ptr," :: msgsrv_port is NULL after getservbyname()");
    Write_Logfile(&log_fd,temp_ptr);
    _exit(1); 
  }
  else
  {
    sprintf(temp_ptr," :: %s%d",listener_portnum_label,ntohs(msgsrv_port->s_port));
    Write_Logfile(&log_fd,temp_ptr);
  }
   
  msgsrv_addr.sin_family = AF_INET;
  msgsrv_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*msgsrv_ip_ptr_save));
  msgsrv_addr.sin_port=msgsrv_port->s_port;
  if (connect(msgsrvfd, (struct sockaddr *)&msgsrv_addr,sizeof(msgsrv_addr)) < 0 )
  {
    sprintf(temp_ptr," :: connect() is <0");
    Write_Logfile(&log_fd,temp_ptr);
    close(msgsrvfd);
    _exit(1); 
  }
  *fd=msgsrvfd;
}

char * Get_Logfile_Name()
{
  char *ptr, *getenv();

  if ( (ptr=getenv("LOGFILE"))==(char *)0 )
  {
    printf("LOGFILE is not defined\n");
   _exit(0);
  }
  else
    printf("LOGFILE is %s\n",ptr);

  return ptr;
}


/* main()  */
main(argc ,argv, envp)
int argc;
char *argv[];
{

/*  log_fd=Open_Logfile(logfile); */  /* PASSED checked for memory leaks */
  log_fd=Open_Logfile(Get_Logfile_Name());  /* PASSED checked for memory leaks */

  Get_Env_Vars();
  Client_Connect(&listener_fd);

  for (;;)
  {
/*    Check_Free_Space(); */

    Check_Mounted_Filesystems(); 

    sprintf(temp," :: Sleeping %d secs",df_cmd_sleep_time);
    Write_Logfile(&log_fd,temp);
    sleep(df_cmd_sleep_time);
  }

  Close_Logfile(&log_fd);
}
