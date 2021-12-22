#include "Complex.hpp"
#include "Computer.hpp"


const char* Complex::NegativePowerException::what() const throw()
{
    return "BAD";
}
Complex::Complex()
{
    this->_type = COMPLEX;
    this->_real = Rational("0");
    this->_imag = Rational("0");
}

Complex::Complex(Rational real, Rational imag) : _real(real), _imag(imag)
{
    this->_type = COMPLEX;
}

Complex::Complex(Complex const &src)
{
    *this = src;
}

Complex::~Complex()
{
}

bool  Complex::operator==(Complex const &rhs)
{
    if (this->_real != rhs._real)
        return false;
    if (this->_imag != rhs._imag)
        return false;
    return true;
}
Complex& Complex::operator=(Complex const &rhs)
{
    this->Symbol::_type = rhs.Symbol::_type;
    this->_real = rhs._real;
    this->_imag = rhs._imag;
    return *this;
}

Complex Complex::operator+(Complex const &rhs)
{
    return Complex(this->_real + rhs._real, this->_imag + rhs._imag);
}

Complex Complex::operator-(Complex const &rhs)
{
    return Complex(this->_real - rhs._real, this->_imag - rhs._imag);
}

Complex Complex::operator*(Complex const &rhs)
{
    return Complex(this->_real * rhs._real - this->_imag * rhs._imag,
                        this->_real * rhs._imag + this->_imag * rhs._real);
}

Complex Complex::operator/(Complex const &rhs)
{
    Rational norm(Rational(rhs._real) * Rational(rhs._real) + Rational(rhs._imag) * Rational(rhs._imag));
    Complex result;
    Complex conjugate(rhs._real, Rational(rhs._imag) * Rational("-1"));
    result = (*this) * conjugate;
    result = result / norm;
    return result;
}

Complex Complex::operator+(Rational const &rhs)
{
    return Complex(this->_real + rhs, this->_imag);
}

Complex Complex::operator-(Rational const &rhs)
{
    return Complex(this->_real - rhs, this->_imag);
}

Complex Complex::operator*(Rational const &rhs)
{
    return Complex(this->_real * rhs, this->_imag * rhs);
}

Complex Complex::operator/(Rational const &rhs)
{
    return Complex(this->_real / rhs, this->_imag / rhs);
}

Complex Complex::operator^(Number power)
{
    if (Number("0") > power)
        throw NegativePowerException();
    Complex res(Rational("1"), Rational("0"));
    Complex tmp(*this);
    while (power > Number("0"))
    {
        if ((power.getValue()[power.getValue().length() - 1] - '0') % 2 == 1)
            res = res * tmp;
        tmp = tmp * tmp;
        power = power / Number("2");
    }
    return Complex(res);
}

Rational Complex::getReal() const
{
    return this->_real;
}

Rational Complex::getImag() const
{
    return this->_imag;
}

string Complex::approximation() const
{
    if (Number("0") == this->_imag.getNumerator())
        return this->_real.approximation(); 
    if (Number("0") == this->_real.getNumerator())
    {
        if (Rational(this->_imag) == Rational("1"))
            return Computer::init().convert.to_bytes(L"𝒾");
        if (Rational(this->_imag) == Rational("-1"))
            return "-" + Computer::init().convert.to_bytes(L"𝒾");
        return this->_imag.approximation() + Computer::init().convert.to_bytes(L"𝒾");
    }
    if (Rational(this->_imag) == Rational("1"))
        return this->_real.approximation() + " + " + Computer::init().convert.to_bytes(L"𝒾");
    if (Rational(this->_imag) == Rational("-1"))
        return this->getReal().approximation() + " - " + Computer::init().convert.to_bytes(L"𝒾");
    if (Rational("0") > this->_imag)
        return this->_real.approximation() + " - " + this->_imag.abs().approximation() + Computer::init().convert.to_bytes(L"𝒾");
    return this->_real.approximation() + " + " + this->_imag.abs().approximation() + Computer::init().convert.to_bytes(L"𝒾");
}

ostream &operator<<(ostream &o, Complex const &rhs)
{
    if (Number("0") == rhs.getImag().getNumerator())
        o << rhs.getReal();
    else if (Number("0") == rhs.getReal().getNumerator())
    {
        o << rhs.getImag() << Computer::init().convert.to_bytes(L"𝒾");
    }
    else if (Rational(rhs.getImag()) == Rational("1"))
        o << rhs.getReal() << " + " << Computer::init().convert.to_bytes(L"𝒾");
    else if (Rational(rhs.getImag()) == Rational("-1"))
        o << rhs.getReal() << " - " << Computer::init().convert.to_bytes(L"𝒾");
    else if (Rational("0") > rhs.getImag())
        o << rhs.getReal() << " - " << rhs.getImag().abs() << Computer::init().convert.to_bytes(L"𝒾");
    else
        o << rhs.getReal() << " + " << rhs.getImag() << Computer::init().convert.to_bytes(L"𝒾");
    return o;
}