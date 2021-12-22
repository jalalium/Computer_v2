#include "Computer.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "Polynomial.hpp"
#include "Function.hpp"


Computer    Computer::single_instance;

wstring Computer::utfExp = L"⁰¹²³⁴⁵⁶⁷⁸⁹";


string      Computer::getSyntaxErrorFlag() const
{
    return "$Syntax Error$: ";
}

string      Computer::getSemanticErrorFlag() const
{
    return "$Semantic Error$: ";
}

string      Computer::getErrorFlag() const
{
    return "$ERROR$: ";
}

int Computer::addition[5][5] = {{0,0,0,0,0}, 
                                {0,1,1,0,1}, 
                                {0,1,1,0,1}, 
                                {0,0,0,1,0},
                                {0,1,1,0,1}};
int Computer::subtraction[5][5] = {{0,0,0,0,0}, 
                                {0,1,1,0,1}, 
                                {0,1,1,0,1}, 
                                {0,0,0,1,0},
                                {0,1,1,0,1}};
int Computer::multiplication[5][5] = {{0,0,0,0,0}, 
                                {0,1,1,1,1}, 
                                {0,1,1,0,1}, 
                                {0,1,0,1,0},
                                {0,1,1,0,1}};
int Computer::division[5][5]= {{0,0,0,0,0}, 
                                {0,1,1,0,0}, 
                                {0,1,1,0,0}, 
                                {0,1,0,0,0},
                                {0,1,1,0,0}};
int Computer::modulo[5][5]  =   {{0,0,0,0,0}, 
                                {0,1,0,0,0}, 
                                {0,0,0,0,0}, 
                                {0,0,0,0,0},
                                {0,0,0,0,0}};

int Computer::power[5][5]   =   {{0,0,0,0,0}, 
                                {0,1,0,0,0}, 
                                {0,1,0,0,0}, 
                                {0,1,0,0,0},
                                {0,1,0,0,0}};

wchar_t Computer::getExponent(int e) const
{
    return utfExp[e];
}

wstring Computer::getWithRep() const
{
    return 	L"\u0305";
}

string Computer::displayRoot() const
{

    wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> convert;
    return convert.to_bytes(L"\u221A");
}

bool Computer::canAdd(int type1, int type2)
{
    return this->addition[type1][type2];
}
bool Computer::canSubtract(int type1, int type2)
{
    return this->subtraction[type1][type2];
}
bool Computer::canMultiply(int type1, int type2)
{
    return this->multiplication[type1][type2];
}
bool Computer::canDivide(int type1, int type2)
{
    return this->division[type1][type2];
}

bool Computer::canMod(int type1, int type2)
{
    return this->modulo[type1][type2];
}

bool Computer::canPow(int type1, int type2)
{
    return this->power[type1][type2];
}

bool Computer::isDeclaredFunction(string func) const
{
    return this->_declaredFunctions.count(toLower(func)) > 0;
}

bool Computer::isDeclaredVariable(string var) const
{
    return this->_declaredVariables.count(toLower(var)) > 0;
}

Symbol *Computer::getFunctionSymbol(string func)
{
    return this->_declaredFunctions[toLower(func)];
}

Symbol *Computer::getVariableSymbol(string var)
{
    return this->_declaredVariables[toLower(var)];
}

void        Computer::printError(string const &error) const
{
    int redColor = 0;
    int tabCount = 0;
    for (size_t idx = 0; idx < error.length(); idx++)
    {
        char c = error[idx];
        if (c == '$')
        {
            if (!redColor)
                cout << "\033[31m";
            else
                cout << "\033[0m";
            redColor ^= 1;
        }
        else if (c == '&')
            tabCount++;
        else if (c == '#')
            tabCount--;
        else if (c == '\n')
        {
            cout << endl;
            if (idx != error.length() - 1)
            {
                for (int i = 0; i < tabCount * 3; i++)
                    cout << " ";
            }
        }
        else
            cout << c;
    }
}

string      Computer::getType(int type) const
{
    if (type == 0)
        return "Operator";
    if (type == 1)
        return "Rational";
    if (type == 2)
        return "Complex";
    if (type == 3)
        return "Matrix";
    if (type == 4)
        return "Polynomial";
    if (type == 5)
        return "Non positive integer";
    return "";
}

void    Computer::addVariable(string name, Symbol* value)
{
    this->_declaredVariables[toLower(name)] = value;
}

void    Computer::addFunction(string name, Symbol* value)
{
    this->_declaredFunctions[toLower(name)] = value;
}

void    Computer::addAllocatedAddress(void* ptr)
{
    this->allocatedAddresses[this->_pos++] = ptr;
}

void    Computer::removeAllocatedAddress(void* ptr)
{
    for (int idx = 0; idx < this->_pos; idx++)
        if (this->allocatedAddresses[idx] == ptr)
            this->allocatedAddresses[idx] = nullptr;
}

void    Computer::freeMemory()
{
    for (int idx = 0; idx < this->_pos; idx++)
    {
        if (this->allocatedAddresses[idx] != nullptr)
        {
            free(this->allocatedAddresses[idx]);
            this->allocatedAddresses[idx] = nullptr;
        }
    }
    this->_pos = 0;
}

void    Computer::getHistory() const
{
    cout << this->history.str();
}

void    Computer::clearVars()
{
    this->_declaredVariables.clear();
}

void    Computer::clearFunctions()
{
    this->_declaredFunctions.clear();
}

void    Computer::getFunctions() const
{
    if (this->_declaredFunctions.size() == 4)
    {
        cout << "NO STORED FUNCTIONS" << endl;
        return ;
    }
    for (auto t : this->_declaredFunctions)
    {
        Function* fun = dynamic_cast<Function*>(t.second);
        if (fun != NULL)
            cout << fun->getDeclaration() << " = " << fun->getExpression() << endl;
    }
}

void    Computer::getVariables() const
{
    if (this->_declaredVariables.size() == 0)
    {
        cout << "NO STORED VARIABLES" << endl;
        return ;
    }
    for (auto t : this->_declaredVariables)
    {
        Rational *rat = dynamic_cast<Rational *>(t.second);
        Complex *comp = dynamic_cast<Complex *>(t.second);
        Matrix *mat = dynamic_cast<Matrix *>(t.second);
        Polynomial *pl = dynamic_cast<Polynomial *>(t.second);
        cout << t.first << " = ";
        if (rat != NULL)
            cout << *rat << endl;
        if (comp != NULL)
            cout << *comp << endl;
        if (mat != NULL)
            cout << *mat << endl;
        if (pl != NULL)
            cout << *pl << endl;
    }
}

bool Computer::isSystem(string const &src) const
{
    string tmp = toLower(src);
    if (tmp.compare("quit") == 0)
        return true;
    if (tmp.compare("history") == 0)
        return true;
    if (tmp.compare("variables") == 0)
        return true;
    if (tmp.compare("functions") == 0)
        return true;
    if (tmp.compare("clearvariables") == 0)
        return true;
    return false;
}