#include "bignum.h"

/*********************************************************************
* Initializer
*********************************************************************/
Bignum::Bignum()
{
  char val[2] = "0";
  init(val);
}

Bignum::Bignum(const char * num)
{
  init(num);
}

Bignum::Bignum(string num)
{
  init(num.c_str());
}

Bignum::Bignum(int num)
{
  initSize<int>(num);
  neg = num < 0;
  data = new uint8_t[dataSize];
  for (int i = 0; i < dataSize; i++) {
    data[i] = num % BASE;
    num /= BASE;
  }
}

Bignum::Bignum(const Bignum & num)
{
  dataSize = num.dataSize;
  neg = num.neg;
  data = new uint8_t[dataSize];
  for (int i = 0; i < dataSize; i++) {
    data[i] = num.data[i];
  }
}

Bignum::Bignum(const uint8_t d[], const uint32_t s, bool n)
{
  dataSize = s;
  neg = n;
  data = new uint8_t[s];
  for (int i = 0; i < s; i++) {
    data[i] = d[i];
  }
}

void Bignum::operator=(const Bignum & num)
{
  dataSize = num.dataSize;
  neg = num.neg;
  delete data;
  data = new uint8_t[dataSize];
  for (int i = 0; i < dataSize; i++) {
    data[i] = num.data[i];
  }
}

void Bignum::operator=(int num)
{
  *this = Bignum(num);
}

void Bignum::operator=(string num)
{
  *this = Bignum(num.c_str());
}

void Bignum::init(const char * num)
{
  if (num[0] == '-') {
    neg = true;
    num = num + 1;
  }
  else if (num[0] == '+') {
    neg = false;
    num = num + 1;
  }
  else {
    neg = false;
  }

  int size = strlen(num);
  dataSize = size / 2 + size % 2;
  data = new uint8_t[dataSize];
  for (int i = size - 1; i >= 0; i -= 2) {
    uint8_t first = num[i] - '0';
    uint8_t second = ((i - 1) >= 0) ? (num[i - 1] - '0') * 10 : 0;
    data[(size / 2) - (i / 2) - (i % 2)] = first + second;
  }
}

template<class T>
void Bignum::initSize(T num)
{
  if (num == 0) {
    dataSize = 1;
    return;
  }
  uint32_t count = 0;
  int finalValue = num < 0 ? -1 : 0;
  while (num != finalValue) {
    num /= BASE;
    count++;
  }
  dataSize = count;
}

Bignum::~Bignum()
{
  delete[] data;
}

/*********************************************************************
* Printing
*********************************************************************/
ostream & operator<<(ostream & o, const Bignum & num)
{
  if (num.neg) {
    o << "-";
  }
  for (int i = num.dataSize - 1; i >= 0; i--) {
    if (num.data[i] < 10 && i + 1 != num.dataSize) {
      o << "0";
    }
    o << (int)num.data[i];
  }
  return o;
}

/*********************************************************************
* Helpers
*********************************************************************/
bool Bignum::operate(const Bignum & num, bool (*fptr)(int, int)) const
{
  if (dataSize == num.dataSize) {
    for (int i = dataSize - 1; i >= 0; i--) {
      if (data[i] != num.data[i]) {
        return fptr(data[i], num.data[i]);
      }
    }
    return fptr(1, 1);
  }
  else {
    return fptr(dataSize, num.dataSize);
  }
}

int Bignum::findSign(const Bignum & num) const
{
  if (num.neg == true && neg == true) {
    return -1;
  }
  else if (num.neg == false && neg == false) {
    return 1;
  }
  else {
    return 0;
  }
}

uint32_t Bignum::chop(const uint8_t d[], uint32_t size) const
{
  uint32_t i = size - 1;
  while (i > 0 && d[i--] == 0) {
    size--;
  }
  return size == 0 ? 1 : size;
}

/*********************************************************************
* Bool Operator
*********************************************************************/
bool Bignum::operator<(const Bignum & num) const
{
  return operate(num, [] (int a, int b) {
    return a < b;
  });
}

bool Bignum::operator<(int num) const
{
  return *this < Bignum(num);
}

bool operator<(int num, const Bignum & num2)
{
  return num2 < (num);
}

bool Bignum::operator>(const Bignum & num) const
{
  return operate(num, [] (int a, int b) {
    return a > b;
  });
}

