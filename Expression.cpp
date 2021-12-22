#include "Expression.hpp"
#include "Function.hpp"
#include "Polynomial.hpp"

string Expression::validChars = " +-*/%^;,.[]()";
string Expression::operators = "+-*/%^(),";

const char *Expression::InvalidCharException::what() const throw()
{
    return (Computer::init().getSyntaxErrorFlag() +
            "Invalid character in expression:\n" +
            highlightError(this->_expr, 1, this->_pos))
        .c_str();
}

Expression::InvalidExpression::InvalidExpression(int type, int tIdx, vector<string> const &tokens, int var1, int var2)
{
    if (tIdx < 0 || tIdx >= (int)tokens.size())
    {
        this->_message = "Cannot Evaluate Expression";
        this->_type = 1;
        return;
    }
    string message;
    string underline;
    if (type == 0)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Operator " + tokens[tIdx] + " requires two operands.\n";
        this->_type = 1;
    }
    else if (type == 1)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Cannot perform operation " + tokens[tIdx] + " between operands of type: " + Computer::init().getType(var1) + " and type " + Computer::init().getType(var2) + ".\n";
        this->_type = 0;
    }
    else if (type == 2)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Function " + tokens[tIdx] + " requires " + to_string(var1) + " arguments, only " + to_string(var2) + " were provided.\n";
        this->_type = 1;
    }
    else if (type == 3)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Could not evaluate expression\n";
        this->_type = 1;
    }
    else if (type == 4)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Cannot perform operation " + tokens[tIdx] + " because right hand side is not valid.\n";
        this->_type = 1;
    }
    else if (type == 5)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Function " + tokens[tIdx] + " requires argument of Type Rational.\n";
        this->_type = 0;
    }
    else if (type == 6)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Function " + tokens[tIdx] + " requires argument of Type Number.\n";
        this->_type = 0;
    }
    else if (type == 7)
    {
        message = Computer::init().getSemanticErrorFlag();
        message += "Cannot perform operation " + tokens[tIdx] + "\n";
        this->_type = 1;
    }
    for (int idx = 0; idx < (int)tokens.size(); idx++)
    {
        if (idx != 0)
        {
            message += " ";
            underline += " ";
        }
        for (char c : tokens[idx])
        {
            message += c;
            if (idx == tIdx)
                underline += "$^$";
            else
                underline += " ";
        }
    }
    if (type == 3)
        this->_message = message;
    else
        this->_message = message + "\n" + underline;
}

const char *Expression::InvalidExpression::what() const throw()
{
    return (this->_message).c_str();
}

const char *Expression::BadTokenException::what() const throw()
{
    return (Computer::init().getSyntaxErrorFlag() +
            "Bad token : " + this->_token)
        .c_str();
}

const char *Expression::BadParenthesisException::what() const throw()
{
    return "Mismatched Parenthesis";
}

const char *Expression::BadBracketException::what() const throw()
{
    return (Computer::init().getSyntaxErrorFlag() + "Mismatched Brackets").c_str();
}

Expression::Expression()
{
}

Expression::Expression(vector<Symbol *> const &symbols)
{
    this->_symbols = symbols;
    this->toRPN();
}

