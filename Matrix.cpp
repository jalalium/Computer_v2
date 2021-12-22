#include "Matrix.hpp"
#include "Expression.hpp"
const char* Matrix::BadMatrixException::what() const throw()
{
    return "BAD";
}
const char* Matrix::BadMatrixOperationException::what() const throw()
{
    return "BAD";
}
const char* Matrix::NegativePowerException::what() const throw()
{
    return "BAD";
}
Matrix::Matrix()
{
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
    this->_col = 0;
    this->_row = 0;
}
vector<string> getRows(string rows)
{
    vector<string> out;
    string tmp;
    for (int i = 0; i < (int)rows.length(); i++)
    {
        if (rows[i] == ';')
        {
            out.push_back(tmp);
            tmp.clear();
        }
        else
            tmp += rows[i];
    }
    out.push_back(tmp);
    return out;
}

vector<string> getEntries(string rows)
{
    vector<string> out;
    string tmp;
    for (int i = 0; i < (int)rows.length(); i++)
    {
        if (rows[i] == ',')
        {
            out.push_back(tmp);
            tmp.clear();
        }
        else
            tmp += rows[i];
    }
    out.push_back(tmp);
    return out;
}

Matrix::Matrix(string const &input)
{ 
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
    string noWhiteSpace = removeWhiteSpace(input);
    if (noWhiteSpace[0] != '[' || noWhiteSpace[noWhiteSpace.length()-1] != ']')
        throw BadMatrixException();
    vector<string> rows = getRows(noWhiteSpace.substr(1,noWhiteSpace.length()-2));
    int columns = -1;
    for (int i = 0; i < (int)rows.size(); i++)
    {
        if (rows[i].length() <= 2)
            throw BadMatrixException();
        if (rows[i][0] != '[' || rows[i].back() != ']')
            throw BadMatrixException();
        vector<string> entries = getEntries(rows[i].substr(1,rows[i].length()-2));
        if (columns == -1)
            columns = entries.size();
        if (columns != (int)entries.size())
            throw BadMatrixException();
        for (int j = 0; j < (int)entries.size(); j++)
        {
            try
            {
                Expression entry = Expression(entries[j]);
                Symbol *res = entry.evaluate();
                Rational *val = dynamic_cast<Rational *>(res);
                if (val == NULL)
                    throw BadMatrixException();
                this->_matrix.push_back(*val);
            }
            catch (std::exception &e)
            {
                throw BadMatrixException();
            }
        }
    }
    this->_col = columns;
    this->_row = rows.size();
}

void Matrix::multiplyBy(Rational rational)
{
    for (int idx = 0; idx < (int)this->_matrix.size(); idx++)
        this->_matrix[idx] = this->_matrix[idx] * rational;
}

Matrix::Matrix(int row, int column, vector<Rational> &inp) : _row(row), _col(column), _matrix(inp)
{
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
}
Matrix::Matrix(int row, int column)
{
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
    vector<Rational> matrix(row * column, Rational("0"));
    this->_row = row;
    this->_col = column;
    this->_matrix = matrix;
}
Matrix::Matrix(Matrix const &src)
{
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
    *this = src;
}
Matrix::~Matrix()
{
}

Matrix &Matrix::operator=(Matrix const &rhs)
{
    Symbol::_type = MATRIX;
    Symbol::_priority = 0;
    this->_col = rhs._col;
    this->_row = rhs._row;
    this->_matrix = rhs._matrix;
    return *this;
}

Matrix Matrix::operator+(Matrix const &rhs)
{
    if (this->_col != rhs._col || this->_row != rhs._row)
        throw BadMatrixOperationException();
    Matrix sum = Matrix(this->_row, this->_col);
    for (int i = 0; i < this->_col; i++)
        for (int j = 0; j < this->_row; j++)
            sum._matrix[j * this->_col + i] = this->_matrix[j * this->_col + i] + rhs._matrix[j * this->_col + i];
    return sum;
}

