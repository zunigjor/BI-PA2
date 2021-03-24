#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CBigInt
{
  public:
    CBigInt();
    CBigInt(const int);
    CBigInt(const char *);
    CBigInt operator = (const CBigInt &);
    friend CBigInt operator + (const CBigInt &, const CBigInt &);
    friend CBigInt operator * (const CBigInt &, const CBigInt &);
    friend void operator += (CBigInt &, const CBigInt &);
    friend void operator *= (CBigInt &, const CBigInt &);
    friend bool operator == (const CBigInt &, const CBigInt &);
    friend bool operator != (const CBigInt &, const CBigInt &);
    friend bool operator >  (const CBigInt &, const CBigInt &);
    friend bool operator >= (const CBigInt &, const CBigInt &);
    friend bool operator <  (const CBigInt &, const CBigInt &);
    friend bool operator <= (const CBigInt &, const CBigInt &);
    friend ostream  & operator << (ostream &, const CBigInt &);
    friend istream & operator >> (istream &, CBigInt &);
  private:
    CBigInt addArrays(const CBigInt & x) const;
    CBigInt subArrays(const CBigInt & bigger, const CBigInt & smaller) const;
    vector<uint8_t> m_number; // array of digits 0...9 // uint8_t is big enough for that
    const int m_base10maxDigit = 9;
    bool m_sign = false; // 0 = +  // 1 = -
};
// Default constructor
CBigInt::CBigInt():m_sign(false) {
    m_number.push_back(0);
}
// From int constructor
CBigInt::CBigInt(const int x) {
    if (x == 0){
        m_number.push_back(0);
    } else {
        m_sign = x < 0;
        int n = (x < 0) ? -x : x;
        while (n){
            m_number.push_back(n % 10);
            n /= 10;
        }
    }
}
// From string constructor
CBigInt::CBigInt(const char * x) {
    string numStr(x);
    char first = numStr[0];
    if (first != '-' && first != '+'){
        m_sign = false;
        for (char i : numStr) {
            if (!isdigit(i))
                throw invalid_argument("Nespravny vstup.");
        }
    } else {
        numStr.erase(numStr.begin());
        m_sign = (first == '-');
        for (char i : numStr) {
            if (!isdigit(i)){
                throw invalid_argument("Nespravny vstup.");
            }
        }
    }
    numStr.erase(0, min(numStr.find_first_not_of('0'), numStr.size() - 1));
    for(auto it = numStr.rbegin(); it != numStr.rend(); it++){
        m_number.push_back((*it) - '0');
    }
}
// Copy operator
CBigInt CBigInt::operator = (const CBigInt & x) {
    this->m_sign = x.m_sign;
    this->m_number = x.m_number;
    return *this;
}

CBigInt CBigInt::addArrays(const CBigInt & x) const{ // todo make this better with 0 padding
    CBigInt result;
    result.m_sign = m_sign;
    result.m_number.pop_back();
    auto it_L = m_number.begin();
    auto it_R = x.m_number.begin();
    bool carry = false;
    bool l_at_end = it_L != m_number.end();
    bool r_at_end = it_R != x.m_number.end();
    while ( l_at_end || r_at_end ){
        int res;
        if (it_L != m_number.end() && it_R != x.m_number.end()){
            res = (*it_L) + (*it_R) + carry;
            carry = (res > m_base10maxDigit);
            res %= 10;
            result.m_number.push_back(res);
            it_L++;
            it_R++;
        } else if (it_L == m_number.end() && it_R != x.m_number.end()){
            res = (*it_R) + carry;
            carry = (res > m_base10maxDigit);
            res %= 10;
            result.m_number.push_back(res);
            it_R++;
        } else if (it_L != m_number.end() && it_R == x.m_number.end()){
            res = (*it_L) + carry;
            carry = (res > m_base10maxDigit);
            res %= 10;
            result.m_number.push_back(res);
            it_L++;
        }
        l_at_end = it_L != m_number.end();
        r_at_end = it_R != x.m_number.end();
    }
    if (carry) result.m_number.push_back(carry);
    return result;
}

CBigInt CBigInt::subArrays(const CBigInt & bigger, const CBigInt & smaller) const {
    CBigInt result;
    result.m_number.pop_back();
    CBigInt zeroPadded = smaller;
    while (zeroPadded.m_number.size() != bigger.m_number.size()){
        zeroPadded.m_number.push_back(0);
    }
    auto it_B = bigger.m_number.begin();
    auto it_S = zeroPadded.m_number.begin();
    bool borrow = false;
    while (it_B != bigger.m_number.end()){
        int res = (*it_B) - (*it_S) - borrow;
        if (res < 0){
            borrow = true;
            res += 10;
        } else {
            borrow = false;
        }
        it_B++;
        it_S++;
        result.m_number.push_back(res);
    }
    auto it = result.m_number.rbegin();
    while (result.m_number.size() > 1){
        if ((*it) != 0)
            break;
        if ((*it) == 0)
            result.m_number.pop_back();
        it++;
    }
    return result;
}

