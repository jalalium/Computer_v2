#include "Function.hpp"
#include "Polynomial.hpp"


const char* Function::WrongNumberOfVariablesException::what() const throw()
{
    return "too much";
}

Function::BadFunctionDefinition::BadFunctionDefinition(string definition, int error) : _definition(definition), _error(error)
{

}
const char* Function::BadFunctionDefinition::what() const throw()
{
    if (this->_error == 0)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Too many opening brackets in function definition\n"
                + highlightError(_definition, '(')).c_str();
    if (this->_error == 1)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Function definition does not end in ')'\n"
                + highlightError(_definition, 0)).c_str();
    if (this->_error == 2)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Function name should only contain letters\n"
                + highlightError(_definition, 0, 1)).c_str();
    if (this->_error == 3)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Function has no arguments\n"
                + _definition).c_str();
    if (this->_error == 4)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Function argument missing\n"
                + highlightError(_definition, 0, 2)).c_str();
    if (this->_error == 5)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Argument name should only contain letters\n"
                + highlightError(_definition, 0, 1)).c_str();
    if (this->_error == 6)
        return (Computer::init().getSyntaxErrorFlag() 
                + "Left or right part of Opening bracket missing\n"
                + highlightError(_definition, '(')).c_str();
    if (this->_error == 7)
        return (Computer::init().getSyntaxErrorFlag() 
                + " Duplicate arguments in function definition.").c_str();
    return "Something went wrong";
}

Function::Function()
{
    Symbol::_type = 5;
}
Function::Function(string &expression, string &variable)
{
    Symbol::_type = 5;
    this->_expression = Expression(expression);
    this->_variables = splitString(variable, ',');
}
    
Function::Function(Function const &src)
{
    Symbol::_type = 5;
    *this = src;
}

Function::Function(Expression const &expr)
{
    Symbol::_type = 5;
    this->_expression = expr;
    vector<Symbol*> sym = this->_expression.getSymbols();
    for (int idx = 0; idx < (int)sym.size(); idx++)
        if (sym[idx]->getType() == -1)
            this->_usedVariables.insert(toLower(sym[idx]->getName()));
}

bool Function::hasVariable(string variable) const
{
    return this->_usedVariables.count(toLower(variable));
}

Function::Function(string definition)
{
    Symbol::_type = 5;
    vector<string> blocks = splitString(removeWhiteSpace(definition), '(');
    if (blocks.size() > 2)
        throw BadFunctionDefinition(definition, 0);
    if (blocks.size() < 2 || blocks[0].length() == 0 || blocks[1].length() == 0)
        throw BadFunctionDefinition(definition, 6);
    if (blocks[1].back() != ')')
        throw BadFunctionDefinition(definition, 1);
    blocks[1] = blocks[1].substr(0,blocks[1].length()-1);
    for (int idx = 0; idx < (int)blocks[0].length(); idx++)
        if (!(blocks[0][idx] >='a'&& blocks[0][idx] <= 'z') &&
            !(blocks[0][idx] >='A'&& blocks[0][idx] <= 'Z'))
            throw BadFunctionDefinition(blocks[0], 2);
    vector<string> arguments = splitString(blocks[1],',');
    if (arguments.size() == 0)
        throw BadFunctionDefinition(definition, 3);
    for (int idx = 0; idx < (int)arguments.size(); idx++)
    {
        if (arguments[idx].length() == 0)
            throw BadFunctionDefinition(blocks[1], 4);
        for (int idx1 = 0; idx1 < (int)arguments[idx].length(); idx1++)
            if (!(arguments[idx][idx1] >= 'a' && arguments[idx][idx1] <= 'z') &&
                !(arguments[idx][idx1] >= 'A' && arguments[idx][idx1] <= 'Z'))
            throw BadFunctionDefinition(arguments[idx], 5);
    }
    this->_name = blocks[0];
    this->_declaration = definition;
    this->_variables = arguments;
    for (string argument : arguments)
        this->_usedVariables.insert(toLower(argument));
    if (this->_variables.size() != this->_usedVariables.size())
        throw BadFunctionDefinition(definition, 7);
}

void Function::setExpression(Expression expr)
{
    this->_expression = expr;
}

string Function::getFName() const
{
    return this->_name;
}

string Function::getDeclaration() const
{
    return this->_declaration;
}

Expression Function::getExpression() const
{
    return this->_expression;
}


Function::~Function()
{
}

Function &Function::operator=(Function const &rhs)
{
    Symbol::_type = 5;
    this->_expression = rhs._expression;
    this->_variables = rhs._variables;
    this->_declaration = rhs._declaration;
    return *this;
}

Symbol *Function::operator()(vector<Symbol *> const &vars)
{
    if (vars.size() != this->_variables.size())
        throw WrongNumberOfVariablesException();
    map<string, Symbol *> input;
    for (int idx = 0; idx < (int)vars.size(); idx++)
    {
        input[toLower(this->_variables[idx])] = vars[idx];
    }
    vector<Symbol *> afterReplace;
    vector<Symbol *> sym = this->_expression.getSymbols();
    for (int idx = 0; idx < (int)sym.size(); idx++)
    {
        if (sym[idx]->getType() == POLYNOMIAL)
        {
            Polynomial* p = dynamic_cast<Polynomial*>(sym[idx]);
            if (p->_form.size() != 1)
                throw;
            string name;
            for (auto t : p->_form)
            {
                if (t.first.size() != 1)
                    throw;
                name = t.first[0].first;
            }
            afterReplace.push_back(input[toLower(name)]);
        }
        else
            afterReplace.push_back(sym[idx]);
    }
    Expression known = Expression(afterReplace);
    return known.evaluate();
}

int Function::getVariablesCount() const
{
    return this->_variables.size();
}

string Function::getName() const
{
    return this->_name;
}

vector<string> Function::getVariables() const
{
    return this->_variables;
}