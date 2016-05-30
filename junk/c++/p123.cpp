#include <fstream.h>

main()
{

  int earnings, tax;
  ifstream f_in("earnings.dat");
  ofstream f_out("taxrate.dat");


  f_in >> earnings;
  while (! f_in.eof() )
  {
    f_out << "Earnings = $" << earnings << "\n";
    if (earnings < 5000 )
      tax=0.2*earnings;
    else
      tax=0.5*earnings;
    f_out << "Tax = $" << tax << "\n";
  f_in >> earnings;
  }

  f_in.close();
  f_out.close();
  return(0);
}
  

  
