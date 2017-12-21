#include <stdio.h>
#include <fcntl.h>

int main() 
{
  int fd;
  close(1);
  fd = open("/tmp/logfile",O_CREAT | O_RDWR, 0744);
  write(fd, "Hello", 5);
  printf("World\n");
  return 0;
}
