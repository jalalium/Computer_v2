#include "Polynomial.hpp"
#include "Computer.hpp"

Polynomial::Polynomial()
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
}

Polynomial::Polynomial(Complex const &cst)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    vector<pair<string, string> > term;
    term.push_back(make_pair("", "0"));
    this->_form[term] = cst; 
}

Polynomial::Polynomial(Rational const &cst)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    vector<pair<string, string> > term;
    term.push_back(make_pair("", "0"));
    this->_form[term] = Complex(cst, Rational("0")); 
}

void Polynomial::solveFirstDegree() const
{
    Complex b,c;
    cout << "Reduced Form : " << *this << " = 0" << endl;
    Computer::init().history  << "Reduced Form : " << *this << " = 0" << endl;
    for (auto t : this->_form)
    {
        for (auto t2 : t.first)
        {
            if (t2.second.compare("0") == 0)
                c = c + t.second;
            else if (t2.second.compare("1") == 0)
                b = b + t.second;
        }
    }
    if (b.getImag().getNumerator() != Number("0") ||
        c.getImag().getNumerator() != Number("0"))
    {
        cout << "Coefficients are not rational, cannot solve it" << endl;
        Computer::init().history  << "Coefficients are not rational, cannot solve it" << endl;
    }
    else if (b.getReal().getNumerator() == Number("0"))
    {
        if (c.getReal().getNumerator() != Number("0"))
        {
            cout << "Expression is never true" << endl;
            Computer::init().history  << "Expression is never true" << endl;
        }
        else
        {
            cout << "Expression is always true" << endl;
            Computer::init().history  << "Expression is always true" << endl;
        }
    }
    else
    {
        cout << "This equation has one solution s1" << endl;
        Computer::init().history  << "This equation has one solution s1" << endl;
        cout << "s1 = " << ((Rational("-1")*c.getReal())/b.getReal()) << endl;
        Computer::init().history  << "s1 = " << ((Rational("-1")*c.getReal())/b.getReal()) << endl;
    }
    Computer::init().history << endl;
} 