Expression::Expression(string const &expr, vector<string> vars)
{
    this->placeHolders = vars;
    vector<string> tokens;
    string tmp;
    for (int idx = 0; idx < (int)expr.length(); idx++)
        if (!isWhiteSpace(expr[idx]) && !validChar(expr[idx]))
            throw InvalidCharException(expr, idx);
    int idx1 = 0;
    while (idx1 < (int)expr.length())
    {
        if (isWhiteSpace(expr[idx1]))
        {
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            idx1++;
        }
        else if (isOperator(expr[idx1]))
        {
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            tmp += expr[idx1];
            tokens.push_back(tmp);
            tmp.clear();
            idx1++;
        }
        else if (expr[idx1] == '[')
        {
            int cnt = 1;
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            tmp += expr[idx1];
            idx1++;
            while (idx1 < (int)expr.length() && cnt != 0)
            {
                tmp += expr[idx1];
                if (expr[idx1] == ']')
                    cnt--;
                if (expr[idx1] == '[')
                    cnt++;
                idx1++;
            }
            if (cnt != 0)
                throw BadBracketException();
            tokens.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp += expr[idx1];
            idx1++;
        }
    }
    if (tmp.length())
        tokens.push_back(tmp);
    vector<string> tmp2;
    for (int idx = 0; idx < (int)tokens.size(); idx++)
    {
        vector<string> blocks = breakDown(tokens[idx]);
        for (int idx2 = 0; idx2 < (int)blocks.size(); idx2++)
            tmp2.push_back(blocks[idx2]);
    }
    tokens = tmp2;
    this->_tokens = tokens;
    for (int idx = 0; idx < (int)tokens.size(); idx++)
    {
        try
        {
            Symbol *sym = tokenToSymbol(tokens[idx]);
            sym->setTokenIndex(idx);
            this->_symbols.push_back(sym);
        }
        catch (BadTokenException &e)
        {
            throw e;
        }
    }
    vector<Symbol *> finalSymbols;
    int idx = 0;
    while (idx < (int)this->_symbols.size() - 1)
    {
        if (this->_symbols[idx]->getName()[0] == '*' && this->_symbols[idx + 1]->getName()[0] == '*')
        {
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "**"));
            idx += 2;
        }
        else if (this->_symbols[idx]->getType() == VALUE && this->_symbols[idx + 1]->getType() > 1)
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (this->_symbols[idx]->getType() >0 && this->_symbols[idx + 1]->getType() == OPERATOR && this->_symbols[idx + 1]->getName()[0] == '(')
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (this->_symbols[idx]->getType() == OPERATOR && this->_symbols[idx]->getName()[0] == ')' &&
                 this->_symbols[idx + 1]->getType() == OPERATOR && this->_symbols[idx + 1]->getName()[0] == '(')
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (idx == 0 && this->_symbols[idx]->getName()[0] == '-')
        {
            finalSymbols.push_back(new Rational("0"));
            finalSymbols.push_back(this->_symbols[idx]);
            idx++;
        }
        else if (idx != 0 && this->_symbols[idx]->getName()[0] == '-' && this->_symbols[idx - 1]->getType() == 0 && this->_symbols[idx - 1]->getName()[0] != '-')
        {
            finalSymbols.push_back(new Rational("0"));
            finalSymbols.push_back(this->_symbols[idx]);
            idx++;
        }
        else
        {
            finalSymbols.push_back(this->_symbols[idx]);
            idx++;
        }
    }
    if (idx < (int)this->_symbols.size())
        finalSymbols.push_back(this->_symbols[idx]);
    this->_symbols = finalSymbols;
    idx = 0;
    try
    {
        this->toRPN();
    }
    catch (Expression::BadParenthesisException &e)
    {
        throw e;
    }
}

