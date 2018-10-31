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
    int findSign(const Bignum& num);
    uint32_t chop(const uint8_t d[], uint32_t currSize);

public:
    Bignum();
    Bignum(char * num);
    Bignum(int num);
    Bignum(const Bignum& num);
    Bignum(const uint8_t d[],const uint32_t s, bool n);
    void operator=(const Bignum& num);
    ~Bignum();

    friend ostream & operator<< (ostream &o, const Bignum& num);
    bool operator<(const Bignum& num);
    bool operator<(int num);
    friend bool operator<(int num, const Bignum& num2);
    bool operator<=(const Bignum& num);
    bool operator<=(int num);
    friend bool operator<=(int num, const Bignum& num2);
    bool operator==(const Bignum& num);
    bool operator==(int num);
    friend bool operator==(int num, const Bignum& num2);
    bool operator>=(const Bignum& num);
    bool operator>=(int num);
    friend bool operator>=(int num, const Bignum& num2);
    bool operator>(const Bignum& num);
    bool operator>(int num);
    friend bool operator>(int num, const Bignum& num2);

    Bignum operator+(const Bignum& num);
    Bignum operator-(const Bignum& num);
    Bignum operator*(const Bignum& num);

    void put();

};
