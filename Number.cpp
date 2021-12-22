#include "Number.hpp"

Number one = Number("1");
Number zero = Number("0");
const char* Number::NotANumberException::what() const throw()
{
    return "BAD";
}

const char* Number::NumberTooLongException::what() const throw()
{
    return "BAD";
}

const char* Number::DivisionByZeroException::what() const throw()
{
    return "BAD";
}

const char* Number::ModuloByZeroException::what() const throw()
{
    return "BAD";
}

const char* Number::NegativePowerException::what() const throw()
{
    return "BAD";
}

Number::Number() : _value("0")
{
    Symbol::_type = VALUE;
}

Number::~Number()
{
}

Number::Number(string const &data) throw(NotANumberException, NumberTooLongException)
{
    Symbol::_type = VALUE;
    if (data.length() > NUMBER_LIMIT)
        throw NumberTooLongException();
    if (data[0] != '-' && (data[0] < '0' || data[0] > '9'))
            throw NotANumberException();
    for (int idx = 1; idx < (int)data.length(); idx++)
        if (data[idx] < '0' || data[idx] > '9')
            throw NotANumberException();
    if (data.compare("-0") == 0)
        this->_value = "0";
    else
    {
        int idx = 0;
        while(idx < (int)data.length()-1 && data[idx] == '0')
            idx++;
        this->_value = data.substr(idx);
    }
}

Number Number::operator=(Number const &rhs)
{
    this->_value = rhs._value;
    return *this;
}

bool Number::operator!=(Number const &rhs)
{
    return this->_value.compare(rhs._value) != 0;
}
bool Number::operator==(Number const &rhs)
{
    if (this->_value.compare("0") == 0 && rhs._value.compare("-0") == 0)
        return true;
    if (this->_value.compare("-0") == 0 && rhs._value.compare("0") == 0)
        return true;
    return this->_value.compare(rhs._value) == 0;
}

Number Number::operator+(Number const &rhs)
{
    if (this->_value[0] == '-' && rhs._value[0] == '-')
        return Number("-1") * (Number(this->_value.substr(1,this->_value.length())) + 
                                Number(rhs._value.substr(1,rhs._value.length())));
    else if (this->_value[0] == '-')
        return Number(rhs) - Number(this->_value.substr(1,this->_value.length()));
    else if (rhs._value[0] == '-')
        return *this - Number(rhs._value.substr(1,rhs._value.length()));
    else
    {
        int len = max(this->_value.length(), rhs._value.length());
        string sum(len+1,'0');
        int i = len;
        int j = this->_value.length() - 1;
        int k = rhs._value.length() - 1;
        int got = 0;
        for (int idx = i; idx >= 0; idx--,j--,k--)
        {
            if (j >= 0)
                got += this->_value[j] - '0';
            if (k >= 0)
                got += rhs._value[k] - '0';
            sum[idx] = (char)('0' + got%10); 
            got /= 10;
        }
        if (sum[0] == '0')
            sum = sum.substr(1, sum.length());
        return Number(sum);
    }   
}

bool Number::operator>=(Number const &rhs)
{
    if (this->_value[0] == '-' && rhs._value[0] != '-')
        return false;
    if (this->_value[0] != '-' && rhs._value[0] == '-')
        return true;
    if (this->_value.length() > rhs._value.length())
        return true;
    if (this->_value.length() == rhs._value.length())
    {
        int idx = 0;
        while(idx < (int)this->_value.length() && this->_value[idx] == rhs._value[idx])
            idx++;
        return (idx == (int)this->_value.length() || this->_value[idx] >= rhs._value[idx]);
    }
    return false;
}

bool Number::operator<=(Number const &rhs)
{
    Number tmp = Number(rhs);
    return tmp >= *this;
}

bool Number::operator<(Number const &rhs)
{
    Number tmp = Number(rhs);
    return tmp > *this; 
}

bool Number::operator>(Number const &rhs)
{
    if (this->_value[0] == '-' && rhs._value[0] != '-')
        return false;
    if (this->_value[0] != '-' && rhs._value[0] == '-')
        return true;
    if (this->_value.length() > rhs._value.length())
        return true;
    if (this->_value.length() == rhs._value.length())
    {
        int idx = 0;
        while(idx < (int)this->_value.length() && idx < (int)rhs._value.length() && this->_value[idx] == rhs._value[idx])
            idx++;
        if (idx == (int)this->_value.length())
            return false;
        if (idx == (int)rhs._value.length())
            return true;
        return (this->_value[idx] > rhs._value[idx]);
    }
    return false;
}



Number Number::operator-(Number const &rhs)
{
    if (this->_value[0] == '-' && rhs._value[0] == '-')
        return Number(rhs._value.substr(1,rhs._value.length())) + *this;
    else if (this->_value[0] == '-')
        return Number("-1")* (Number(rhs) + Number(this->_value.substr(1, this->_value.length())));
    else if (rhs._value[0] == '-')
    {
        return (*this + Number(rhs._value.substr(1, rhs._value.length())));
    }
    else
    {
        if (*this >= rhs)
        {
            int len = max(this->_value.length(), rhs._value.length());
            string diff(len, '0');
            int i = len - 1;
            int j = this->_value.length() - 1;
            int k = rhs._value.length() - 1;
            int got = 0;
            int carry = 0;
            for (int idx = i; idx >= 0; idx--, j--, k--)
            {
                if (k >= 0)
                {
                    if (this->_value[j] < rhs._value[k] + carry)
                    {
                        got = 10 + this->_value[j] - rhs._value[k] - carry;
                        carry = 1;
                    }
                    else
                    {
                        got = this->_value[j] - rhs._value[k] - carry;
                        carry = 0;
                    }
                }
                else
                {
                    got = this->_value[j] - '0' - carry;
                    carry = 0;
                    if (got < 0)
                    {
                        got += 10;
                        carry =1;
                    }
                }
                diff[idx] = (char)(got + '0');
            }
            trim(diff);
            return Number(diff);
        }
        else
            return Number("-1") * (Number(rhs) - *this);
    }
}

