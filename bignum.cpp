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
bool Bignum::operate(const Bignum & num, bool (*fptr)(int, int))
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

int Bignum::findSign(const Bignum & num)
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

uint32_t Bignum::chop(const uint8_t d[], uint32_t size)
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
bool Bignum::operator<(const Bignum & num)
{
  return operate(num, [] (int a, int b) {
    return a < b;
  });
}

bool Bignum::operator<(int num)
{
  return *this < Bignum(num);
}

bool operator<(int num, Bignum num2)
{
  return num2 < (num);
}

bool Bignum::operator>(const Bignum & num)
{
  return operate(num, [] (int a, int b) {
    return a > b;
  });
}

bool Bignum::operator>(int num)
{
  return *this > Bignum(num);
}

bool operator>(int num, Bignum num2)
{
  return num2 > (num);
}

bool Bignum::operator<=(const Bignum & num)
{
  return operate(num, [] (int a, int b) {
    return a <= b;
  });
}

bool Bignum::operator<=(int num)
{
  return *this <= Bignum(num);
}

bool operator<=(int num, Bignum num2)
{
  return num2 <= (num);
}

bool Bignum::operator>=(const Bignum & num)
{
  return operate(num, [] (int a, int b) {
    return a >= b;
  });
}

bool Bignum::operator>=(int num)
{
  return *this >= Bignum(num);
}

bool operator>=(int num, Bignum num2)
{
  return num2 >= (num);
}

bool Bignum::operator==(const Bignum & num)
{
  return operate(num, [] (int a, int b) {
    return a == b;
  });
}

bool Bignum::operator==(int num)
{
  return *this == Bignum(num);
}

bool operator==(int num, Bignum num2)
{
  return num2 == (num);
}

/*********************************************************************
* Arithmetics Operations
*********************************************************************/
Bignum Bignum::operator+(const Bignum & num)
{
  int sign = findSign(num);
  if (sign == 0) {
    Bignum op1 = *this;
    Bignum op2 = num;
    if (this->neg) {
      op1.neg = false;
      return op2.operator-(op1);
    }
    else {
      op2.neg = false;
      return op1.operator-(op2);
    }
  }

  uint32_t big = *this > num ? dataSize + 1 : num.dataSize + 1;
  uint8_t * newData = new uint8_t[big];
  uint8_t ret = 0;
  Bignum result;
  for (int i = 0; i < big; i++) {
    if (dataSize > i && num.dataSize > i) {
      newData[i] = (data[i] + num.data[i] + ret) % BASE;
      ret = (data[i] + num.data[i] + ret) / BASE;
    }
    else if (dataSize > i) {
      newData[i] = (data[i] + ret) % BASE;
      ret = (data[i] + ret) / BASE;
    }
    else if (num.dataSize > i) {
      newData[i] = (num.data[i] + ret) % BASE;
      ret = (num.data[i] + ret) / BASE;
    }
    else {
      if (ret != 0) {
        newData[i] = ret;
        result = Bignum(newData, big, sign == -1);
        delete[] newData;
      }
      else {
        result = Bignum(newData, (big - 1), sign == -1);
        delete[] newData;
      }
    }
  }
  return result;
}

Bignum Bignum::operator+(int num)
{
  return this->operator+(Bignum(num));
}

Bignum operator+(int num, Bignum num2)
{
  return num2.operator+(Bignum(num));
}

Bignum Bignum::operator+() const {
  Bignum ans = *this;
  ans.neg = false;
  return ans;
}

Bignum Bignum::operator-(const Bignum & num)
{
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
    }
    else {
      op2.neg = false;
      return op1 + op2;
    }
  }

  if (*this <= num) {
    max = &num;
    min = this;
    invertSign = true;
  }
  else {
    max = this;
    min = &num;
    invertSign = false;
  }

  uint8_t * newData = new uint8_t[max->dataSize];
  uint8_t stole = 0;

  for (int i = 0; i < max->dataSize; i++) {
    if (max->dataSize > i && min->dataSize > i) {
      if (min->data[i] > max->data[i] - stole) {
        newData[i] = (max->data[i] + BASE) - min->data[i] - stole;
        stole = 1;
      }
      else {
        newData[i] = max->data[i] - min->data[i] - stole;
        stole = 0;
      }
    }
    else if (max->dataSize > i) {
      if (0 > max->data[i] - stole) {
        newData[i] = max->data[i] + BASE - stole;
        stole = 1;
      }
      else {
        newData[i] = max->data[i] - stole;
        stole = 0;
      }
    }
  }

  uint32_t size = chop(newData, max->dataSize);

  bool neg;
  if (sign == -1 && invertSign) {
    neg = false;
  }
  else if (sign == 1 && invertSign) {
    neg = true;
  }
  else if (sign == -1) {
    neg = true;
  }
  else {
    neg = false;
  }

  Bignum result = Bignum(newData, size, neg);
  delete[] newData;
  return result;
}

Bignum Bignum::operator-(int num)
{
  return this->operator-(Bignum(num));
}

Bignum operator-(int num, Bignum num2)
{
  return Bignum(num).operator-(num2);
}

Bignum Bignum::operator-() const {
  Bignum ans = *this;
  ans.neg = !ans.neg;
  return ans;
}

Bignum Bignum::operator*(const Bignum & num)
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

Bignum Bignum::operator*(int num)
{
  return this->operator*(Bignum(num));
}

Bignum operator*(int num, Bignum num2)
{
  return num2.operator*(num);
}

void Bignum::initDivision(const Bignum & num, Bignum & quotient, Bignum & denominator, int & index) {
  index = 1;
  denominator = +num;
  quotient = +(*this);

  while (denominator * BASE <= quotient) {
    index += 1;
    denominator = denominator * BASE;
  }
}

void Bignum::diviseStep(Bignum & reminder, Bignum & denominator, uint8_t * newData) {
  uint8_t diviser = 0;
  while (reminder >= (denominator * (diviser + 1))) {
    diviser++;
  }
  *newData = diviser;
  reminder = reminder - (denominator * (diviser));
  // divide by 100
  denominator = Bignum(denominator.data + 1, denominator.dataSize - 1, false);
}

Bignum Bignum::operator/(const Bignum & num)
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

Bignum Bignum::operator/(int num)
{
  return this->operator/(Bignum (num));
}

Bignum operator/(int num, Bignum num2)
{
  return Bignum(num) / num2;
}

Bignum Bignum::operator%(const Bignum & num) {
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