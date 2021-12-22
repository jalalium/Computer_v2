#ifndef POLYNOMIAL_HPP
# define POLYNOMIAL_HPP
#include "Complex.hpp"

class Polynomial : public Symbol
{
public:
    Polynomial();
    Polynomial(Complex const &cst);
    Polynomial(Rational const &cst);
    Polynomial(string token);
    Polynomial(map< vector< pair <string, string> >, Complex> const &form);
    Polynomial(Polynomial const &src);
    ~Polynomial();
    Polynomial&                                         operator=(Polynomial const &rhs);
    Polynomial                                          operator+(Polynomial const &rhs);
    Polynomial                                          operator-(Polynomial const &rhs);
    Polynomial                                          operator*(Polynomial const &rhs);
    Polynomial                                          operator^(Number const &exp);
    void                                                solveSecondDegree() const;
    void                                                solveFirstDegree() const;
    map< vector< pair <string, string> > , Complex>     _form;
private:
};

ostream& operator<<(ostream &o, Polynomial const &rhs);
#endif