void Polynomial::solveSecondDegree() const
{
    Complex a,b,c;
    cout << "Reduced Form : " << *this << " = 0" << endl;
    for (auto t : this->_form)
    {
        for (auto t2 : t.first)
        {
            if (t2.second.compare("0") == 0)
                c = c + t.second;
            else if (t2.second.compare("1") == 0)
                b = b + t.second;
            else if (t2.second.compare("2") == 0)
                a = a + t.second;
        }
    }
    if (a.getImag().getNumerator() != Number("0") ||
        b.getImag().getNumerator() != Number("0") ||
        c.getImag().getNumerator() != Number("0"))
    {
        cout << "Coefficients are not rational, cannot solve it" << endl;
        Computer::init().history  << "Coefficients are not rational, cannot solve it" << endl;
    }
    else
    {
        Rational delta;
        delta = (b * b - Rational("4") * a * c).getReal();
        if (delta.getNumerator() > Number("0"))
        {
            cout << "Solving Equation" << endl;
            Computer::init().history  << "Solving Equation" << endl;
            cout << "Delta : " << delta << " is positive " << endl;
            Computer::init().history  << "Delta : " << delta << " is positive " << endl;
            cout << "This equation has two solutions s1, s2" << endl;
            Computer::init().history  << "This equation has two solutions s1, s2" << endl;
            int numeratorLength = 0;
            numeratorLength += (Rational("-1")*b.getReal()).getApproximation().second;
            numeratorLength += 7;
            numeratorLength += delta.getApproximation().second; 
            cout << "       " << (Rational("-1")*b.getReal()) << " + " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            Computer::init().history  << "       " << (Rational("-1")*b.getReal()) << " + " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            cout << "s1 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) + delta.getRoot())/(Rational("2")*a))<< endl; 
            Computer::init().history  << "s1 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) + delta.getRoot())/(Rational("2")*a))<< endl; 
            cout << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            Computer::init().history  << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            cout << "       " << (Rational("-1")*b.getReal()) << " - " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            Computer::init().history  << "       " << (Rational("-1")*b.getReal()) << " - " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            cout << "s2 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " "  << (((Rational("-1")*b.getReal()) - delta.getRoot())/(Rational("2")*a))<< endl; 
            Computer::init().history  << "s2 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " "  << (((Rational("-1")*b.getReal()) - delta.getRoot())/(Rational("2")*a))<< endl; 
            cout << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            Computer::init().history << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
        }
        else if (delta.getNumerator() == Number("0"))
        {
            cout << "Solving Equation" << endl;
            Computer::init().history  << "Solving Equation" << endl;
            cout << "Delta : " << delta << " is zero. " << endl;
            Computer::init().history  << "Delta : " << delta << " is zero. " << endl;
            cout << "This equation has one solution s1 with multiplicity 2." << endl;
            Computer::init().history  << "This equation has one solution s1 with multiplicity 2." << endl;
            int numeratorLength = 10;
            cout << "        " << (Rational("-1")*b.getReal()) << endl; 
            Computer::init().history  << "        " << (Rational("-1")*b.getReal()) << endl; 
            cout << "s1 = " << string(numeratorLength, '-') <<  " " << Computer::init().convert.to_bytes(L"\u2248") << " " << ((Rational("-1")*b.getReal())/(Rational("2")*a)).approximation()<< endl; 
            Computer::init().history  << "s1 = " << string(numeratorLength, '-') <<  " " << Computer::init().convert.to_bytes(L"\u2248") << " " << ((Rational("-1")*b.getReal())/(Rational("2")*a)).approximation()<< endl; 
            cout << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            Computer::init().history  << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
        }
        else
        {    
            cout << "Solving Equation" << endl;
            Computer::init().history  << "Solving Equation" << endl;
            cout << "Delta : " << delta << " is negative " << endl;
            Computer::init().history  << "Delta : " << delta << " is negative " << endl;
            cout << "This equation has two solutions s1, s2" << endl;
            Computer::init().history  << "This equation has two solutions s1, s2" << endl;
            int numeratorLength = 0;
            numeratorLength += (Rational("-1")*b.getReal()).getApproximation().second;
            numeratorLength += 7;
            numeratorLength += delta.getApproximation().second; 
            cout << "       " << (Rational("-1")*b.getReal()) << " + " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            Computer::init().history  << "       " << (Rational("-1")*b.getReal()) << " + " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            cout << "s1 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) + delta.getRoot())/(Rational("2")*a)).approximation()<< endl; 
            Computer::init().history  << "s1 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) + delta.getRoot())/(Rational("2")*a)).approximation()<< endl; 
            cout << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            Computer::init().history  << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            cout << "       " << (Rational("-1")*b.getReal()) << " - " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            Computer::init().history  << "       " << (Rational("-1")*b.getReal()) << " - " << Computer::init().displayRoot() << "(" << delta << ")" << endl; 
            cout << "s2 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) - delta.getRoot())/(Rational("2")*a)).approximation()<< endl; 
            Computer::init().history  << "s2 = " << string(numeratorLength, '-') << " " << Computer::init().convert.to_bytes(L"\u2248") << " " << (((Rational("-1")*b.getReal()) - delta.getRoot())/(Rational("2")*a)).approximation()<< endl; 
            cout << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
            Computer::init().history  << "     " << string(numeratorLength/2 - 1,' ') << (Rational("2")*a) << endl;
        }
    }
    Computer::init().history << endl;
}
Polynomial::Polynomial(string token)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    vector<pair<string, string> > term;
    term.push_back(make_pair(token, "1"));
    this->_form[term] = Complex(Rational("1"),Rational("0")); 
}

Polynomial::Polynomial(map< vector< pair <string, string> >, Complex> const & form)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    this->_form = form;
}

Polynomial::Polynomial(Polynomial const &src)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    *this = src;
}

Polynomial::~Polynomial()
{
}

Polynomial &Polynomial::operator=(Polynomial const &rhs)
{
    this->_type = POLYNOMIAL;
    Symbol::_priority = 0;
    this->_form = rhs._form;
    return *this;
}

Polynomial Polynomial::operator+(Polynomial const &rhs)
{
    map< vector< pair <string, string> >, Complex> sum;   
    for (auto t : this->_form)
        sum[t.first] = t.second;
    for (auto t : rhs._form)
        sum[t.first] = sum[t.first] + t.second;
    return Polynomial(sum);
}

Polynomial Polynomial::operator-(Polynomial const &rhs)
{
    map< vector< pair <string, string> >, Complex> diff;   
    for (auto t : this->_form)
        diff[t.first] = t.second;
    for (auto t : rhs._form)
    {
        diff[t.first] = diff[t.first] - t.second;
        if (diff[t.first] == Complex(Rational("0"), Rational("0")))
            diff.erase(t.first);
    }
    return Polynomial(diff);
}

