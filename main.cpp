#include "bignum.h"
#include <iostream>
#include <stdint.h>
#include <string>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("2");
    Bignum b = 4568217;
    Bignum c = "4568217";
    Bignum d = Bignum("-100000000000000000000000000000000");
    Bignum e = b;
    cout << 3746487 + d << endl;
    return 0;
}
