#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Computer.hpp"

static Rational zero = Rational("0");
static Rational one = Rational("1");


const char* Rational::NotANumberException::what() const throw()
{
    return "BAD";
}

const char* Rational::NumberTooLongException::what() const throw()
{
    return "BAD";
}

const char* Rational::ModuloByNonIntException::what() const throw()
{
    return "BAD";
}

const char* Rational::DivisionByZeroException::what() const throw()
{
    return "BAD";
}

const char* Rational::ModuloByZeroException::what() const throw()
{
    return "BAD";
}



Rational::Rational()
{
    Symbol::_type = VALUE;
    Symbol::_name = "rational";
    Symbol::_priority = 0;
    this->_numerator = Number("0");
    this->_denominator = Number("1");
}

Rational::Rational(Number const &numerator,Number const &denominator)
{
    Symbol::_type = VALUE;
    Symbol::_name = "rational";
    Symbol::_priority = 0;
    this->_numerator = numerator;
    this->_denominator = denominator;
}



Rational::Rational(string data) throw(NumberTooLongException, NotANumberException)
{
    Symbol::_type = VALUE;
    Symbol::_name = "rational";
    Symbol::_priority = 0;
    if (data.length() > NUMBER_LIMIT)
        throw NumberTooLongException();
    if (data[0] != '-' && (data[0] < '0' || data[0] > '9'))
        throw NotANumberException();
    int point = 0;
    int pointPos = -1;
    for (int idx = 0; idx < (int)data.length(); idx++)
        if (data[idx] == '.')
            point++;
    if (point > 1)
        throw NotANumberException();
    for (int idx = 0; idx < (int)data.length(); idx++)
        if (data[idx] == '.')
            pointPos = idx;
    if (pointPos == 0)
        throw NotANumberException(); 
    for (int idx = 1; idx < (int)data.length(); idx++)
        if (idx != pointPos && (data[idx] < '0' || data[idx] > '9'))
            throw NotANumberException();
    string tmp;
    for (int idx = 0; idx < (int)data.length(); idx++)
        if (idx != pointPos)
            tmp += data[idx];
    if (data.compare("-0") == 0)
    {
        this->_numerator = Number("0");
        this->_denominator = Number("1");
    }
    else
    {
        this->_numerator = tmp;
        if (pointPos != -1)
            this->_denominator = Number("1" + string(data.length() - 1 - pointPos, '0'));
        else
            this->_denominator = Number("1");
    }
}

Rational::Rational(Rational const &src)
{
    this->Symbol::_type = VALUE;
    Symbol::_name = "rational";
    Symbol::_priority = 0;
    *this = src;
}

Rational::~Rational()
{
}

bool    Rational::operator!=(Rational const &rhs)
{
    Rational tmp = Rational(rhs);
    return !(tmp == *this);
}
bool    Rational::operator==(Rational const &rhs)
{
    Rational tmp = Rational(rhs);
    return (this->_numerator * tmp._denominator == tmp._numerator * this->_denominator);
}
Rational &Rational::operator=(Rational const &rhs)
{
    this->Symbol::_type = rhs.Symbol::_type;
    this->_numerator = rhs._numerator;
    this->_denominator = rhs._denominator;
    return *this;
}

Rational    Rational::operator+(Rational const &rhs)
{
    Rational sum;
    sum._numerator = (this->_numerator * rhs._denominator) + (this->_denominator * rhs._numerator);
    sum._denominator = this->_denominator * rhs._denominator;
    return sum;
}

Rational    Rational::operator-(Rational const &rhs)
{
    Rational sum;
    sum._numerator = (this->_numerator * rhs._denominator) - (this->_denominator * rhs._numerator);
    sum._denominator = this->_denominator * rhs._denominator;
    return sum;
}
Complex    Rational::operator+(Complex const &rhs)
{
    return Complex(*this + rhs.getReal(), rhs.getImag());
}

