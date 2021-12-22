#ifndef MATRIX_HPP
# define MATRIX_HPP
#include "utils.hpp"
#include "Symbol.hpp"
#include "Rational.hpp"

class Matrix : public Symbol
{
public:
    class BadMatrixException : public exception
    {
        virtual const char *what() const throw();
    };
    class BadMatrixOperationException : public exception
    {
        virtual const char *what() const throw();
    }; 
    class NegativePowerException : public exception
    {
        virtual const char *what() const throw();
    }; 
    Matrix();
    Matrix(string const &input);
    Matrix(int row, int colomn, vector<Rational> &inp);
    Matrix(int row, int colomn);
    Matrix(Matrix const &src);
    ~Matrix();
    Matrix& operator=(Matrix const &rhs);
    Matrix operator+(Matrix const &rhs);
    Matrix operator-(Matrix const &rhs);
    Matrix operator*(Matrix const &rhs);
    Matrix operator^(Matrix const &rhs);
    Matrix operator^(Number power);
    Matrix  getIdentity();
    void multiplyBy(Rational rational);
    int getRow() const;
    int getCol() const;
    vector<Rational> getMatrix() const;
private:
    int _row;
    int _col;
    vector<Rational> _matrix;
};

ostream& operator<<(ostream &o, Matrix const &rhs);
#endif
