#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define true    1

/* funcs */
FILE * fopen_iponly_file();
void fgets_iponly_file();
int open_iponly_file();
int read_iponly_file();
void parse_args();

/* Vars */
int iponlyfd; 
FILE *iponlyFILE;
char * iponlyfilename;
char buffer[80];
int sizeread=0;

struct iponly {  
  char *ipa;
  struct iponly *next_iponly;
};

struct iponly *iponly_head;
struct iponly *iponly_current;

void fgets_iponly_file(f)
FILE *f;
{
  struct iponly *temp_iponly, *i;
  iponly_head=(struct iponly *)malloc(sizeof(struct iponly));
  iponly_current=iponly_head; 
  iponly_current->ipa=NULL;
  iponly_current->next_iponly=NULL;


  memset(buffer,sizeof(buffer),'\0');
  while (fgets(buffer, sizeof(buffer),f) != NULL )
  {
/*    printf("read=%s, len=%d)",buffer,strlen(buffer)); */
    iponly_current->ipa=(char *)malloc(strlen(buffer));
    strncpy(iponly_current->ipa,buffer,strlen(buffer)-1);

    temp_iponly=(struct iponly *)malloc(sizeof(struct iponly));
    iponly_current->next_iponly=temp_iponly;
    iponly_current=temp_iponly;
  }

  for (i=iponly_head; i->next_iponly!=NULL;i=i->next_iponly)
  {
     printf("ip=%s\n",i->ipa);
  }
}

void gethostinfo()
{
  struct iponly *i=NULL;
  struct hostent *hostinfo=NULL;

  for (i=iponly_head; i->next_iponly!=NULL;i=i->next_iponly)
  {
  }

  


}
FILE * fopen_iponly_file(file_path)
char * file_path;
{
  FILE *f=NULL;
  if ( (f=fopen(file_path,"r")) == NULL )
  {
    printf("%s failed fopen(). errno=%d\n",file_path,errno);
    exit(1);
  }
  else
  {
    printf("%s fopen() OK.\n",file_path);
  }
  return f;
}

/*
int open_iponly_file(file_path)
char * file_path;
{
  int fd=-1;
  if ( fd=open(file_path,O_RDONLY|O_NONBLOCK) <= 0 )
  {
    printf("%s failed to open. errno=%d\n",file_path,errno);
    exit(1);
  }
  else
  {
    printf("%s opened. fd=%d\n",file_path,fd);
  }
  return fd;
}

int read_iponly_file(fd)
int fd;
{
  int howmuch=0;
  (ssize_t)howmuch=read(fd, buffer, sizeof(buffer));
  return howmuch;
}

*/

void parse_args(argc, argv)
int argc;
char  ** argv;
{
  int c;

  while (true)
  { 
    c=getopt(argc, argv, "f:");

    switch(c)
    { 
      case 'f':
      iponlyfilename=optarg;
      printf("IP only path name=%s\n",iponlyfilename);
      break;
    } 

    if (optind == argc)
      break;

  }  
} 

int main(argc,argv)
int argc;
char **argv;
{
  parse_args(argc,argv);
  iponlyFILE=fopen_iponly_file(iponlyfilename);


  fgets_iponly_file(iponlyFILE);
}

