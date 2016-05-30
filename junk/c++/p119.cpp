#include <iostream.h>

main()
{
  int yard, feet, inch;

  cout << "Please enter the length to be converted:  \n";
  cin >> yard;

  while (yard > 0)
  {
    inch=36*yard;
    feet=3*yard;
    cout << "yard(s) = " << yard << "\n";
    cout << "feet = " << feet << "\n";
    cout << "inch = " << inch << "\n";
    cout << "Please enter the length to be converted:  \n";
    cin >> yard;
  }

  cout << ">>> End of Program <<<";
  return(0);
}

  
  
