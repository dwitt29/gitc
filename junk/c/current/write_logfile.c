#include <unistd.h>
#include <errno.h>
#include <string.h>

void write_logfile(fd, text)
int * fd;
char * text;
{
  char * logrec = NULL;
  char * timeis = NULL;
  timeis = (char *)now(); 

  logrec=(char *)calloc((size_t)(strlen(timeis)+strlen(text)+2),(size_t)sizeof(char)); 
  strcpy(logrec,timeis);
  strcat(logrec,text);
  strcat(logrec,"\n");

  if ( write(*fd, logrec, strlen(logrec)) == -1 )
  { printf("%s\n",strerror(errno)); }

  free(logrec);

}

