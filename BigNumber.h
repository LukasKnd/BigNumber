#ifndef BIGNUMBER_H_INCLUDED
#define BIGNUMBER_H_INCLUDED

#include <iostream>
#include <vector>
#include <assert.h>
#include <regex>
#include <random>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <map>
#include <cmath>

using namespace std;

class BigNumber {

    friend BigNumber bPrime(BigNumber n);
    friend BigNumber bLog(double d);
    friend BigNumber bSin(double d);

    vector<int> nums;
    int sign;
    int scale;
public:
    BigNumber();

    BigNumber(const std::string &num);

    BigNumber(long num);

    string toString();

    /// returns 1 if this is bigger -1 if smaller 0 if equal
    int compareTo (const BigNumber &rhs);

    BigNumber add(const BigNumber &rhs);

    BigNumber subtract(const BigNumber &rhs);

    BigNumber multiply(const BigNumber &rhs, int rounding=105);

    BigNumber ceil();

    BigNumber times10(int times);

    BigNumber mod(const BigNumber &rhs);

    BigNumber divide(const BigNumber &rhs, int maxScale=105);

    BigNumber power(int pow);

    BigNumber round(int scale);

    BigNumber abs(const BigNumber &rhs);

private:
    void process_string (const std::string &s);

    void removeLeadingZero();

    int getLarger(const vector<int> &a, const vector<int> &b, const int &scaleA, const int &scaleB);

    void sub_pos(const BigNumber &larger, const BigNumber &smaller, BigNumber &res);

    void sum_pos(const BigNumber &larger, const BigNumber &smaller, BigNumber &res);

    void truncZero();

};

bool isprime(BigNumber n);
string convertDoubleToString(double num);

/// returns pi with maximum accuracy of 99
/// n - accuracy
BigNumber bPi(int n);

/// return square root of a number with accuracy of 99 digits
BigNumber bSqrt(double d);

/// returns sin value with accuracy of 99 digits
BigNumber bSin(double d);

/// returns natural logarithm with accuracy of 99 digits
BigNumber bLog(double d);

///returns power of a number with accuracy of 99 digits
///d - number
///p - power
BigNumber bPow(double d, int p);

/// finds the next prime after n
BigNumber bPrime(BigNumber n);

/// returns factorial of a number
BigNumber bFactorial(int n);


#endif // BIGNUMBER_H_INCLUDED