Complex    Rational::operator-(Complex const &rhs)
{
    return Complex(*this - rhs.getReal(), Rational("-1") * rhs.getImag());
}

Complex    Rational::operator*(Complex const &rhs)
{
    return Complex(Complex(*this, Rational("0")) * rhs);
}

Complex    Rational::operator/(Complex const &rhs)
{
    return Complex(Complex(*this, Rational("0")) / rhs);
}

Matrix     Rational::operator*(Matrix const &rhs)
{
    Matrix res = Matrix(rhs);
    res.multiplyBy(*this);
    return res;
}

Number  Rational::getNumerator() const
{
    return this->_numerator;
}
Number  Rational::getDenominator() const
{
    return this->_denominator;
}


Rational Rational::operator*(Rational const &rhs)
{
    Rational sum;
    sum._numerator = this->_numerator * rhs._numerator;
    sum._denominator = this->_denominator * rhs._denominator;
    return sum;
}

bool    Rational::isNumber() const
{
    return gcd(this->_denominator, this->_numerator) == this->_denominator ;
}

Rational Rational::operator/(Rational const &rhs) throw(DivisionByZeroException)
{
    if (Number("0") == rhs._numerator)
        throw DivisionByZeroException();
    Rational sum;
    sum._numerator = this->_numerator * rhs._denominator;
    sum._denominator = this->_denominator * rhs._numerator;
    return sum;
}

Rational Rational::operator%(Rational const &rhs) throw(ModuloByNonIntException, ModuloByZeroException)
{
    if (Number("0") == rhs._numerator)
        throw ModuloByZeroException();
    if (!(this->_denominator == Number("1")) || !(Number("1") == rhs._denominator))
        throw ModuloByNonIntException();
    Rational sum = Rational();
    sum._numerator = this->_numerator % rhs._numerator;
    sum._denominator = Number("1");
    return sum;
}

Rational Rational::operator^(Number power)
{
    Rational pow;
    pow._numerator = this->_numerator;
    pow._denominator = this->_denominator;
    if (Number("0") > power)
    {
        swap(pow._numerator, pow._denominator);
        power = power * Number("-1");
    }
    pow._numerator = pow._numerator ^ power;
    pow._denominator = pow._denominator ^ power;
    return pow;
}
string Rational::approximation() const
{
    string output;
    string answer;
    Number a = this->_numerator;
    Number b = this->_denominator;
    if (a < Number("0") && b < Number("0"))
    {
        a = a * Number("-1");
        b = b * Number("-1");
    }
    bool isNeg = (a < Number("0") || b < Number("0"));
    if (a < Number("0"))
        a = a * Number("-1");
    if (b < Number("0"))
        b = b * Number("-1");
    int iter = 0;
    Number res = a / b;
    a = a - res * b; 
    map<string,int> mp;
    a = a * Number("10");
    bool hasDecimal = false;
    while (iter < 10)
    {
        Number quotient = Number("9");
        while (quotient > Number("0") && quotient * b > a)
            quotient = quotient - Number("1");
        a = a - quotient * b;
        if (quotient.getValue().compare("0") != 0)
            hasDecimal = true;
        output += quotient.getValue();
        a = a * Number("10");
        iter++;
    }
    if (isNeg)
        answer += '-';
    for (int idx = 0; idx < (int)res.getValue().length(); idx++)
        answer += res.getValue()[idx];
    if (hasDecimal)
    {
        answer += '.';
        answer += output;
    }
    return answer;
}


