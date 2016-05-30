#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define	true	1

/* Vars */
char * 	domain,
     *	primary,
     *	secondary,
     *	refresh,
     *	retry,
     *	expire,
     *	email,
     *	mx,
     *	minimum;
	

int open_hosts_file();
void write_dnsdb_file();
void parse_args();
void make_db_header();

void write_dnsdb_file(fd, text)
int * fd;
void * text;
{
  if ( write(*fd, text, strlen(text)) == -1 )
  { printf("%s\n",strerror(errno)); }

}

void make_db_header(db_file_path)
char * db_file_path;
{
 int fd;
 char * line= (char *)calloc(80,sizeof(char)) ;

 fd=open(db_file_path,O_CREAT|O_APPEND|O_RDWR,S_IRUSR|S_IWUSR); 

 sprintf(line,"@ IN SOA\t%s.%s. %s ( 0 %s %s %s %s )\n",primary,domain,email,refresh,retry,expire,minimum); 
 write_dnsdb_file(&fd, line);

 strcpy(line,"\0");
 sprintf(line," IN NS\t\t%s.%s.\n",primary,domain);
 write_dnsdb_file(&fd, line);
 
 strcpy(line,"\0");
 sprintf(line," IN NS\t\t%s.%s.\n",secondary,domain);
 write_dnsdb_file(&fd, line);

 close(fd);
 /* return fd; */
}

int open_hosts_file(file_path)
char * file_path;
{
  int fd=-1;
  fd=open(file_path,O_RDONLY);
  return fd;
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
        minimum=optarg;
        printf("Minimum=%s\n",minimum);
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
  make_db_header("/tmp/db.corp");
}

