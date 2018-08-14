//////////////////////////////////////////////////
//               Example Program                //
//   Prints Fibonacci numbers to fib_out.txt    //
//    until the user terminates the process     //
//////////////////////////////////////////////////

#include <fstream>

#include "BigInt.h"

int main(int argc, char* argv[])
{
  BigInt x;
  BigInt y;
  BigInt result;

  x = 0;
  y = 1;

  ofstream out;
  out.open("fib_out.txt");
  time_t first;
  
  unsigned long long count = 0;

  while (1)
  {
    out << x << "\n";

    result = x + y;
    x = y;
    y = result;
    
    count++;
    if (count % 1000 == 0)
      cout << count << " numbers printed to file.\n";
  }

  out.close();

  return 0;
}