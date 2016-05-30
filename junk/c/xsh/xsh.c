#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char first;
char fs[81];
int i=0; 
int ilimit=0; 
int found=0; 
int skip_test=0;
char * gcmd[];
char * cmd=NULL;
char * ptr=NULL;

void build_permitted_cmds();
void run_permitted_cmds();

void build_permitted_cmds()
{
  cmd="exit";
  gcmd[i]=(char *)malloc(strlen(cmd)+1);
  strcpy(gcmd[i++],cmd);

  cmd="pwd";
  gcmd[i]=(char *)malloc(strlen(cmd)+1);
  strcpy(gcmd[i++],cmd);

  cmd="ls";
  gcmd[i]=(char *)malloc(strlen(cmd)+1);
  strcpy(gcmd[i++],cmd);

  ilimit=i;
}

void run_permitted_cmds()
{
  execlp(fs,fs,(char *)0); 
}

main()
{

  build_permitted_cmds();

  scanf("%c",&first);  /* read Xvisions newline char */
  printf("$ ");		/* send $ prompt */

  for (;;)
  {
    fgets(fs,sizeof(fs),stdin);
    if ( fs[strlen(fs)-1] != '\n' )
    {
       printf("BUFFER OVERFLOW ATTEMPT--EXITTING\n");
       exit(1);
    }

    fs[strlen(fs)-1]='\0'; /* replace \n with NULL */

    found=0;
    for(i=0; i<ilimit; i++)
    {

      /* check export cmd */
      skip_test=0;
      if (strcmp("export",fs) == 0)
      {
        skip_test=1;
        break;
      }

      /* check for XYZ test */
      skip_test=0;
      if (strcmp("echo XYZ",fs) == 0)
      {
        printf("XYZ\n");
        skip_test=1;
        break;
      }

      /* check for & set DISPLAY var */
      skip_test=0;
      if (strncmp("DISPLAY=",fs,strlen("DISPLAY=")) == 0)
      {
        ptr=strtok(fs,"=");
        ptr=strtok(NULL,"=");
        setenv("DISPLAY",ptr,1);
        skip_test=1;
        break;
      }

      /* check list of permitted cmds */
      if (strcmp(gcmd[i],fs) == 0)
      {
        found=1;
        break;
      }
      
    }
    
    if (skip_test == 1)
    {
      skip_test=0;
      continue;
    }
    
    if (found == 1)
        run_permitted_cmds();
    else
	exit(2);
   }
 }
