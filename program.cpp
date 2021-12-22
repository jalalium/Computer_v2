#include "Number.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix.hpp"
#include "assert.h"
#include "Expression.hpp"
#include "Function.hpp"
#include "Polynomial.hpp"
#define EVALUATION 1
#define ASSIGNIATION 2
#define RESOLUTION 3
#define SYSTEM 4
#define SKIP 5

int getTypeOfCommand(string &command)
{
    if (getCount(command, '$') || getCount(command, '&'))
        throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Invalid expression\n");
    if (Computer::init().isSystem(command))
        return SYSTEM;
    int equalCount = getCount(command, '=');
    int interoCount = getCount(command, '?');
    if (removeWhiteSpace(command).length() == 0)
        return SKIP;
    if (!equalCount && !interoCount)
        return EVALUATION;
    if (equalCount > 1)
        throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Number of Equal signs greater than 1.\n" + highlightError(command, '=') + "\n");
    if (interoCount > 1)
        throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Number of Question marks greater than 1.\n" + highlightError(command, '?') + "\n");
    if (interoCount == 1)
    {
        string noWhiteSpace = removeWhiteSpace(command);
        if (noWhiteSpace.back() != '?')
            throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Question mark only allowed at the end of the expression.\n" + highlightError(command, '?') + "\n");
        else if (noWhiteSpace[noWhiteSpace.length() - 2] == '=')
            return EVALUATION;
        else
            return RESOLUTION;
    }
    else
    {
        string lhs;
        string rhs;
        vector<string> sides;
        sides = splitString(command, '=');
        if (sides.size() < 2)
            throw runtime_error("Invalid expression\n");
        lhs = sides[0];
        rhs = sides[1];
        if (removeWhiteSpace(rhs).length() == 0)
            throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Right hand side of the equation empty.\n");
        if (removeWhiteSpace(lhs).length() == 0)
            throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Left hand side of the equation empty.\n");
        return ASSIGNIATION;
    }
}

