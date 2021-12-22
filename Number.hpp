#ifndef NUMBER_HPP
# define NUMBER_HPP
#include "utils.hpp"
#include "Symbol.hpp"
# define NUMBER_LIMIT 1000000

class Number : public Symbol
{
public:
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
    class NegativePowerException : public exception
    {
        virtual const char* what() const throw();
    };
    Number();
    Number(string const &data) throw(NotANumberException, NumberTooLongException);
    ~Number();
    Number operator=(Number const &rhs);
    Number operator+(Number const &rhs);
    Number operator-(Number const &rhs);
    bool operator>=(Number const &rhs);
    bool operator>(Number const &rhs);
    bool operator<=(Number const &rhs);
    bool operator<(Number const &rhs);
    bool operator==(Number const &rhs);
    bool operator!=(Number const &rhs);
    Number operator*(Number const &rhs);
    Number operator/(Number const &rhs) throw(DivisionByZeroException);
    Number operator%(Number const &rhs) throw(ModuloByZeroException);
    Number operator^(Number power) throw(NegativePowerException);
    string getValue() const;
private:
    string _value;
};

ostream& operator<<(ostream &o, Number const &rhs);

Number karatsuba(Number a, Number b);
Number gcd(Number a, Number b);

#endif
