#include "bignum.h"
#include <iostream>
#include <stdint.h>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("999999999999999999999999999999");
    uint8_t gf = 116;
    Bignum b = Bignum("1");
    cout << (a+b) << endl;
    return 0;
}
