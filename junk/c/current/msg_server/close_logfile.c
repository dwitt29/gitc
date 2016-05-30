#include <unistd.h>
void close_log_file(fd)
int * fd;
{
  close(*fd);

}

