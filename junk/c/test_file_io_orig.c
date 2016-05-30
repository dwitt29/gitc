#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

char * pathname="/tmp/logfile.out";
int open_fd;


int open_log_file(file_path)
char * file_path;
{
  int fd=-1; 
  fd=open(file_path,O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR); 
  return fd;
}

void write_logfile(fd, text)
int * fd;
void * text;
{
  if ( write(*fd, text, strlen(text)) == -1 )
  { printf("%s\n",strerror(errno)); }
  
}

void close_log_file(fd)
int * fd;
{
  close(*fd);

}


main()
{
 open_fd=open_log_file(pathname);
 
 write_logfile(&open_fd,"Hello Dave\n");

 close_log_file(&open_fd);
}
