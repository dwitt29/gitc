#include <iostream.h>

main()
{

  long original_flags = cin.flags();
  float float1=3601.234, float2=0.0028, float3=-142.1;

  cout << "\t\t" << float1 << "\t\t";
  cout.setf(ios::scientific);
  cout << float1 << "\n\n";

  cout.setf(ios::fixed);
  cout << "\t\t" << float2 << "\t\t";
  cout.flags(original_flags);
  cout.setf(ios::scientific);
  cout << float2 << "\n\n";
   
  cout.setf(ios::fixed);
  cout << "\t\t" << float3 << "\t\t";
  cout.flags(original_flags);
  cout.setf(ios::scientific);
  cout << float3 << "\n\n";

  cout.flags(original_flags);

  return(0);
}