Polynomial Polynomial::operator*(Polynomial const &rhs)
{
    map< vector< pair <string, string> >, Complex> prod;   
    for (auto t : this->_form)
    {
        for (auto t2 : rhs._form)
        {
            Complex coef = t.second * t2.second;
            map<string, string> vars;
            vector<pair<string, string> > unknowns;
            for (pair<string, string> var : t.first)
            {
                vars[var.first] = var.second; 
            }
            for (pair<string, string> var : t2.first)
            {
                if (vars.count(var.first))
                    vars[var.first] = (Number(vars[var.first]) + Number(var.second)).getValue(); 
                else
                    vars[var.first] = var.second; 
            }
            bool hasConstant = false;
            for (auto t : vars)
                if (t.first.length() == 0)
                    hasConstant = true;
            hasConstant &= vars.size() > 1;
            for (auto t : vars)
            {
                if (t.first.length() || !hasConstant)
                unknowns.push_back(make_pair(t.first, t.second));
            }
            if (prod.count(unknowns))
                prod[unknowns] = prod[unknowns] + coef; 
            else
                prod[unknowns] = coef; 
            if (prod[unknowns] == Complex(Rational("0"),Rational("0")))
                prod.erase(unknowns);
        }
    }
    return Polynomial(prod);
}

Polynomial Polynomial::operator^(Number const &exp)
{
    Number idx = Number("1");
    Polynomial res = Polynomial(this->_form);
    if (Number("0") == exp)
        return Polynomial(Rational("1"));
    while (idx < exp)
    {
        res = res * (*this);
        idx = idx + Number("1");
    }
    return Polynomial(res);
}

ostream& operator<<(ostream& o, Polynomial const &rhs)
{
    vector<pair<pair <string, string>, Complex> > terms;
    for (auto t : rhs._form)
    {
        string vars;
        int first = true;
        string mxPower = "";
        for (auto t2 : t.first)
        {
            if (! first)
                vars += Computer::init().convert.to_bytes(L"\u00B7");
            vars += t2.first;
            if (t2.second.compare("1") != 0)
            {
                wstring s;
                for (char c : t2.second)
                    s += Computer::init().getExponent(c - '0');
                vars += Computer::init().convert.to_bytes(s);
            }
            if (t2.second.length() > mxPower.length())
                mxPower = t2.second;
            if (t2.second.length() == mxPower.length() && t2.second > mxPower) 
                mxPower = t2.second;
            first = false;
        }
        terms.push_back(make_pair(make_pair(vars,mxPower), t.second));
    }
    sort(terms.begin(), terms.end(),[](pair<pair <string, string>, Complex> &a,  pair< pair <string, string> , Complex> &b)
                                    {
                                        if (a.first.second.length() < b.first.second.length())
                                            return false;
                                        if (a.first.second.length() > b.first.second.length())
                                            return true;
                                        return a.first.second > b.first.second;
                                    });
    if (terms.size() == 0)
    {
        o << Number("0");
        return o;
    }
    for (int idx = 0; idx < (int)terms.size(); idx++)
    {
        if (terms[idx].first.first.length() == 0 || terms[idx].first.second == "0")
        {
            if (terms[idx].second.getImag() != Rational("0"))
            {
                o << (idx == 0 ? "(" : " + (");
                o << terms[idx].second << ")";
            }
            else if (terms[idx].second.getReal() != Rational("0"))
            {
                o << (idx == 0 ? "" : (terms[idx].second.getReal() < Rational("0") ? " - " : " + "));
                o << (terms[idx].second.getReal()).abs();
            }
        }
        else
        {
            if (terms[idx].second.getImag() != Rational("0"))
            {
                o << (idx == 0 ? "(" : " + (");
                o << terms[idx].second << ")";
            }
            else
            {
                o << (idx == 0 ? (terms[idx].second.getReal() < Rational("0") ? "-" : "") : (terms[idx].second.getReal() < Rational("0") ? " - " : " + "));
                if (terms[idx].second.getReal().abs() != Rational("1"))
                    o << (terms[idx].second.getReal()).abs();
            }
            o << terms[idx].first.first;
        }
    }
    return o;
}