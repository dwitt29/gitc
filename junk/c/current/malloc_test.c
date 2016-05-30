#include <stdlib.h>

main()
{
  char * a1 = NULL;
  char * a2 = NULL;

  a1 = (char *)malloc(10);
  free(a1);
  a1 = (char *)malloc(10);

  a2 = (char *)malloc(10);
}