Expression::Expression(string const &expr)
{
    vector<string> tokens;
    string tmp;
    for (int idx = 0; idx < (int)expr.length(); idx++)
        if (!isWhiteSpace(expr[idx]) && !validChar(expr[idx]))
            throw InvalidCharException(expr, idx);
    int idx1 = 0;
    while (idx1 < (int)expr.length())
    {
        if (isWhiteSpace(expr[idx1]))
        {
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            idx1++;
        }
        else if (isOperator(expr[idx1]))
        {
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            tmp += expr[idx1];
            tokens.push_back(tmp);
            tmp.clear();
            idx1++;
        }
        else if (expr[idx1] == '[')
        {
            int cnt = 1;
            if (tmp.length())
                tokens.push_back(tmp);
            tmp.clear();
            tmp += expr[idx1];
            idx1++;
            while (idx1 < (int)expr.length() && cnt != 0)
            {
                tmp += expr[idx1];
                if (expr[idx1] == ']')
                    cnt--;
                if (expr[idx1] == '[')
                    cnt++;
                idx1++;
            }
            if (cnt != 0)
                throw BadBracketException();
            tokens.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp += expr[idx1];
            idx1++;
        }
    }
    if (tmp.length())
        tokens.push_back(tmp);
    vector<string> tmp2;
    for (int idx = 0; idx < (int)tokens.size(); idx++)
    {
        vector<string> blocks = breakDown(tokens[idx]);
        for (int idx2 = 0; idx2 < (int)blocks.size(); idx2++)
            tmp2.push_back(blocks[idx2]);
    }
    tokens = tmp2;
    this->_tokens = tokens;
    for (int idx = 0; idx < (int)tokens.size(); idx++)
    {
        try
        {
            Symbol *sym = tokenToSymbol(tokens[idx]);
            sym->setTokenIndex(idx);
            this->_symbols.push_back(sym);
        }
        catch (BadTokenException &e)
        {
            throw e;
        }
    }
    vector<Symbol *> finalSymbols;
    int idx = 0;
    while (idx < (int)this->_symbols.size() - 1)
    {
        if (this->_symbols[idx]->getName()[0] == '*' && this->_symbols[idx + 1]->getName()[0] == '*')
        {
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "**"));
            idx += 2;
        }
        else if (this->_symbols[idx]->getType() == VALUE && this->_symbols[idx + 1]->getType() > 1)
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (this->_symbols[idx]->getType() == VALUE && this->_symbols[idx + 1]->getType() == OPERATOR && this->_symbols[idx + 1]->getName()[0] == '(')
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (this->_symbols[idx]->getType() == OPERATOR && this->_symbols[idx]->getName()[0] == ')' &&
                 this->_symbols[idx + 1]->getType() == OPERATOR && this->_symbols[idx + 1]->getName()[0] == '(')
        {
            finalSymbols.push_back(this->_symbols[idx]);
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (idx == 0 && this->_symbols[idx]->getName()[0] == '-' && this->_symbols[idx+1]->getType() > 0)
        {
            finalSymbols.push_back(new Rational("-1"));
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else if (idx != 0 && this->_symbols[idx]->getName()[0] == '-' && this->_symbols[idx - 1]->getType() == 0 && (this->_symbols[idx - 1]->getName()[0] != ')'))
        {
            finalSymbols.push_back(new Rational("-1"));
            finalSymbols.push_back(new Symbol(OPERATOR, 2, "*"));
            idx++;
        }
        else
        {
            finalSymbols.push_back(this->_symbols[idx]);
            idx++;
        }
    }
    if (idx < (int)this->_symbols.size())
        finalSymbols.push_back(this->_symbols[idx]);
    this->_symbols = finalSymbols;
    idx = 0;
    try
    {
        this->toRPN();
    }
    catch (Expression::BadParenthesisException &e)
    {
        throw e;
    }
}
Symbol *symbolAdd(Symbol *a, Symbol *b)
{
    Rational *rationala = dynamic_cast<Rational *>(a);
    Rational *rationalb = dynamic_cast<Rational *>(b);
    Complex *compa = dynamic_cast<Complex *>(a);
    Complex *compb = dynamic_cast<Complex *>(b);
    Matrix *mata = dynamic_cast<Matrix *>(a);
    Matrix *matb = dynamic_cast<Matrix *>(b);
    Polynomial *pa = dynamic_cast<Polynomial *>(a);
    Polynomial *pb = dynamic_cast<Polynomial *>(b);

    if (rationala != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)(new Rational(*rationala + *rationalb));
        else if (compb != NULL)
            return (Symbol *)new Complex(*rationala + *compb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*rationala) + *pb);
    }
    else if (compa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Complex(*compa + *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*compa + *compb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*compa) + *pb);
    }
    else if (mata != NULL)
        return (Symbol *)new Matrix(*mata + *matb);
    else if (pa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*rationalb) + *pa);
        else if (compb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*compb) + *pa);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(*pb + *pa);
    }
    return NULL;
}

