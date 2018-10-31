#include "bignum.h"
#include <iostream>
#include <stdint.h>

using namespace std;
main(int argc, char const *argv[])
{
    Bignum a = Bignum("99");
    uint8_t gf = 116;
    Bignum b = Bignum("100");
    Bignum c = b;
    cout << (a-b) << endl;
    return 0;
}
