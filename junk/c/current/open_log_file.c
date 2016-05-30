#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open_log_file(file_path)
char * file_path;
{
  int fd=-1;
  fd=open(file_path,O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR);
  return fd;
}

