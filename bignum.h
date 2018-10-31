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

    void init(const char * num);
    template <class T> void initSize(T num);

    bool operate(const Bignum& num, bool (*fptr)( int, int));
    int findSign(const Bignum& num);
    uint32_t chop(const uint8_t d[], uint32_t currSize);

public:

    static const uint32_t BASE = 100;

    Bignum();
    Bignum(const char * num);
    Bignum(string num);
    Bignum(int num);
    Bignum(const Bignum& num);
    Bignum(const uint8_t d[],const uint32_t s, bool n);
    void operator=(const Bignum& num);
    void operator=(int num);
    void operator=(string num);
    ~Bignum();

    friend ostream & operator<< (ostream &o, const Bignum& num);
    
    bool operator<(const Bignum& num);
    bool operator<(int num);
    friend bool operator<(int num, Bignum num2);
    bool operator<=(const Bignum& num);
    bool operator<=(int num);
    friend bool operator<=(int num, Bignum num2);
    bool operator==(const Bignum& num);
    bool operator==(int num);
    friend bool operator==(int num, Bignum num2);
    bool operator>=(const Bignum& num);
    bool operator>=(int num);
    friend bool operator>=(int num, Bignum num2);
    bool operator>(const Bignum& num);
    bool operator>(int num);
    friend bool operator>(int num, Bignum num2);

    Bignum operator+(const Bignum& num);
    Bignum operator+(int num);
    friend Bignum operator+(int num, Bignum num2);
    Bignum operator-(const Bignum& num);
    Bignum operator*(const Bignum& num);

    void put();

};