Symbol *symbolSubtract(Symbol *a, Symbol *b)
{
    Rational *rationala = dynamic_cast<Rational *>(a);
    Rational *rationalb = dynamic_cast<Rational *>(b);
    Complex *compa = dynamic_cast<Complex *>(a);
    Complex *compb = dynamic_cast<Complex *>(b);
    Matrix *mata = dynamic_cast<Matrix *>(a);
    Matrix *matb = dynamic_cast<Matrix *>(b);
    Polynomial *pa = dynamic_cast<Polynomial *>(a);
    Polynomial *pb = dynamic_cast<Polynomial *>(b);
    if (rationala != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Rational(*rationala - *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*rationala - *compb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*rationala) - *pb);
    }
    else if (compa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Complex(*compa - *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*compa - *compb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*compa) - *pb);
    }
    else if (pa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Polynomial(*pa - Polynomial(*rationalb));
        else if (compb != NULL)
            return (Symbol *)new Polynomial(*pa - Polynomial(*compb));
        else if (pb != NULL)
            return (Symbol *)new Polynomial(*pa - *pb);
    }
    else
        return (Symbol *)new Matrix(*mata - *matb);
    return NULL;
}

Symbol *symbolMultiply(Symbol *a, Symbol *b)
{
    Rational *rationala = dynamic_cast<Rational *>(a);
    Rational *rationalb = dynamic_cast<Rational *>(b);
    Complex *compa = dynamic_cast<Complex *>(a);
    Complex *compb = dynamic_cast<Complex *>(b);
    Matrix *mata = dynamic_cast<Matrix *>(a);
    Matrix *matb = dynamic_cast<Matrix *>(b);
    Polynomial *pa = dynamic_cast<Polynomial *>(a);
    Polynomial *pb = dynamic_cast<Polynomial *>(b);
    if (rationala != NULL)
    {
        if (rationalb != NULL)
        {
            return (Symbol *)new Rational(*rationala * *rationalb);
        }
        else if (compb != NULL)
            return (Symbol *)new Complex(*rationala * *compb);
        else if (matb != NULL)
            return (Symbol *)new Matrix(*rationala * *matb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(Polynomial(*rationala) * *pb);
    }
    else if (compa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Complex(*compa * *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*compa * *compb);
        else if (pb != NULL)
            return (Symbol *)new Polynomial(*pb * *pa);
    }
    else if (mata != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Matrix(*rationalb * *mata);
        else if (matb != NULL)
            return (Symbol *)new Matrix(*mata * *matb);
    }
    else if (pa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Polynomial(*pa * Polynomial(*rationalb));
        else if (compb != NULL)
            return (Symbol *)new Polynomial(*pa * Polynomial(*compb));
        else if (pb != NULL)
            return (Symbol *)new Polynomial(*pa * *pb);
    }
    return NULL;
}

Symbol *symbolDivide(Symbol *a, Symbol *b)
{
    Rational *rationala = dynamic_cast<Rational *>(a);
    Rational *rationalb = dynamic_cast<Rational *>(b);
    Complex *compa = dynamic_cast<Complex *>(a);
    Complex *compb = dynamic_cast<Complex *>(b);
    Polynomial *pa = dynamic_cast<Polynomial *>(a);
    if (rationala != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Rational(*rationala / *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*rationala / *compb);
    }
    else if (compa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Complex(*compa / *rationalb);
        else if (compb != NULL)
            return (Symbol *)new Complex(*compa / *compb);
    }
    else if (pa != NULL)
    {
        if (rationalb != NULL)
            return (Symbol *)new Polynomial(*pa * (Rational("1") / *rationalb));
        else if (compb != NULL)
            return (Symbol *)new Polynomial(*pa * (Complex(Rational("1"), Rational("1")) / *compb));
    }
    return NULL;
}

Symbol *symbolModulo(Symbol *a, Symbol *b)
{
    Rational *rationala = dynamic_cast<Rational *>(a);
    Rational *rationalb = dynamic_cast<Rational *>(b);
    if (rationala != NULL && rationalb != NULL)
        return (Symbol *)new Rational(*rationala % *rationalb);
    return NULL;
}

Symbol *symbolPower(Symbol *a, Symbol *b)
{
    Rational *rationalb = dynamic_cast<Rational *>(b);
    Rational *rationala = dynamic_cast<Rational *>(a);
    Complex *compa = dynamic_cast<Complex *>(a);
    Matrix *mata = dynamic_cast<Matrix *>(a);
    Polynomial *pa = dynamic_cast<Polynomial *>(a);
    Number power = rationalb->getNumerator();
    if (!(rationalb->getDenominator() == Number("1")))
    {
        power = power / rationalb->getDenominator();
    }
    if (rationala != NULL)
        return (Symbol *)new Rational(*rationala ^ power);
    if (compa != NULL)
        return (Symbol *)new Complex(*compa ^ power);
    if (mata != NULL)
        return (Symbol *)new Matrix(*mata ^ power);
    if (pa != NULL)
        return (Symbol *)new Polynomial(*pa ^ power);
    return NULL;
}

Symbol *matrixMultiply(Symbol *a, Symbol *b)
{
    Matrix *mata = dynamic_cast<Matrix *>(a);
    Matrix *matb = dynamic_cast<Matrix *>(b);
    return (Symbol *)new Matrix(*mata ^ *matb);
}

Symbol *Expression::evaluate() const
{
    stack<Symbol *> stk;
    for (int idx = 0; idx < (int)this->_rpn.size(); idx++)
    {
        if (this->_rpn[idx]->getType() == 0)
        {
            string type = this->_rpn[idx]->getName();
            if (!type.compare("+"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canAdd(a->getType(), b->getType()))
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                try
                {
                    stk.push(symbolAdd(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(7, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else if (type.compare("-") == 0)
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canSubtract(a->getType(), b->getType()))
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                try
                {
                    stk.push(symbolSubtract(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(7, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else if (!type.compare("*"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canMultiply(a->getType(), b->getType()))
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                try
                {
                    stk.push(symbolMultiply(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(7, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else if (!type.compare("/"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canDivide(a->getType(), b->getType()))
                {
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                }
                try
                {
                    stk.push(symbolDivide(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(4, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else if (!type.compare("%"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canMod(a->getType(), b->getType()))
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                try
                {
                    stk.push(symbolModulo(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(4, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else if (!type.compare("^"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (!Computer::init().canPow(a->getType(), b->getType()) || !(dynamic_cast<Rational *>(b)->isNumber()))
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), 5);
                stk.push(symbolPower(a, b));
            }
            else if (!type.compare("**"))
            {
                if (stk.size() < 2)
                    throw InvalidExpression(0, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                Symbol *b = stk.top();
                stk.pop();
                Symbol *a = stk.top();
                stk.pop();
                if (a->getType() != MATRIX || b->getType() != MATRIX)
                    throw InvalidExpression(1, this->_rpn[idx]->getTokenIndex(), this->_tokens, a->getType(), b->getType());
                try
                {
                    stk.push(matrixMultiply(a, b));
                }
                catch (const std::exception &e)
                {
                    throw InvalidExpression(7, this->_rpn[idx]->getTokenIndex(), this->_tokens);
                }
            }
            else
                stk.push(this->_rpn[idx]);
        }
        else if (this->_rpn[idx]->getType() == 5)
        {
            if (this->_rpn[idx]->getName().compare("sqrt") == 0)
            {
                if (stk.size() < 1)
                    throw InvalidExpression(2, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Symbol *s = stk.top();
                stk.pop();
                if (!dynamic_cast<Rational *>(s))
                    throw InvalidExpression(5, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                stk.push(new Complex(dynamic_cast<Rational *>(s)->getRoot()));
            }
            else if (this->_rpn[idx]->getName().compare("abs") == 0)
            {
                if (stk.size() < 1)
                    throw InvalidExpression(2, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Symbol *s = stk.top();
                stk.pop();
                if (!dynamic_cast<Rational *>(s))
                    throw InvalidExpression(5, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                stk.push(new Rational(dynamic_cast<Rational *>(s)->abs()));
            }
            else if (this->_rpn[idx]->getName().compare("factorial") == 0)
            {
                if (stk.size() < 1)
                    throw InvalidExpression(2, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Symbol *s = stk.top();
                stk.pop();
                if (!dynamic_cast<Rational *>(s))
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Rational tmp = *(dynamic_cast<Rational *>(s));
                Number gg = gcd(tmp.getNumerator(), tmp.getDenominator());
                if (dynamic_cast<Rational *>(s)->getDenominator() != gg)
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                gg = tmp.getNumerator() / gg;
                Number res = Number("1");
                Number iter = Number("1");
                while (iter <= gg)
                {
                    res = res * iter;
                    iter = iter + Number("1");
                }
                stk.push(new Rational(res, Number("1")));
            }
            else if (this->_rpn[idx]->getName().compare("gcd") == 0)
            {
                if (stk.size() < 3)
                    throw InvalidExpression(2, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Symbol *s = stk.top();
                stk.pop();
                stk.pop();
                Symbol *ss = stk.top();
                stk.pop();
                if (!dynamic_cast<Rational *>(s))
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                if (!dynamic_cast<Rational *>(ss))
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Rational tmp = *(dynamic_cast<Rational *>(s));
                Number gg = gcd(tmp.getNumerator(), tmp.getDenominator());
                if (dynamic_cast<Rational *>(s)->getDenominator() != gg)
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                Rational tmp2 = *(dynamic_cast<Rational *>(ss));
                Number gg2 = gcd(tmp2.getNumerator(), tmp2.getDenominator());
                if (dynamic_cast<Rational *>(ss)->getDenominator() != gg)
                    throw InvalidExpression(6, this->_rpn[idx]->getTokenIndex(), this->_tokens, 1, (stk.size() + 1) / 2);
                gg = tmp.getNumerator() / gg;
                gg2 = tmp2.getNumerator() / gg2;
                Number res = gcd(gg, gg2);
                stk.push(new Rational(res, Number("1")));
            }

            else
            {
                Function *func = dynamic_cast<Function *>(this->_rpn[idx]);
                vector<Symbol *> variables;
                while (stk.size() > 0 && (int)variables.size() < func->getVariablesCount())
                {
                    variables.push_back(stk.top());
                    stk.pop();
                    if (stk.size() > 0 && stk.top()->getName()[0] == ',')
                        stk.pop();
                }
                if ((int)variables.size() < func->getVariablesCount())
                    throw InvalidExpression(2, this->_rpn[idx]->getTokenIndex(), this->_tokens, func->getVariablesCount(), variables.size());
                reverse(variables.begin(), variables.end());
                try
                {
                    stk.push((*func)(variables));
                }
                catch (...)
                {
                    throw InvalidExpression(3, -1, this->_tokens);
                }
            }
        }
        else
        {
            stk.push(this->_rpn[idx]);
        }
    }
    if (stk.size() != 1)
    {
        throw InvalidExpression(3, -1, this->_tokens);
    }
    return stk.top();
}

Symbol *Expression::tokenToSymbol(string const &token)
{
    if (token.length() == 1 && isOperator(token[0]))
        return new Symbol(OPERATOR, priority(token[0]), token);
    try
    {
        Rational *rational = new Rational(token);
        return rational;
    }
    catch (...)
    {
    }
    if (token.compare("i") == 0 || token.compare("I") == 0)
    {
        return (Symbol *)(new Complex(Rational("0"), Rational("1")));
    }
    try
    {
        Matrix *matrix = new Matrix(token);
        return (Symbol *)matrix;
    }
    catch (...)
    {
    }
    if (Computer::init().isDeclaredFunction(token))
    {
        return Computer::init().getFunctionSymbol(token);
    }
    bool isPlaceHolder = false;
    for (string tmp : this->placeHolders)
        if (toLower(tmp) == toLower(token))
        {
            isPlaceHolder = true;
        }
    if (!isPlaceHolder && Computer::init().isDeclaredVariable(token))
    {
        return Computer::init().getVariableSymbol(token);
    }
    for (int idx = 0; idx < (int)token.length(); idx++)
        if (!((token[idx] >= 'a' && token[idx] <= 'z') ||
              (token[idx] >= 'A' && token[idx] <= 'Z')))
        {
            throw BadTokenException(token);
        }
    this->unkowns.insert(token);
    return (Symbol *)(new Polynomial(token));
}

vector<string> Expression::breakDown(string const &token) const
{
    vector<string> out;
    int numPrefix = 0;
    if (token[0] < '0' || token[0] > '9')
    {
        out.push_back(token);
        return out;
    }
    while ((token[numPrefix] >= '0' && token[numPrefix] <= '9') ||
           token[numPrefix] == '.')
        numPrefix++;
    out.push_back(token.substr(0, numPrefix));
    if (numPrefix < (int)token.length())
        out.push_back(token.substr(numPrefix));
    return out;
}

bool Expression::validChar(char const c) const
{
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    if (c >= '0' && c <= '9')
        return true;
    for (int idx = 0; idx < (int)validChars.length(); idx++)
        if (c == validChars[idx])
            return true;
    return false;
}
bool Expression::isOperator(char const c) const
{
    for (int idx = 0; idx < (int)operators.length(); idx++)
        if (c == operators[idx])
            return true;
    return false;
}

Expression::Expression(Expression const &src)
{
    *this = src;
}
Expression::~Expression()
{
}

vector<Symbol *> Expression::getSymbols() const
{
    return this->_symbols;
}

Expression &Expression::operator=(Expression const &rhs)
{
    this->_symbols = rhs._symbols;
    this->_rpn = rhs._rpn;
    this->placeHolders = rhs.placeHolders;
    this->unkowns = rhs.unkowns;
    this->_tokens = rhs._tokens;
    return *this;
}

void Expression::toRPN()
{
    stack<Symbol *> stk;
    for (int idx = 0; idx < (int)this->_symbols.size(); idx++)
    {
        switch (this->_symbols[idx]->getType())
        {
        case OPERATOR:
            if (this->_symbols[idx]->getName()[0] == ',')
                this->_rpn.push_back(this->_symbols[idx]);
            else if (this->_symbols[idx]->getPriority() < 4)
            {
                if (stk.size() == 0)
                    stk.push(this->_symbols[idx]);
                else
                {
                    while (stk.size() > 0 && (stk.top()->getPriority() >= this->_symbols[idx]->getPriority()))
                    {
                        if (stk.top()->getPriority() >= 4)
                            break;
                        this->_rpn.push_back(stk.top());
                        stk.pop();
                    }
                    stk.push(this->_symbols[idx]);
                }
            }
            else if (this->_symbols[idx]->getName()[0] == '(')
                stk.push(this->_symbols[idx]);
            else if (this->_symbols[idx]->getName()[0] == ')')
            {
                while (stk.size() > 0 && stk.top()->getName()[0] != '(')
                {
                    this->_rpn.push_back(stk.top());
                    stk.pop();
                }
                if (stk.size() == 0)
                    throw BadParenthesisException();
                else
                {
                    stk.pop();
                    if (stk.size() > 0 && stk.top()->getType() == FUNCTION)
                    {
                        this->_rpn.push_back(stk.top());
                        stk.pop();
                    }
                }
            }
            break;
        case VALUE:
            this->_rpn.push_back(this->_symbols[idx]);
            break;
        case COMPLEX:
            this->_rpn.push_back(this->_symbols[idx]);
            break;
        case MATRIX:
            this->_rpn.push_back(this->_symbols[idx]);
            break;
        case POLYNOMIAL:
            this->_rpn.push_back(this->_symbols[idx]);
            break;
        case FUNCTION:
            stk.push(this->_symbols[idx]);
            break;
        }
    }
    while (stk.size() > 0)
    {
        if (stk.top()->getName()[0] == '(')
            throw BadParenthesisException();
        this->_rpn.push_back(stk.top());
        stk.pop();
    }
}

ostream &operator<<(ostream &o, const Expression &rhs)
{
    int i;

    i = 0;
    for (Symbol *s : rhs._symbols)
    {
        if (i != 0)
            o << " ";
        Rational *r = dynamic_cast<Rational *>(s);
        Complex *c = dynamic_cast<Complex *>(s);
        Matrix *m = dynamic_cast<Matrix *>(s);
        Function *f = dynamic_cast<Function *>(s);
        Polynomial *p = dynamic_cast<Polynomial *>(s);
        if (r != NULL)
            o << *r;
        else if (c != NULL)
            o << *c;
        else if (m != NULL)
            o << *m;
        else if (p != NULL)
            o << *p;
        else if (f != NULL)
        {
            o << f->getFName();
            i = -1;
        }
        else
            o << s->getName();
        i++;
    }
    return o;
}
