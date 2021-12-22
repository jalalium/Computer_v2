#ifndef COMPLEX_HPP
#define COMPLEX_HPP
#include "utils.hpp"
#include "Rational.hpp"

class Complex : public Symbol
{
public:
    class ModuloByNonIntException : public exception
    {
        virtual const char *what() const throw();
    };
    class DivisionByZeroException : public exception
    {
        virtual const char *what() const throw();
    };
    class NegativePowerException : public exception
    {
        virtual const char *what() const throw();
    };
    Complex();
    Complex(Rational real, Rational imag);
    Complex(Complex const &src);
    ~Complex();
    bool operator==(Complex const &rhs);
    Complex &operator=(Complex const &rhs);
    Complex operator+(Complex const &rhs);
    Complex operator-(Complex const &rhs);
    Complex operator*(Complex const &rhs);
    Complex operator/(Complex const &rhs);
    Complex operator+(Rational const &rhs);
    Complex operator-(Rational const &rhs);
    Complex operator*(Rational const &rhs);
    Complex operator/(Rational const &rhs);
    Complex operator^(Number power);
    string approximation() const;
    Rational getReal() const;
    Rational getImag() const;

private:
    Rational _real;
    Rational _imag;
};

ostream &operator<<(ostream &o, Complex const &rhs);
#endif
