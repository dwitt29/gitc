
main(argc ,argv, envp)
int argc;
char *argv[];
{
  char *ptr, *getenv();


  if ( (ptr=getenv("HOME"))==(char *)0 )
    printf("HOME is not defined\n");
  else
    printf("HOME is %s\n",ptr);
 


  exit(0);
}

