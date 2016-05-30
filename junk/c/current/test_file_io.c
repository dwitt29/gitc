/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
*/

char * pathname="/tmp/logfile.out";
int open_fd;

main()
{
 open_fd=open_log_file(pathname);
 
 write_logfile(&open_fd,"Hello Dave\n");

 close_log_file(&open_fd);
}