CBigInt operator + (const CBigInt & lhs, const CBigInt & rhs){
    if (rhs == 0){
        return lhs;
    } else if (lhs == 0){
        return rhs;
    }
    if ((lhs.m_sign == 0 && rhs.m_sign == 0) || (lhs.m_sign == 1 && rhs.m_sign == 1)){ //// +10 + +10 = 20 // -10 + -10 == -20
        return lhs.addArrays(rhs);
    } else {
        CBigInt absRhs = rhs;
        absRhs.m_sign = 0;
        CBigInt absLhs = lhs;
        absLhs.m_sign = 0;
        if (absRhs == absLhs){
            return CBigInt(0);
        } else if (absRhs > absLhs){ //// LHS/+10/ + RHS/-20/ = res/-10/ // LHS/-10/ + RHS/+20/ = res/+10/ |rhs| > |lhs|
            CBigInt result = rhs.subArrays(rhs,lhs);
            result.m_sign = rhs.m_sign;
            return result;
        } else { //// LHS/+10/ + /-5/ = +5  // -10 + +5 = -5
            CBigInt result = lhs.subArrays(lhs,rhs);
            result.m_sign = lhs.m_sign;
            return result;
        }
    }
}

CBigInt operator * (const CBigInt & lhs, const CBigInt & rhs){
    if (lhs == 0 || rhs == 0){
        return CBigInt(0);
    } else if (lhs == 1){
            return rhs;
    } else if (rhs == 1){
            return lhs;
    } else {
        size_t leftSize = lhs.m_number.size();
        size_t rightSize = rhs.m_number.size();
        size_t resultSize =  leftSize + rightSize;
        CBigInt result;
        result.m_sign = (rhs.m_sign != lhs.m_sign);
        result.m_number = vector<uint8_t>(resultSize, 0);
        int carry;
        for (size_t it_R = 0; it_R < rightSize; ++it_R){
            carry = 0;
            for (size_t it_L = 0; it_L < leftSize; ++it_L) {
                result.m_number[it_L + it_R] += carry + lhs.m_number[it_L] * rhs.m_number[it_R];
                carry = result.m_number[it_L + it_R] / 10;
                result.m_number[it_L + it_R] = result.m_number[it_L + it_R] % 10;
            }
            result.m_number[it_R + leftSize] = carry;
        }
        auto it = result.m_number.rbegin();
        while (result.m_number.size() > 1){
            if (*it != 0)
                break;
            if(*it == 0)
                result.m_number.pop_back();
            it++;
        }
        return result;
    }
}

void operator += (CBigInt & lhs, const CBigInt & rhs){
    lhs = (lhs + rhs);
}

void operator *= (CBigInt & lhs, const CBigInt & rhs){
    lhs = (lhs * rhs);
}

bool operator == (const CBigInt & lhs, const CBigInt & rhs){
    if (lhs.m_number.size() == 1 && rhs.m_number.size() == 1 && lhs.m_number[0] == 0 && rhs.m_number[0] == 0)
        return true;
    return (lhs.m_sign == rhs.m_sign && lhs.m_number == rhs.m_number);
}

bool operator != (const CBigInt & lhs, const CBigInt & rhs){
    if (lhs.m_number.size() == 1 && rhs.m_number.size() == 1 && lhs.m_number[0] == 0 && rhs.m_number[0] == 0)
        return false;
    return !(lhs.m_sign == rhs.m_sign && lhs.m_number == rhs.m_number);
}

bool operator > (const CBigInt & lhs, const CBigInt & rhs){
    if (lhs.m_number.size() == 1 && rhs.m_number.size() == 1 && lhs.m_number[0] == 0 && rhs.m_number[0] == 0)
        return false;
    if (lhs.m_sign == 0 && rhs.m_sign == 1)
        return true;
    if (lhs.m_sign == 1 && rhs.m_sign == 0)
        return false;
    if (lhs.m_sign == rhs.m_sign){
        if (lhs.m_number.size() > rhs.m_number.size()){
            return true;
        } else if (lhs.m_number.size() < rhs.m_number.size()){
            return false;
        } else if (lhs.m_number.size() == rhs.m_number.size()){
            auto it_L = lhs.m_number.rbegin();
            auto it_R = rhs.m_number.rbegin();
            while (it_L != lhs.m_number.rend()){
                if ((*it_L) == (*it_R)){
                    it_L++; it_R++;
                } else return (*it_L) > (*it_R);
            }
            return false;
        }
    }
    return false;
}

bool operator >=(const CBigInt & lhs, const CBigInt & rhs){
    return ((lhs > rhs) || (lhs == rhs));
}

