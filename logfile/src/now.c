#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char * now()
{
  time_t t;
  struct tm * timenow = NULL;
  char * nowis= NULL; 

  nowis=(char *)calloc(24,sizeof(char)); 
  t=time(NULL);
  timenow=localtime(&t);
  sprintf(nowis,"%.2d/%.2d/%.4d %.2d:%.2d:%.2d => \0",(timenow->tm_mon+1),timenow->tm_mday,(timenow->tm_year+1900), timenow->tm_hour,timenow->tm_min,timenow->tm_sec);
  return(nowis); 

/*  printf("%s\n",nowis); */
}