Number karatsuba(Number a, Number b)
{
    if (a.getValue().length() == 1)
    {
        long long int got = 0;
        string digitProduct(b.getValue().length() + 1, '0');
        int pos = digitProduct.length() - 1;
        for (int j = b.getValue().length() - 1; j >= 0; j--)
        {
            got += (a.getValue()[0] - '0') * (b.getValue()[j] - '0');
            digitProduct[pos] = (char)(got % 10 + '0');
            got /= 10;
            pos--;
        }
        if (got != 0)
            digitProduct[pos] = (char)(got + '0');
        trim(digitProduct);
        return Number(digitProduct);
    }
    else if (b.getValue().length() == 1)
    {
        long long int got = 0;
        string digitProduct(a.getValue().length() + 1, '0');
        int pos = digitProduct.length() - 1;
        for (int j = a.getValue().length() - 1; j >= 0; j--)
        {
            got += (b.getValue()[0] - '0') * (a.getValue()[j] - '0');
            digitProduct[pos] = (char)(got % 10 + '0');
            got /= 10;
            pos--;
        }
        if (got != 0)
            digitProduct[pos] = (char)(got + '0');
        trim(digitProduct);
        return Number(digitProduct);
    }
    else
    {
        int minLen = min(a.getValue().length(), b.getValue().length());
        int split = minLen/2;
        string zeros1 = string(2*split, '0');
        string zeros2 = string(split, '0');
        string a1,a2,b1,b2;
        a1 = a.getValue().substr(0, a.getValue().length() - split);
        a2 = a.getValue().substr(a.getValue().length() - split);
        b1 = b.getValue().substr(0, b.getValue().length() - split);
        b2 = b.getValue().substr(b.getValue().length() - split);
        Number z0, z1, z2;
        z0 = karatsuba(Number(a2), Number(b2));
        z1 = karatsuba(Number(a2) + Number(a1), Number(b2) + Number(b1));
        z2 = karatsuba(Number(a1), Number(b1));
        Number diff = z1-z2-z0;
        assert((diff +z0+z2) == z1);
        Number result = Number(Number(z2.getValue()+zeros1) + Number((z1-z2-z0).getValue() + zeros2) + z0);
        string tmp = result.getValue();
        trim(tmp);
        result = Number(tmp);
        return result;
    }
}

Number Number::operator*(Number const &rhs)
{
    if (this->_value[0] == '-' && rhs._value[0] == '-')
        return Number(this->_value.substr(1, this->_value.length())) *
               Number(rhs._value.substr(1, rhs._value.length()));
    else if (this->_value[0] == '-')
        return Number("-" + (Number(this->_value.substr(1, this->_value.length())) * rhs)._value);
    else if (rhs._value[0] == '-')
        return Number("-" + (Number(rhs._value.substr(1, rhs._value.length())) * (*this))._value);
    else
        return karatsuba(*this, rhs);
}

Number Number::operator/(Number const &rhs) throw(DivisionByZeroException)
{
    if (rhs._value == "0")
        throw DivisionByZeroException();
    if (this->_value[0] == '-' && rhs._value[0] == '-')
        return Number(this->_value.substr(1, this->_value.length())) / Number(rhs._value.substr(1, rhs._value.length()));
    else if (this->_value[0] == '-')
        return Number("-1") * (Number(this->_value.substr(1, this->_value.length())) / rhs);
    else if (rhs._value[0] == '-')
        return Number("-1") * (*this / Number(rhs._value.substr(1, rhs._value.length())));
    else
    {
        if (rhs._value == "2")
        {
            Number tmp = *this * Number("5");
            if (tmp._value.length() == 1)
                return Number("0");
            return Number(tmp._value.substr(0,tmp._value.length() -1));
        }
        else
        {
            Number low("0");
            Number hi = *this;
            while (hi >= low)
            {
                Number tmp = (low + hi) * Number("5");
                Number mid = (tmp._value.length() == 1 ? Number("0") : Number(tmp._value.substr(0, tmp._value.length() - 1)));
                if (mid * rhs > *this)
                    hi = mid - Number("1");
                else
                    low = mid + Number("1");
            }
            return Number(hi);
        }
    }
}

Number Number::operator%(Number const &rhs) throw(ModuloByZeroException)
{
    if (Number(rhs) == Number("0"))
        throw ModuloByZeroException();
    return *this - (*this / rhs) * rhs;
}

Number Number::operator^(Number power) throw(NegativePowerException)
{
    if (Number("0") > power)
        throw NegativePowerException();
    Number res("1");
    Number tmp(*this);
    while (power > Number("0"))
    {
        if ((power._value[power._value.length() - 1] - '0') % 2 == 1)
            res = res * tmp;
        tmp = tmp * tmp;
        power = power / Number("2");
    }
    return Number(res);
}

Number gcd(Number a, Number b)
{
    return b == Number("0") ? a : gcd(b, a % b);
}

string Number::getValue() const
{
    return this->_value;
}

ostream& operator<<(ostream &o, Number const &rhs)
{
    o << rhs.getValue();
    return o;
}