bool operator < (const CBigInt & lhs, const CBigInt & rhs){
    if (lhs.m_number.size() == 1 && rhs.m_number.size() == 1 && lhs.m_number[0] == 0 && rhs.m_number[0] == 0)
        return false;
    if (lhs.m_sign == 0 && rhs.m_sign == 1)
        return false;
    if (lhs.m_sign == 1 && rhs.m_sign == 0)
        return true;
    if (lhs.m_sign == rhs.m_sign){
        if (lhs.m_number.size() > rhs.m_number.size()){
            return false;
        } else if (lhs.m_number.size() < rhs.m_number.size()){
            return true;
        } else if (lhs.m_number.size() == rhs.m_number.size()){
            auto it_L = lhs.m_number.rbegin();
            auto it_R = rhs.m_number.rbegin();
            while (it_L != lhs.m_number.rend()){
                if ((*it_L) == (*it_R)){
                    it_L++; it_R++;
                } else return (*it_L) < (*it_R);
            }
            return false;
        }
    }
    return false;
}

bool operator <=(const CBigInt & lhs, const CBigInt & rhs){
    return ((lhs < rhs) || (lhs == rhs));
}

ostream &operator<<(ostream & os, const CBigInt & x){
    if (x.m_number.size() == 1 && x.m_number[0] == 0){os << 0;return os;}
    os << ((!x.m_sign) ? "" : "-");
    for (auto it = x.m_number.rbegin(); it != x.m_number.rend() ; ++it) {
        os << (int)(*it);
    }
    return os;
}

istream &operator>>(istream & is, CBigInt & x){
    string s1;
    is >> s1;
    if ( !isdigit(s1[0]) ){    //// abcd || -abcd || -2131
        if (s1[0] != '-'){     /// abcd
            is.setstate(ios::failbit);
            return is;
        } else {
            if (!isdigit(s1[1])){ //// -abcd
                is.setstate(ios::failbit);
                return is;
            }
        }
    }
    size_t i;
    bool signFlag = false;
    for (i = 0; i < s1.size(); ++i) {
        if (i==0 && s1[0] == '-'){
            signFlag = true;
            continue;
        }
        if (i>0 && !isdigit(s1[i]))
            break;
    }
    string s2;
    for (size_t j = 0; j < i ; ++j) {
        s2.push_back(s1[j]);
    }
    x = CBigInt(s2.c_str());
    x.m_sign = signFlag;
    return is;
}

#ifndef __PROGTEST__
static bool equal ( const CBigInt & x, const char * val )
{
  ostringstream oss;
  oss << x;
  return oss . str () == val;
}
int main ( void )
{
    CBigInt a;
    CBigInt b;
    istringstream is;

    a = 10;
    a += -20;
    assert ( equal ( a, "-10" ) );
    a *= 5;
    assert ( equal ( a, "-50" ) );
    b = a + 73;
    assert ( equal ( b, "23" ) );
    b = a * -7;
    assert ( equal ( b, "350" ) );
    assert ( equal ( a, "-50" ) );

    a = 10;
    a += 20;
    assert ( equal ( a, "30" ) );
    a *= 5;
    assert ( equal ( a, "150" ) );
    b = a + 3;
    assert ( equal ( b, "153" ) );
    b = a * 7;
    assert ( equal ( b, "1050" ) );
    assert ( equal ( a, "150" ) );



    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert ( equal ( a, "-87654321098765432109" ) );
    a *= "54321987654321987654";
    assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
    a *= 0;
    assert ( equal ( a, "0" ) );
    a = 10;
    b = a + "400";
    assert ( equal ( b, "410" ) );
    b = a * "15";
    assert ( equal ( b, "150" ) );
    assert ( equal ( a, "10" ) );


    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert ( equal ( b, "1234" ) );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert ( equal ( b, "12" ) );
    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert ( equal ( b, "999" ) );
    is . clear ();
    is . str ( "abcd" );
    assert ( ! ( is >> b ) );
    is . clear ();
    is . str ( "- 758" );
    assert ( ! ( is >> b ) );
    a = 42;
    try
    {
      a = "-xyz";
      assert ( "missing an exception" == NULL );
    }
    catch ( const invalid_argument & e )
    {
      assert ( equal ( a, "42" ) );
    }

    a = "73786976294838206464";
    assert ( a < "1361129467683753853853498429727072845824" );
    assert ( a <= "1361129467683753853853498429727072845824" );
    assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
    assert ( a != "1361129467683753853853498429727072845824" );
    assert ( ! ( a < "73786976294838206464" ) );
    assert ( a <= "73786976294838206464" );
    assert ( ! ( a > "73786976294838206464" ) );
    assert ( a >= "73786976294838206464" );
    assert ( a == "73786976294838206464" );
    assert ( ! ( a != "73786976294838206464" ) );
    assert ( a < "73786976294838206465" );
    assert ( a <= "73786976294838206465" );
    assert ( ! ( a > "73786976294838206465" ) );
    assert ( ! ( a >= "73786976294838206465" ) );
    assert ( ! ( a == "73786976294838206465" ) );
    assert ( a != "73786976294838206465" );
    a = "2147483648";
    assert ( ! ( a < -2147483648 ) );
    assert ( ! ( a <= -2147483648 ) );
    assert ( a > -2147483648 );
    assert ( a >= -2147483648 );
    assert ( ! ( a == -2147483648 ) );
    assert ( a != -2147483648 );

    return 0;
}
#endif /* __PROGTEST__ */