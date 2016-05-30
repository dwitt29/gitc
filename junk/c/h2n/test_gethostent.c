#include <netdb.h>


struct hostent * hostinfo=NULL;
char *i=NULL;

struct in_addr *ptr=NULL;

doit(listptr)
char **listptr;
{
  char *ptr=NULL;

  while ( (ptr=(char *)*listptr++) != NULL)
	printf("Addr == %s\n",inet_ntoa(*ptr)); 
}

char line[90];
main()
{

while ( (hostinfo=gethostent()) != NULL )
{
  printf("Host == %s\n",hostinfo->h_name);

  while ( *hostinfo->h_addr_list != NULL )
  {
    ptr=(struct in_addr *)*hostinfo->h_addr_list;     
    printf("Addr == %s\n",inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list)); 
    *hostinfo->h_addr_list++;
  }
}
}
