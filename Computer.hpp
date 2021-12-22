#ifndef COMPUTER_HPP
# define COMPUTER_HPP
#include "Symbol.hpp"
#include "utils.hpp"
#include <array>

class Computer
{
public:
    static Computer& init()
    {
        return single_instance;
    }
    bool        isDeclaredFunction(string func) const;
    bool        isDeclaredVariable(string var) const;
    bool        canAdd(int type1, int type2);
    bool        canSubtract(int type1, int type2);
    bool        canMultiply(int type1, int type2);
    bool        canDivide(int type1, int type2);
    bool        canMod(int type1, int type2);
    bool        canPow(int type1, int type2);
    void        addVariable(string name, Symbol* value);
    void        addFunction(string name, Symbol* value);
    void        getVariables() const;
    void        getFunctions() const;
    void        clearVars();
    void        clearFunctions();
    Symbol*     getFunctionSymbol(string func);
    Symbol*     getVariableSymbol(string var);
    wchar_t  getExponent(int e) const;
    wstring     getWithRep() const;
    string      displayRoot() const;
    string      getSyntaxErrorFlag() const;
    string      getErrorFlag() const;
    string      getSemanticErrorFlag() const;
    string      getType(int type) const;
    bool        isSystem(string const &c) const;
    void        printError(string const &tmp) const;
    void        addAllocatedAddress(void* ptr);
    void        removeAllocatedAddress(void* ptr);
    void        freeMemory();
    wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t>     convert;
    stringstream     history;
    void                getHistory() const;
private:
    Computer() {}
    static Computer single_instance;
    map<string, Symbol*> _internalFunctions;
    map<string, Symbol*> _declaredVariables;
    map<string, Symbol*> _declaredFunctions;
    array<void*,1000000> allocatedAddresses;
    int _pos;
    static int addition[5][5];
    static int subtraction[5][5];
    static int multiplication[5][5];
    static int division[5][5];
    static int modulo[5][5];
    static int power[5][5];
    static wstring utfExp;
};
#endif