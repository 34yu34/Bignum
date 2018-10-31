#include "bignum.h"

Bignum::Bignum() {
    char * val = "0";
    init(val);
}

Bignum::Bignum(char * num) {
    init(num);
}

Bignum::Bignum(int num) {
    initSize<int>(num);
    data = new uint8_t[dataSize]; 
    for(int i = 0; i < dataSize; i++)
    {
        data[i] = num % 100;
        num /= 100;
    }
}

Bignum::Bignum(const Bignum& num) {
    dataSize = num.dataSize;
    data = new uint8_t[dataSize];
    for(int i = 0; i < dataSize; i++)
    {
        data[i] = num.data[i];
    }
}

Bignum::Bignum(const uint8_t d[],const uint32_t s) {
    dataSize = s;
    data = new uint8_t[s];
    for(int i = 0; i < s; i++)
    {
        data[i] = d[i];
    }
}

void Bignum::init(char * num) {
   
    if (num[0] == '-') {
        neg = true;
        num = num + 1;
    } else if (num[0] == '+') {
        neg = false;
        num = num + 1;
    } else {
        neg = false;
    }
   
    int size = strlen(num);
    dataSize = size / 2 + size % 2;
    data = new uint8_t[dataSize];
    for(int i = size-1; i >= 0; i-=2)
    {
        uint8_t first = num[i] - '0';
        uint8_t second = ((i-1) >= 0) ? (num[i-1] - '0') * 10 : 0;
        data[(size/2) - (i/2) - (i%2)] = first + second;
    }
}

template <class T>
void Bignum::initSize(T num) {
    uint32_t count = 0;
    int finalValue =  num < 0 ? -1 : 0;
    while( num != finalValue ){
        num /= 100;
        count++;
    }
    dataSize = count;
}

Bignum::~Bignum() {
    delete data;
}

ostream & operator<< (ostream & o, const Bignum num) {
    for(int i = num.dataSize - 1; i >= 0; i--) {  
        o << (int)num.data[i]; 
    }
    return o;
}

const bool Bignum::operate(const Bignum& num, bool (*fptr)( int, int)) {
    if (dataSize == num.dataSize) {
        for(int i = dataSize; i >= 0; i--) {
            if (data[i] != num.data[i]) {
                return fptr(data[i], num.data[i]);
            }
        }
        return fptr(1,1);
    } else {
        return fptr(dataSize, num.dataSize);
    }
}

const bool Bignum::operator<(const Bignum& num) {
    return operate(num, [](int a, int b) {return a < b;});
}

const bool Bignum::operator>(const Bignum& num) {
    return operate(num, [](int a, int b) {return a > b;});
}

const bool Bignum::operator<=(const Bignum& num) {
    return operate(num, [](int a, int b) {return a <= b;});
}

const bool Bignum::operator>=(const Bignum& num) {
    return operate(num, [](int a, int b) {return a >= b;});
}

const bool Bignum::operator==(const Bignum& num) {
    return operate(num, [](int a, int b) {return a == b;});
}

const Bignum Bignum::operator+(const Bignum& num) {
    uint32_t small = this->operator<(num) ? dataSize + 1 : num.dataSize + 1;
    uint32_t big = this->operator>(num) ? dataSize + 1 : num.dataSize + 1;
    uint8_t* newData = new uint8_t[big];
    uint8_t ret = 0;
    for(int i = 0; i < big; i++) {
        if (dataSize > i && num.dataSize > i) {
            newData[i] = (data[i] + num.data[i] + ret) % 100;
            ret = (data[i] + num.data[i]) / 100;
        } else if (dataSize > i) {
            newData[i] = (data[i] + ret) % 100;
            ret = data[i] / 100;
        } else if (num.dataSize > i) {
            newData[i] = (num.data[i] + ret) % 100;
            ret = num.data[i] / 100;
        } else {
            if (ret != 0) {
                newData[i] = ret;
                return Bignum(newData, big);
            } else {
                return Bignum(newData, (big-1));
            }
        }
    }
}
