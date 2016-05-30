#include <stdio.h>

main()
{

  int earnings, tax;
  FILE *fin, *fout;

  fin=fopen("earnings.dat","r");
  fout=fopen("taxrate.dat","w");

  while (fscanf(fin,"%d",&earnings) != EOF )
  {
    fprintf(fout,"Earnings = %d\n ", earnings);
    if (earnings < 5000 )
      tax=0.2*earnings;
    else
      tax=0.5*earnings;
    fprintf(fout,"Tax = %d\n",tax);
  }

  return(0);
}
  

  