pair<string, Symbol *> performAssignationOf(string command)
{
    string lhs;
    string rhs;
    vector<string> sides;
    sides = splitString(command, '=');
    lhs = sides[0];
    rhs = sides[1];
    string functionError;
    string varError;
    try
    {
        Function *func = new Function(lhs);
        Expression erhs = Expression(rhs, func->getVariables());
        Symbol *res;
        try
        {
            res = erhs.evaluate();
        }
        catch (Expression::InvalidExpression &e)
        {
            if (e._type == 1)
                throw e;
            res = NULL;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        for (string unkown : erhs.unkowns)
            if (!func->hasVariable(unkown))
                throw runtime_error(Computer::init().getSemanticErrorFlag() + rhs + " contains variables not present in function definition.");
        func->setExpression(erhs);
        Computer::init().addFunction(func->getFName(), func);
        return make_pair(func->getFName(), res);
    }
    catch (Function::BadFunctionDefinition &e)
    {
        functionError = ("Error while performing Assignation of : " +
                         lhs + "\n&" +
                         e.what() + "#");
    }
    catch (Expression::InvalidCharException &e)
    {
        functionError = ("Error while parsing : " +
                         rhs + "\n&" +
                         e.what() + "#");
    }
    catch (Expression::BadTokenException &e)
    {
        functionError = ("Error while tokenizing : " +
                         rhs + "\n&" +
                         e.what() + "#");
    }
    catch (Expression::BadParenthesisException &e)
    {
        functionError = ("Error while processing : " +
                         rhs + "\n&" +
                         e.what() + "#");
    }
    catch (Expression::InvalidExpression &e)
    {
        functionError = ("Error while evaluating : " +
                         rhs + "\n&" +
                         e.what() + "#");
    }
    catch (const std::exception &e)
    {
        functionError = e.what();
    }
    try
    {
        lhs = removeWhiteSpace(lhs);
        if (!isAlpha(lhs))
            throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Variable name " + lhs + " should only contain letters.");
        if (lhs == "i")
            throw runtime_error(Computer::init().getSyntaxErrorFlag() + "Cannot assign variable with name " + lhs + ". It is reserved to complex numbers.");
        Expression erhs = Expression(rhs);
        Symbol *res = erhs.evaluate();
        Rational *rat = dynamic_cast<Rational *>(res);
        if (rat != NULL)
        {
            Computer::init().addVariable(lhs, res);
            return make_pair(lhs, res);
        }
        Complex *cp = dynamic_cast<Complex *>(res);
        if (cp != NULL)
        {
            Computer::init().addVariable(lhs, res);
            return make_pair(lhs, res);
        }
        Matrix *mat = dynamic_cast<Matrix *>(res);
        if (mat != NULL)
        {
            Computer::init().addVariable(lhs, res);
            return make_pair(lhs, res);
        }
        throw runtime_error(Computer::init().getSemanticErrorFlag() + "Could not evaluate " + rhs + ".");
    }
    catch (runtime_error &e)
    {
        varError = e.what();
    }
    catch (Expression::InvalidCharException &e)
    {
        varError = ("Error while parsing : " +
                    rhs + "\n&" +
                    e.what() + "#");
    }
    catch (Expression::BadTokenException &e)
    {
        varError = ("Error while tokenizing : " +
                    rhs + "\n&" +
                    e.what() + "#");
    }
    catch (Expression::BadParenthesisException &e)
    {
        varError = ("Error while processing : " +
                    rhs + "\n&" +
                    e.what() + "#");
    }
    catch (Expression::BadBracketException &e)
    {
        varError = ("Error while processing : " +
                    rhs + "\n&" +
                    e.what() + "#");
    }
    catch (Expression::InvalidExpression &e)
    {
        varError = ("Error while evaluating : " +
                    rhs + "\n&" +
                    e.what() + "#");
    }
    catch (const std::exception &e)
    {
        varError = e.what();
    }
    throw runtime_error(Computer::init().getErrorFlag() + "Error while performing assignation command\n" +
                        "Did you mean to assign a function?&\n" + "&" + functionError + "##\n"
                                                                                        "Did you mean to assign a variable?&\n" +
                        "&" + varError + "##\n");
}

Symbol *performEvaluationOf(string command)
{
    try
    {
        if (!getCount(command, '=') && !getCount(command, '?'))
        {
            Symbol *res = Expression(command).evaluate();
            Rational *rat = dynamic_cast<Rational *>(res);
            Complex *comp = dynamic_cast<Complex *>(res);
            Matrix *mat = dynamic_cast<Matrix *>(res);
            Polynomial *pl = dynamic_cast<Polynomial *>(res);
            if (!rat && !comp && !mat && !pl)
                throw runtime_error("Too many Unkowns");
            return res;
        }
        vector<string> sides = splitString(command, '=');
        if (sides.size() != 2)
            throw runtime_error("Error while parsing : " +
                                command + "\n&" +
                                "Too many equal signs" + "##\n");
        if (trimWhitSpace(sides[1]).compare("?") != 0)
            throw runtime_error("Error while parsing : " +
                                command + "\n&" +
                                "Expecting ? in rhs of = sign, found " + sides[1] + "##\n");
        Symbol *res = Expression(sides[0]).evaluate();
        Rational *rat = dynamic_cast<Rational *>(res);
        Complex *comp = dynamic_cast<Complex *>(res);
        Matrix *mat = dynamic_cast<Matrix *>(res);
        Polynomial *pl = dynamic_cast<Polynomial *>(res);
        if (!rat && !comp && !mat && !pl)
            throw runtime_error("Error while evaluating : " +
                                command + "\n&" +
                                "Too many unkowns" + "##\n");
        return res;
    }
    catch (Expression::InvalidCharException &e)
    {
        throw runtime_error("Error while parsing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::BadTokenException &e)
    {
        throw runtime_error("Error while tokenizing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::BadParenthesisException &e)
    {
        throw runtime_error("Error while processing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::InvalidExpression &e)
    {
        throw runtime_error("Error while evaluating : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (runtime_error &e)
    {
        throw runtime_error("Error while evaluating : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (exception &e)
    {
        throw runtime_error("Error while evaluating : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
}

void performResolutionOf(string command)
{
    try
    {
        command = removeWhiteSpace(command);
        command = command.substr(0, command.length() - 1);
        vector<string> sides = splitString(command, '=');
        if (sides.size() != 2)
            throw runtime_error("Error while parsing : " +
                                command);
        Expression lhs = Expression(sides[0]);
        Expression rhs = Expression(sides[1]);
        Symbol *elhs = lhs.evaluate();
        Symbol *erhs = rhs.evaluate();
        vector<Symbol *> difference = {elhs, new Symbol(OPERATOR, priority('-'), "-"), erhs};
        Symbol *equation = Expression(difference).evaluate();
        if (dynamic_cast<Rational *>(equation))
        {
            Rational *res = dynamic_cast<Rational *>(equation);
            if (res->getNumerator() == Number("0"))
            {
                cout << "Expression is always true" << endl;
                Computer::init().history << "Expression is always true" << endl;
                Computer::init().history << endl;
            }
            else
            {
                cout << "Expression is never true" << endl;
                Computer::init().history << "Expression is never true" << endl;
                Computer::init().history << endl;
            }
        }
        else if (dynamic_cast<Complex *>(equation))
        {
            Complex *res = dynamic_cast<Complex *>(equation);
            if (res->getReal().getNumerator() == Number("0") && res->getImag().getNumerator() == Number("0"))
            {
                cout << "Expression is always true" << endl;
                Computer::init().history << "Expression is always true" << endl;
                Computer::init().history << endl;
            }
            else
            {
                cout << "Expression is never true" << endl;
                Computer::init().history << "Expression is never true" << endl;
                Computer::init().history << endl;
            }
        }
        else if (dynamic_cast<Matrix *>(equation))
        {
            cout << "Invalid equation, lhs is Matrix and rhs is Rational" << endl;
            Computer::init().history << "Invalid equation, lhs is Matrix and rhs is Rational" << endl;
            Computer::init().history << endl;
        }
        else if (dynamic_cast<Polynomial *>(equation))
        {
            set<string> variables;
            Polynomial *res = dynamic_cast<Polynomial *>(equation);
            Number largestExponent = Number("0");
            for (auto t : res->_form)
            {
                for (auto t2 : t.first)
                {
                    if (t2.first.length() > 0)
                        variables.insert(t2.first);
                    if (Number(t2.second) > largestExponent)
                        largestExponent = Number(t2.second);
                }
            }
            if (variables.size() > 1)
            {
                cout << "Equation with more than one variable! cannot solve it!" << endl;
                Computer::init().history << "Equation with more than one variable! cannot solve it!" << endl;
                Computer::init().history << endl;
            }
            else if (largestExponent > Number("2"))
            {
                cout << "Degree Of polynomial larger than 2, cannot solve it!" << endl;
                Computer::init().history << "Degree Of polynomial larger than 2, cannot solve it!" << endl;
                Computer::init().history << endl;
            }
            else if (largestExponent == Number("2"))
                res->solveSecondDegree();
            else
                res->solveFirstDegree();
        }
    }
    catch (Expression::InvalidCharException &e)
    {
        throw runtime_error("Error while parsing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::BadTokenException &e)
    {
        throw runtime_error("Error while tokenizing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::BadParenthesisException &e)
    {
        throw runtime_error("Error while processing : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (Expression::InvalidExpression &e)
    {
        throw runtime_error("Error while evaluating : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (runtime_error &e)
    {
        throw runtime_error("Error while resolving : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
    catch (exception &e)
    {
        throw runtime_error("Error while resolving : " +
                            command + "\n&" +
                            e.what() + "##\n");
    }
}

void addBuiltInFunctions()
{
    Computer::init().addFunction("sqrt", new Symbol(5, -1, "sqrt"));
    Computer::init().addFunction("abs", new Symbol(5, -1, "abs"));
    Computer::init().addFunction("gcd", new Symbol(5, -1, "gcd"));
    Computer::init().addFunction("factorial", new Symbol(5, -1, "factorial"));
}

void performSystem(string command)
{
    if (command.compare("quit") == 0)
    {
        Computer::init().freeMemory();
        exit(0);
    }
    else if (command.compare("history") == 0)
        Computer::init().getHistory();
    else if (command.compare("variables") == 0)
        Computer::init().getVariables();
    else if (command.compare("functions") == 0)
        Computer::init().getFunctions();
    else if (command.compare("clearVariables") == 0)
        Computer::init().clearVars();
}

int main()
{
    string command;
    int commandType;
    addBuiltInFunctions();
    while (true)
    {
        cout << "> ";
        if (!getline(cin, command, '\n'))
            break;
        try
        {
            commandType = getTypeOfCommand(command);
            switch (commandType)
            {
            case SKIP:
                break;
            case ASSIGNIATION:
                try
                {
                    pair<string, Symbol *> assign = performAssignationOf(command);
                    Rational *rat = dynamic_cast<Rational *>(assign.second);
                    if (assign.second == NULL)
                    {
                        Expression expr = Expression(splitString(command, '=')[1],Function(splitString(command, '=')[0]).getVariables());
                        cout << expr << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << expr << endl;
                        Computer::init().history << endl;
                    }
                    if (rat != NULL)
                    {
                        cout << *rat << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *rat << endl;
                        Computer::init().history << endl;
                    }
                    Complex *comp = dynamic_cast<Complex *>(assign.second);
                    if (comp != NULL)
                    {
                        cout << *comp << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *comp << endl;
                        Computer::init().history << endl;
                    }
                    Matrix *mat = dynamic_cast<Matrix *>(assign.second);
                    if (mat != NULL)
                    {
                        cout << *mat << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *mat << endl;
                        Computer::init().history << endl;
                    }
                    Polynomial *pl = dynamic_cast<Polynomial *>(assign.second);
                    if (pl != NULL)
                    {
                        cout << *pl << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *pl << endl;
                        Computer::init().history << endl;
                    }
                }
                catch (runtime_error &e)
                {
                    Computer::init().printError(e.what());
                }
                break;
            case EVALUATION:
                try
                {
                    Symbol *res = performEvaluationOf(command);
                    if (dynamic_cast<Rational *>(res) != NULL)
                    {
                        cout << *(dynamic_cast<Rational *>(res)) << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *(dynamic_cast<Rational *>(res)) << endl;
                        Computer::init().history << endl;
                    }
                    else if (dynamic_cast<Complex *>(res) != NULL)
                    {
                        cout << *(dynamic_cast<Complex *>(res)) << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *(dynamic_cast<Complex *>(res)) << endl;
                        Computer::init().history << endl;
                    }
                    else if (dynamic_cast<Matrix *>(res) != NULL)
                    {
                        cout << *(dynamic_cast<Matrix *>(res)) << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *(dynamic_cast<Matrix *>(res)) << endl;
                        Computer::init().history << endl;
                    }
                    else if (dynamic_cast<Polynomial *>(res) != NULL)
                    {
                        cout << *(dynamic_cast<Polynomial *>(res)) << endl;
                        Computer::init().history << command << endl;
                        Computer::init().history << *(dynamic_cast<Polynomial *>(res)) << endl;
                        Computer::init().history << endl;
                    }
                }
                catch (runtime_error &e)
                {
                    Computer::init().printError(Computer::init().getErrorFlag() +
                                                "Error while performing evaluation command&\n" +
                                                e.what());
                }
                break;
            case RESOLUTION:
                try
                {
                    Computer::init().history << command << endl;
                    performResolutionOf(command);
                }
                catch (runtime_error &e)
                {
                    Computer::init().printError(Computer::init().getErrorFlag() +
                                                "Error while performing resolution command&\n" +
                                                e.what());
                }
                break;
            case SYSTEM:
                performSystem(command);
            }
        }
        catch (exception &e)
        {
            Computer::init().printError(e.what());
        }
    }
    Computer::init().freeMemory();
}
