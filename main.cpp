#include "bignum.h"
#include <iostream>
#include <stdint.h>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("534");
    uint8_t gf = 116;
    Bignum b = Bignum("116");
    cout << (a+b) << a << endl;
    return 0;
}
