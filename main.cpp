#include "bignum.h"
#include <iostream>
#include <stdint.h>
#include <string>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("2");
    Bignum d = 4568217;
    Bignum e = "4568217";
    uint8_t gf = 116;
    Bignum b = Bignum("-100000000000000000000000000000000");
    Bignum c = b;
    cout << 3746487 + d << endl;
    return 0;
}
