#include <stdio.h>

main()
{
  int yard, feet, inch;

  printf("Please enter the length to be converted: ");
  scanf("%d",&yard);
  while (yard > 0)
  {
    inch=36*yard;
    feet=3*yard;
    printf("%d yard(s) =\n",yard);
    printf("%d feet(s) \n",feet);
    printf("%d inch(s) \n",inch);
    printf("Please enter the length to be converted: ");
    scanf("%d",&yard);
  }

  printf(">>> End of Program <<<");
  return(0);
}

  
  
