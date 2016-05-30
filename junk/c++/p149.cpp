#include <iostream.h>

void function_a(void);

int int_value1=1;

main()
{
  extern int int_value1;

  static int int_value2;

  register int register_value=0;

  int int_value3=0;

  cout << "#1=" << int_value1 << "\n" << "Reg=" << register_value << "\n";
  cout << "#2=" << int_value2 << "\n" << "#3=" << int_value3 << "\n";
  function_a();

  cout << "#2=" << int_value2 << "\n" << "#3=" << int_value3 << "\n";

  function_a();

  return(0);

}

void function_a(void)
{

  static int *pointer_to_int_value1 = &int_value1;

  int int_value1=32;

  static int int_value2=2;

  int_value2+=2;

  cout << "#1=" << int_value1 << "\n" << "#2=" << int_value2 << "\n";
  cout << "p1=" <<  *pointer_to_int_value1 << "\n";

}