pair<wstring,int>  Rational::getApproximation() const
{
    string output;
    wstring withRep;
    Number a = this->_numerator;
    Number b = this->_denominator;
    if (a < Number("0") && b < Number("0"))
    {
        a = a * Number("-1");
        b = b * Number("-1");
    }
    bool isNeg = (a < Number("0") || b < Number("0"));
    if (a < Number("0"))
        a = a * Number("-1");
    if (b < Number("0"))
        b = b * Number("-1");
    int iter = 0;
    Number res = a / b;
    a = a - (res * b); 
    map<string,int> mp;
    a = a * Number("10");
    int rep = 1e9;
    while (a > Number("0") && iter < 100)
    {
        Number quotient = Number("9");
        while (quotient > Number("0") && quotient * b > a)
            quotient = quotient - Number("1");
        if (mp.count(a.getValue()))
        {
            rep = mp[a.getValue()];
            break;
        }
        mp[a.getValue()] = iter;
        a = a - (quotient * b);
        output += quotient.getValue();
        a = a * Number("10");
        iter++;
    }
    if (isNeg)
        withRep += '-';
    for (int idx = 0; idx < (int)res.getValue().length(); idx++)
        withRep += res.getValue()[idx];
    if (output.length())
        withRep += '.';
    for (int idx = 0; idx < (int)output.length(); idx++)
    {
        withRep += output[idx];
        if (idx >= rep)
            withRep += Computer::init().getWithRep();
    }
    if (a > Number("0") && rep == 1e9)
        withRep += L"...";
    return {withRep, res.getValue().length() + 1 + output.length()};
}

Rational  Rational::round() const
{
    int lenNum = this->getNumerator().getValue().length();
    int lenDenom = this->getDenominator().getValue().length();
    int remove = max(0,min(lenDenom-7, lenNum-7));
    return Rational(Number(this->getNumerator().getValue().substr(0,lenNum-remove)), Number(this->getDenominator().getValue().substr(0,lenDenom-remove)));
}

Rational    Rational::abs() const
{
    Rational pos;
    pos._numerator = (this->getNumerator() < Number("0") ? this->getNumerator() * Number("-1") : this->getNumerator());
    pos._denominator = (this->getDenominator() < Number("0") ? this->getDenominator() * Number("-1") : this->getDenominator());
    return pos;
}

Complex Rational::getRoot() const
{
    if (Rational("0") == *this)
        return Complex(Rational("0"),Rational("0"));
    bool isComplex = false ^ (this->getNumerator() < Number("0")) ^ (this->getDenominator() < Number("0"));
    string ans = string(this->_numerator.getValue().length(),'0') + "." + string(10,'0');
    bool decimal = false;
    Rational myVal = *this;
    if (isComplex)
    {
        if (myVal.getNumerator() < Number("0"))
            myVal._numerator = myVal._numerator*Number("-1");
        if (myVal.getDenominator() < Number("0"))
            myVal._denominator = myVal._denominator*Number("-1");
    }
    for (int idx = 0; idx < (int)ans.length(); idx++)
    {
        if (ans[idx] == '.')
        {
            decimal =true;
            continue;
        }
        ans[idx] = '9';
        while (ans[idx] > '0')
        {
            Rational root = Rational(ans);
            if (root*root <= myVal)
                break;
            ans[idx]--;
        }
        if (ans[idx] != '0')
            decimal =true;
        if (ans[idx] == '0' && !decimal)
        {
            ans = ans.substr(1);
            idx--;
        }
    }
    if (isComplex)
        return Complex(Rational("0"),Rational(ans));
    else
        return Complex(Rational(ans), Rational("0"));
}

bool Rational::operator>(Rational const &rhs)
{
    return this->_numerator*rhs._denominator > this->_denominator*rhs._numerator;
}

bool Rational::operator>=(Rational const &rhs)
{
    return (*this > rhs) || (*this == rhs);
}

bool Rational::operator<(Rational const &rhs)
{
    return this->_numerator*rhs._denominator < this->_denominator*rhs._numerator;
}

bool Rational::operator<=(Rational const &rhs)
{
    return (*this < rhs) || (*this == rhs);
}

ostream &operator<<(ostream &o, Rational const &rhs)
{
    wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    o << convert.to_bytes(rhs.getApproximation().first);
    return o;
}