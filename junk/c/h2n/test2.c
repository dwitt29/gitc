#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>

#define true	1

void parse_args();
void write_dnsdb_file();
void make_forward_header();
void gethostinfo();

char *  domain,
     *  primary,
     *  secondary,
     *  refresh,
     *  retry,
     *  expire,
     *  email,
     *  mx,
     *  ttl;


void write_dnsdb_file(fd, text)
int * fd;
void * text;
{
  if ( write(*fd, text, strlen(text)) == -1 )
  { printf("%s\n",strerror(errno)); }

}

void parse_args(argc, argv)
int argc;
char  ** argv;
{
  int c;

  while (true)
  {
    c=getopt(argc, argv, "d:p:s:r:t:e:m:x:c:");

    switch(c)
    {
      case 'd':
        domain=optarg;
        printf("Domain=%s\n",domain);
        break;

      case 'p':
        primary=optarg;
        printf("Primary=%s\n",primary);
        break;

      case 's':
        secondary=optarg;
        printf("Secondary=%s\n",secondary);
        break;

      case 'r':
        refresh=optarg;
        printf("Refresh=%s\n",refresh);
        break;

      case 't':
        retry=optarg;
        printf("Retry=%s\n",retry);
        break;
        break;

      case 'c':
        email=optarg;
        printf("Email=%s\n",email);
        break;

      case 'x':
        mx=optarg;
        printf("MX=%s\n",mx);
        break;

      case 'e':
        expire=optarg;
        printf("Expire=%s\n",expire);
        break;

      case 'm':
        ttl=optarg;
        printf("TTL=%s\n",ttl);
        break;
    }

    if (optind == argc)
        break;

  }
}

void gethostinfo(fd,line)
int * fd;
char * line;
{
  
  char **i=NULL;
  struct hostent * hostinfo=NULL;
  struct in_addr * iptr=NULL;

  hostinfo=gethostent();

  while ( hostinfo != NULL )
  {

    for (i=hostinfo->h_addr_list; *i; ++i )
    {
      strcpy(line,"\0");
      sprintf(line,"%s.%s.\tIN A\t%s\n",hostinfo->h_name,domain,inet_ntoa(*(struct in_addr *)*i));
      write_dnsdb_file(&fd, line);
    }

    for (i=hostinfo->h_aliases; *i; ++i )
    {
      strcpy(line,"\0");
      sprintf(line,"%s.%s.\tIN A\t\n",*i,domain);
      write_dnsdb_file(&fd, line);
    }
    hostinfo=gethostent();
  }
}

void make_forward_header(db_file_path)
char * db_file_path;
{
 int fd;
 char * line= (char *)calloc(80,sizeof(char)) ;

 fd=open(db_file_path,O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR);

 strcpy(line,"\0");
 sprintf(line,"%s.\tIN SOA\t%s.%s. %s ( \n",domain,primary,domain,email);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"\t\t\t0\t\t; Serial\n");
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"\t\t\t%s\t\t; Refresh\n",refresh);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"\t\t\t%s\t\t; Retry\n",retry);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"\t\t\t%s\t\t; Expire\n",expire);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"\t\t\t%s )\t\t; TTL\n",ttl);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,";\n; Name servers\n;\n");
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"%s.\tIN NS\t\t%s.%s.\n",domain,primary,domain);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,"%s.\tIN NS\t\t%s.%s.\n",domain,secondary,domain);
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line,";\n; Addresses for canonical names\n;\n");
 write_dnsdb_file(&fd, line);

 gethostinfo(fd,line);

 close(fd);
}

int main(argc,argv)
int argc;
char **argv;
{
  parse_args(argc,argv);
  make_forward_header("/tmp/db.corp");
}
