#ifndef RATIONAL_HPP
# define RATIONAL_HPP
#include "utils.hpp"
#include "Symbol.hpp"
#include "Number.hpp"

class Complex;
class Matrix;
class Rational : public Symbol
{
public:
    class ModuloByNonIntException : public exception
    {
        virtual const char* what() const throw();
    };
    class ModuloByZeroException : public exception
    {
        virtual const char* what() const throw();
    };
    class DivisionByZeroException : public exception
    {
        virtual const char* what() const throw();
    };
    class NotANumberException : public exception
    {
        virtual const char* what() const throw();
    };
    class NumberTooLongException : public exception
    {
        virtual const char* what() const throw();
    };
    Rational();
    Rational(string data) throw (NumberTooLongException, NotANumberException);
    Rational(Rational const &src);
    Rational(Number const &numerator, Number const &denominator);
    ~Rational();
    bool       operator==(Rational const &rhs);
    bool       operator!=(Rational const &rhs);
    bool       operator>(Rational const &rhs);
    bool       operator>=(Rational const &rhs);
    bool       operator<(Rational const &rhs);
    bool       operator<=(Rational const &rhs);
    bool       isNumber() const; 
    Rational& operator=(Rational const &rhs);
    Rational operator+(Rational const &rhs);
    Rational operator-(Rational const &rhs);
    Rational operator*(Rational const &rhs);
    Rational operator/(Rational const &rhs) throw(DivisionByZeroException);
    Rational operator%(Rational const &rhs) throw(ModuloByNonIntException, ModuloByZeroException);
    Rational operator^(Number power);
    Rational  round() const;
    Complex operator+(Complex const &rhs);
    Complex operator-(Complex const &rhs);
    Complex operator*(Complex const &rhs);
    Complex operator/(Complex const &rhs);
    Matrix operator*(Matrix const &rhs);
    Complex getRoot() const;
    Number  getNumerator() const;
    Number  getDenominator() const;
    Rational    abs() const;
    pair <wstring,int>  getApproximation() const;
    string approximation() const;
private:
    Number _numerator;
    Number _denominator;
};

ostream& operator<<(ostream &o, Rational const &rhs);
#endif
