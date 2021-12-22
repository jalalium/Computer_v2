#ifndef SYMBOL_HPP
# define SYMBOL_HPP
#include "utils.hpp"

#define OPERATOR 0
#define VALUE 1
#define COMPLEX 2
#define MATRIX 3
#define POLYNOMIAL 4
#define FUNCTION 5

class Symbol
{
public:
    Symbol();
    Symbol(int type, int priority, string name);
    Symbol(Symbol const &src);
    virtual ~Symbol(){}
    void*   operator new(size_t size);
    void    operator delete(void * p);
    int     getType() const;
    string  getName() const;
    int     getPriority() const;
    void    setTokenIndex(int idx);
    int     getTokenIndex() const;
protected:
    int     _type;
    int     _priority;
    string  _name;
    int     _tokenIndex;
};


#endif
