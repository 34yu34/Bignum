#include "bignum.h"
#include <iostream>
#include <stdint.h>
#include <string>

using namespace std;
int main(int argc, char const * argv[])
{
  Bignum a = Bignum("2");
  Bignum b = 5000000;
  Bignum c = "4568217";
  Bignum cc = 3746487;
  Bignum d = Bignum("-100000000000000000000000000000000");
  Bignum e = b;
  cout << 36 / a << endl;
  return 0;
}