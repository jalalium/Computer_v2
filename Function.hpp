#ifndef FUNCTION_HPP
# define FUNCTION_HPP
#include "utils.hpp"
#include "Symbol.hpp"
#include "Expression.hpp"
#include <map>

class Function : public Symbol
{
public:
    class WrongNumberOfVariablesException : public exception
    {
        virtual const char* what() const throw();
    };
    class BadFunctionDefinition : public exception
    {
        public:
            BadFunctionDefinition(string reason, int error);
        virtual const char* what() const throw();
        string  _definition;
        int     _error;
    };
    Function();
    Function(string &expression, string &variable);
    Function(string definition);
    Function(Expression const &expr);
    Function(Function const &src);
    ~Function();
    Function& operator=(Function const &rhs);
    Symbol* operator()(vector<Symbol*> const &vars);
    int getVariablesCount() const;
    string getName() const;
    vector<string> getVariables() const;
    bool hasVariable(string variable) const;
    void setExpression(Expression expr);
    string getFName() const;
    string getDeclaration() const;
    Expression getExpression() const;
private:
    Expression _expression;
    vector<string> _variables;
    set<string> _usedVariables;
    string _name;
    string _declaration;
};
#endif