bool Bignum::operator>(int num) const
{
  return *this > Bignum(num);
}

bool operator>(int num, const Bignum & num2)
{
  return num2 > (num);
}

bool Bignum::operator<=(const Bignum & num) const
{
  return operate(num, [] (int a, int b) {
    return a <= b;
  });
}

bool Bignum::operator<=(int num) const
{
  return *this <= Bignum(num);
}

bool operator<=(int num, const Bignum & num2)
{
  return num2 <= (num);
}

bool Bignum::operator>=(const Bignum & num) const
{
  return operate(num, [] (int a, int b) {
    return a >= b;
  });
}

bool Bignum::operator>=(int num) const
{
  return *this >= Bignum(num);
}

bool operator>=(int num, const Bignum & num2)
{
  return num2 >= (num);
}

bool Bignum::operator==(const Bignum & num) const
{
  return operate(num, [] (int a, int b) {
    return a == b;
  });
}

bool Bignum::operator==(int num) const
{
  return *this == Bignum(num);
}

bool operator==(int num, const Bignum & num2)
{
  return num2 == (num);
}

/*********************************************************************
**********************************************************************
*
* Arithmetics Operations
*
**********************************************************************
*********************************************************************/

/*********************************************************************
* Addition
*********************************************************************/
void Bignum::additionStep(uint8_t * result, const Bignum & num1, const Bignum & num2, uint8_t & ret, int index) const
{
  if (num1.dataSize > index && num2.dataSize > index) {
    *result = (num1.data[index] + num2.data[index] + ret) % BASE;
    ret = (num1.data[index] + num2.data[index] + ret) / BASE;
  }
  else if (num1.dataSize > index) {
    *result = (num1.data[index] + ret) % BASE;
    ret = (num1.data[index] + ret) / BASE;
  }
  else if (num2.dataSize > index) {
    *result = (num2.data[index] + ret) % BASE;
    ret = (num2.data[index] + ret) / BASE;
  }
  else if (ret != 0) {
    *result = ret;
  }
}

Bignum Bignum::operator+(const Bignum & num) const
{
  int sign = findSign(num);
  if (sign == 0) {
    if (this->neg) {
      return num - (+(*this));
    }
    else {
      return (*this) - (+num);
    }
  }

  uint32_t maxSize = *this > num ? dataSize + 1 : num.dataSize + 1;
  uint8_t * newData = new uint8_t[maxSize];
  uint8_t ret = 0;
  for (int i = 0; i < maxSize; i++) {
    additionStep(newData + i, *this, num, ret, i);
  }
  Bignum result = Bignum(newData, ret == 0 ? maxSize - 1 : maxSize, sign == -1);
  delete[] newData;
  return result;
}

Bignum Bignum::operator+(int num) const
{
  return this->operator+(Bignum(num));
}

Bignum operator+(int num, const Bignum & num2)
{
  return num2.operator+(Bignum(num));
}

Bignum Bignum::operator+() const
{
  Bignum ans = *this;
  ans.neg = false;
  return ans;
}

void Bignum::operator+=(const Bignum & num)
{
  *this = this->operator+(num);
}

void Bignum::operator+=(int num)
{
  *this = this->operator+(num);
}

/*********************************************************************
* Substraction
*********************************************************************/
void Bignum::substractionStep(uint8_t * result, const Bignum * max, const Bignum * min, uint8_t & stole, int index) const
{
  if (max->dataSize > index && min->dataSize > index) {
    if (min->data[index] > max->data[index] - stole) {
      *result = (max->data[index] + BASE) - min->data[index] - stole;
      stole = 1;
    }
    else {
      *result = max->data[index] - min->data[index] - stole;
      stole = 0;
    }
  }
  else if (max->dataSize > index) {
    if (0 > max->data[index] - stole) {
      *result = max->data[index] + BASE - stole;
      stole = 1;
    }
    else {
      *result = max->data[index] - stole;
      stole = 0;
    }
  }
}

Bignum Bignum::operator-(const Bignum & num) const
{
  int sign = findSign(num);

  if (sign == 0) {
    if (this->neg) {
      return -num + (*this);
    }
    else {
      return num + (+(*this));
    }
  }

  const Bignum * max = *this <= num ? &num : this;
  const Bignum * min = *this <= num ? this : &num;
  bool invertSign = *this <= num;

  uint8_t * newData = new uint8_t[max->dataSize];
  uint8_t stole = 0;

  for (int i = 0; i < max->dataSize; i++) {
    substractionStep(newData + i, max, min, stole, i);
  }

  uint32_t size = chop(newData, max->dataSize);

  Bignum result = Bignum(newData, size, (sign == -1) ^ (invertSign)); // xor operator let invert sign invert the sign
  delete[] newData;
  return result;
}