Matrix Matrix::operator-(Matrix const &rhs)
{
    if (this->_col != rhs._col || this->_row != rhs._row)
        throw BadMatrixOperationException();
    Matrix sum = Matrix(this->_row, this->_col);
    for (int i = 0; i < this->_col; i++)
        for (int j = 0; j < this->_row; j++)
            sum._matrix[j * this->_col + i] = this->_matrix[j * this->_col + i] - rhs._matrix[j * this->_col + i];
    return sum;
}

Matrix Matrix::operator^(Matrix const &rhs)
{
    if (this->_col != rhs._row)
        throw BadMatrixOperationException();
    Matrix prod = Matrix(this->_row, rhs._col);
    for (int j = 0; j < this->_row; j++)
        for (int i = 0; i < rhs._col; i++)
            for (int k = 0; k < this->_col; k++)
                prod._matrix[j * rhs._col + i] = prod._matrix[j * rhs._col + i] + this->_matrix[j * this->_col + k] * rhs._matrix[k * rhs._col + i];
    return prod;
}
Matrix Matrix::operator*(Matrix const &rhs)
{
    if (this->_col != rhs._col || this->_row != rhs._row)
        throw BadMatrixOperationException();
    Matrix prod = Matrix(this->_row, rhs._col);
    for (int j = 0; j < this->_row; j++)
        for (int i = 0; i < rhs._col; i++)
            prod._matrix[j * rhs._col + i] = this->_matrix[j * this->_col + i] * rhs._matrix[j * rhs._col + i];
    return prod;
}

Matrix Matrix::getIdentity()
{
    vector<Rational> mat(this->_col * this->_row, Rational("0"));
    for (int i = 0; i < this->_row; i++)
        for (int j = 0; j < this->_col; j++)
            if (i == j)
                mat[i * this->_col + j] = Rational("1");
    return Matrix(this->_col, this->_row, mat);
}

Matrix Matrix::operator^(Number power)
{
    if (Number("0") > power)
        throw NegativePowerException();
    if (this->_col != this->_row)
        throw NegativePowerException();
    Matrix res = this->getIdentity();
    Matrix tmp(*this);
    while (power > Number("0"))
    {
        if ((power.getValue()[power.getValue().length() - 1] - '0') % 2 == 1)
            res = res ^ tmp;
        tmp = tmp ^ tmp;
        power = power / Number("2");
    }
    return res;
}

int Matrix::getRow() const
{
    return this->_row;
}
int Matrix::getCol() const
{
    return this->_col;
}

vector<Rational> Matrix::getMatrix() const
{
    return this->_matrix;
}

ostream &operator<<(ostream &o, Matrix const &rhs)
{
    wstring mat;
    vector<int> maxEntryLength(rhs.getCol(), 0);
    for (int row = 0; row < rhs.getRow(); row++)
        for (int col = 0; col < rhs.getCol(); col++)
        {
            int len = 0;
            for (wchar_t wc : rhs.getMatrix()[row * rhs.getCol() + col].getApproximation().first)
                if (wc < 127)
                    len++;
            maxEntryLength[col] = max(maxEntryLength[col], len + 1);
        }
    for (int row = 0; row < rhs.getRow(); row++)
    {
        mat += L"\u2503";
        for (int col = 0; col < rhs.getCol(); col++)
        {
            wstring padding;
            int len = 0;
            for (wchar_t wc : rhs.getMatrix()[row * rhs.getCol() + col].getApproximation().first)
                if (wc < 127)
                    len++;
            while ((int)padding.length() + len < maxEntryLength[col])
                padding += ' ';
            mat += padding;
            mat += rhs.getMatrix()[row * rhs.getCol() + col].getApproximation().first;
            if (col != rhs.getCol() - 1)
                mat += L" ,";
        }
        mat += L"\u2503";
        if (row != rhs.getRow() - 1)
            mat += '\n';
    }
    wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> convert;
    o << convert.to_bytes(mat);
    return o;
}