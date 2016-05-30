#include <sys/time.h>

main()
{
  struct timeval now;
  struct timezone  *tz = NULL;

  
  if ((gettimeofday(&now, tz)) == 0 )
    printf("time of day acquired"); 
  else
    printf("gettimeofday() failed"); 
}
