#include "bignum.h"

Bignum::Bignum() {
    char val[2] = "0";
    init(val);
}

Bignum::Bignum(char * num) {
    init(num);
}

Bignum::Bignum(int num) {
    initSize<int>(num);
    neg = num < 0;
    data = new uint8_t[dataSize]; 
    for(int i = 0; i < dataSize; i++)
    {
        data[i] = num % 100;
        num /= 100;
    }
}

Bignum::Bignum(const Bignum& num) {
    dataSize = num.dataSize;
    neg = num.neg;
    data = new uint8_t[dataSize];
    for(int i = 0; i < dataSize; i++) {
        data[i] = num.data[i];
    }
}

void Bignum::operator=(const Bignum& num) {
    dataSize = num.dataSize;
    neg = num.neg;
    delete data;
    data = new uint8_t[dataSize];
    for(int i = 0; i< dataSize; i++){
        data[i] = num.data[i];
    }
}

Bignum::Bignum(const uint8_t d[], const uint32_t s, bool n) {
    dataSize = s;
    neg = n;
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

ostream & operator<< (ostream & o, const Bignum & num) {
    if (num.neg) {
        o << "-";
    }
    for(int i = num.dataSize - 1; i >= 0; i--) {
        if (num.data[i] < 10 && i != num.dataSize - 1) {
            o << '0';
        }
        o << (int)num.data[i]; 
    }
    return o;
}

bool Bignum::operate(const Bignum& num, bool (*fptr)( int, int)) {
    if (dataSize == num.dataSize) {
        for(int i = dataSize-1; i >= 0; i--) {
            if (data[i] != num.data[i]) {
                return fptr(data[i], num.data[i]);
            }
        }
        return fptr(1,1);
    } else {
        return fptr(dataSize, num.dataSize);
    }
}

int Bignum::findSign(const Bignum& num) {
    if (num.neg == true && neg == true) {
        return -1;
    } else if (num.neg ==false && neg == false) {
        return 1;
    } else {
        return 0;
    }
}

bool Bignum::operator<(const Bignum& num) {
    return operate(num, [](int a, int b) {return a < b;});
}

bool Bignum::operator>(const Bignum& num) {
    return operate(num, [](int a, int b) {return a > b;});
}

bool Bignum::operator<=(const Bignum& num) {
    return operate(num, [](int a, int b) {return a <= b;});
}

bool Bignum::operator>=(const Bignum& num) {
    return operate(num, [](int a, int b) {return a >= b;});
}

bool Bignum::operator==(const Bignum& num) {
    return operate(num, [](int a, int b) {return a == b;});
}

Bignum Bignum::operator+(const Bignum& num) {
    int sign = findSign(num);
    if (sign == 0) {
        Bignum op1 = *this;
        Bignum op2 = num;
        if (this->neg) {
            op1.neg = false;
            return op2.operator-(op1);
        } else {
            op2.neg = false;
            return op1.operator-(op2);
        }
    }

    uint32_t big = *this > num ? dataSize + 1 : num.dataSize + 1;
    uint8_t * newData = new uint8_t[big];
    uint8_t ret = 0;
    Bignum result;
    for(int i = 0; i < big; i++) {
        if (dataSize > i && num.dataSize > i) {
            newData[i] = (data[i] + num.data[i] + ret) % 100;
            ret = (data[i] + num.data[i] + ret) / 100;
        } else if (dataSize > i) {
            newData[i] = (data[i] + ret) % 100;
            ret = (data[i]+ret) / 100;
        } else if (num.dataSize > i) {
            newData[i] = (num.data[i] + ret) % 100;
            ret = (num.data[i]+ret) / 100;
        } else {
            if (ret != 0) {
                newData[i] = ret;
                result = Bignum(newData, big, sign == -1);
                delete newData;
            } else {
                result = Bignum(newData, (big-1), sign == -1); 
                delete newData;
            }
        }
    }
    return result;
}

Bignum Bignum::operator-(const Bignum & num) {
    const Bignum * max;
    const Bignum * min;
    bool invertSign;
    int sign = findSign(num);
    
    if (sign == 0) {
        Bignum op1 = *this;
        Bignum op2 = num;
        if (this->neg) {
            op2.neg = true; 
            return op1 + op2;
        } else {
            op2.neg = false;
            return op1 + op2;
        }
    }

    if (*this <= num) {
        max = &num;
        min = this;
        invertSign = true;
    } else {
        max = this;
        min = &num;
        invertSign = false;
    }

    uint8_t* newData = new uint8_t[max->dataSize];
    uint8_t stole = 0;

    for(int i = 0; i < max->dataSize; i++) {
        if (max->dataSize > i && min->dataSize > i) {
            if (min->data[i] > max->data[i] - stole) {
                newData[i] = (max->data[i] + 100) - min->data[i] - stole;
                stole = 1;
            } else {
                newData[i] = max->data[i] - min->data[i] - stole;
                stole = 0;
            }
        } else if (max->dataSize > i) {
            if (0 > max->data[i]-stole) {
                newData[i] = max->data[i] + 100 - stole;
                stole = 1;
            } else {
                newData[i] = max->data[i] - stole;
                stole = 0;
            }
        }
    }

    uint32_t size = max->dataSize;
    uint32_t i = max->dataSize;
    while (newData[i--] == 0) {
        size--;
    }
    size = size == 0 ? 1 : size;

    bool neg;
    if (sign == -1 && invertSign) {
        neg = false;
    } else if ( sign == 1 && invertSign) {
        neg = true;
    } else if (sign ==-1) {
        neg = true;
    } else {
        neg = false;
    }

    Bignum result = Bignum(newData, size, neg);
    delete newData;
    return result;
}