Bignum Bignum::operator-(int num) const
{
  return this->operator-(Bignum(num));
}

Bignum operator-(int num, const Bignum & num2)
{
  return Bignum(num).operator-(num2);
}

Bignum Bignum::operator-() const
{
  Bignum ans = *this;
  ans.neg = !ans.neg;
  return ans;
}

void Bignum::operator-=(const Bignum & num)
{
  *this = this->operator-(num);
}

void Bignum::operator-=(int num)
{
  *this = this->operator-(num);
}

Bignum Bignum::operator*(const Bignum & num) const
{
  int sign = findSign(num);
  uint32_t size = dataSize + num.dataSize;

  uint8_t * newData = new uint8_t[size];
  for (uint32_t i = 0; i < size; i++) {
    newData[i] = 0;
  }

  uint16_t ret = 0;
  for (uint32_t i = 0; i < dataSize; i++) {
    for (uint32_t j = 0; j < num.dataSize; j++) {
      uint16_t val = (uint16_t)data[i] * (uint16_t)num.data[j] + ret;
      newData[i + j] += (uint8_t)(val % BASE);
      ret = val / BASE + newData[i + j] / BASE;
      newData[i + j] %= BASE;
      if (j + 1 == num.dataSize) {
        newData[i + j + 1] = ret;
        ret = 0;
      }
    }
  }
  size = chop(newData, size);

  Bignum result = Bignum(newData, size, sign == 0);
  delete[] newData;
  return result;
}

Bignum Bignum::operator*(int num) const
{
  return this->operator*(Bignum(num));
}

Bignum operator*(int num, const Bignum & num2)
{
  return num2.operator*(num);
}

void Bignum::operator*=(const Bignum & num)
{
  *this = this->operator*(num);
}

void Bignum::operator*=(int num)
{
  *this = this->operator*(num);
}

void Bignum::initDivision(const Bignum & num, Bignum & quotient, Bignum & denominator, int & index) const
{
  index = 1;
  denominator = +num;
  quotient = +(*this);

  while (denominator * BASE <= quotient) {
    index += 1;
    denominator = denominator * BASE;
  }
}

void Bignum::diviseStep(Bignum & reminder, Bignum & denominator, uint8_t * newData) const
{
  uint8_t diviser = 0;
  while (reminder >= (denominator * (diviser + 1))) {
    diviser++;
  }
  *newData = diviser;
  reminder = reminder - (denominator * (diviser));
  // divide by 100
  denominator = Bignum(denominator.data + 1, denominator.dataSize - 1, false);
}

Bignum Bignum::operator/(const Bignum & num) const
{
  bool sign = findSign(num) == 0;
  if (*this < num) return Bignum(0);

  int maxIndex;
  Bignum denominator, quotient;

  initDivision(num, quotient, denominator, maxIndex);

  uint8_t * newData = new uint8_t[maxIndex];

  for (int i = maxIndex - 1; i >= 0; i--) {
    diviseStep(quotient, denominator, newData + i);
  }

  Bignum ans = Bignum(newData, maxIndex, sign);
  delete[] newData;
  return ans;
}

Bignum Bignum::operator/(int num) const
{
  return this->operator/(Bignum(num));
}

Bignum operator/(int num, const Bignum & num2)
{
  return Bignum(num) / num2;
}

Bignum Bignum::operator%(const Bignum & num) const
{
  bool sign = findSign(num) == 0;
  if (*this < num) return Bignum(0);

  int maxIndex;
  Bignum denominator, reminder;

  initDivision(num, reminder, denominator, maxIndex);

  uint8_t * newData = new uint8_t[maxIndex];

  for (int i = maxIndex - 1; i >= 0; i--) {
    diviseStep(reminder, denominator, newData + i);
  }

  reminder.neg = sign;
  return reminder;
}

Bignum Bignum::operator%(int num) const
{
  return this->operator%(Bignum (num));
}

Bignum operator%(int num, const Bignum & num2)
{
  return Bignum(num).operator%(num2);
}