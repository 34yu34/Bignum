#include<stdint.h>
#include<cstring>
#include<iostream>

using namespace std;
class Bignum
{
private:
    uint8_t * data;
    uint32_t dataSize;
    bool neg; 

    void init(char * num);
    template <class T> void initSize(T num);

    bool operate(const Bignum& num, bool (*fptr)( int, int));

public:
    Bignum();
    Bignum(char * num);
    Bignum(int num);
    Bignum(const Bignum& num);
    Bignum(const uint8_t d[],const uint32_t s);
    ~Bignum();

    friend ostream & operator<< (ostream &o, const Bignum&  num);
    const bool operator<(const Bignum& num);
    const bool operator<=(const Bignum& num);
    const bool operator==(const Bignum& num);
    const bool operator>=(const Bignum& num);
    const bool operator>(const Bignum& num);

    const Bignum operator+(const Bignum& num);
    const Bignum operator-(const Bignum& num);

    void put();

};
