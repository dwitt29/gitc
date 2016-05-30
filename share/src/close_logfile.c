#include <unistd.h>
void close_logfile(fd)
int * fd;
{
  close(*fd);

}

