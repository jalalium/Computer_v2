#include "Symbol.hpp"
#include "Computer.hpp"

Symbol::Symbol()
{
}

Symbol::Symbol(Symbol const &rhs)
{
    *this = rhs;
}

void*   Symbol::operator new(size_t size)
{
    void * p = malloc(size);
    Computer::init().addAllocatedAddress(p);
    return p;
}

void    Symbol::operator delete(void* ptr)
{
    Computer::init().removeAllocatedAddress(ptr);
    std::free(ptr);
}

Symbol::Symbol(int type, int priority, string name) : _type(type), _priority(priority), _name(name)
{
}

int         Symbol::getType() const
{
    return this->_type;
}

int         Symbol::getPriority() const
{
    return this->_priority;
}

int         Symbol::getTokenIndex() const
{
    return this->_tokenIndex;
}


string      Symbol::getName() const
{
    return this->_name;
}

void       Symbol::setTokenIndex(int idx)
{
    this->_tokenIndex = idx;
}