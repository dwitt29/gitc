#include <iostream.h>

void function_a(void);
void function_b(void);

extern int int_value;

main()
{

  int_value++;

  cout << "\nin main :: " << int_value;
  function_a();

  return(0);
}

int int_value=10;

void function_a(void)
{
  int_value++;
  cout << "\nin function_a :: " << int_value;
  function_b();
}

#include <iostream.h>

extern int int_value;

void function_b(void)
{
  int_value++;
  cout << "\nin function_b :: " << int_value << "\n";
}


