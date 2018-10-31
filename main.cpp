#include "bignum.h"
#include <iostream>
#include <stdint.h>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("9999999999999999999999999999");
    uint8_t gf = 116;
    Bignum b = Bignum("-100000000000000000000000000000000");
    Bignum c = b;
    cout << (a-b) * Bignum(100) << endl;
    return 0;
}
