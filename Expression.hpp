#ifndef EXPRESSION_HPP
# define EXPRESSION_HPP
#include "utils.hpp"
#include "Symbol.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Computer.hpp"

class Expression
{
public:
    class InvalidCharException : public exception
    {
    public:
        InvalidCharException(string expr, int pos) : _pos(pos), _expr(expr) {}
        virtual const char *what() const throw();

    private:
        int _pos;
        string _expr;
    };
    class InvalidExpression : public exception
    {
    public:
        InvalidExpression(int type, int tIdx, vector<string> const &tokens, int var1 = 0 , int var2 = 0);
        virtual const char *what() const throw();
        int _type;
    private:
        string _message;
    };
    class BadTokenException : public exception
    {
    public:
        BadTokenException(string token) : _token(token) {}
        virtual const char *what() const throw();

    private:
        string _token;
    };
    class BadParenthesisException : public exception
    {
    public:
        virtual const char *what() const throw();
    };
    class BadBracketException : public exception
    {
    public:
        virtual const char *what() const throw();
    };
    Expression();
    Expression(vector<Symbol *> const &symbols);
    Expression(string const &expr);
    Expression(string const &expr, vector<string> vars);
    Expression(Expression const &src);
    ~Expression();
    vector<Symbol *> getSymbols() const;
    Expression &operator=(Expression const &rhs);
    bool validChar(char const c) const;
    bool isOperator(char const c) const;
    vector<string> breakDown(string const &token) const;
    void toRPN();
    Symbol *tokenToSymbol(string const &token);
    Symbol *evaluate() const;
    void    print() const;
    vector<Symbol *> _rpn;
    vector<Symbol *> _symbols;
    set<string> unkowns;
    vector<string> placeHolders;
private:
    static string validChars;
    static string operators;
    vector<string> _tokens;
};

ostream&    operator<<(ostream &o,const Expression &rhs);
#endif
