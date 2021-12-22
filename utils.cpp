#include "utils.hpp"

string removeWhiteSpace(string const &str)
{
    string tmp;
    for (int i = 0; i < (int)str.length(); i++)
        if (!isWhiteSpace(str[i]))
            tmp += str[i];
    return tmp;
}
vector<string> splitString(string const &str, char sep)
{
    vector<string> out;
    string tmp;
    for (int idx = 0; idx < (int)str.length(); idx++)
    {
        if (str[idx] == sep)
        {
            out.push_back(tmp);
            tmp.clear();
            continue;
        }
        tmp += str[idx];
    }
    out.push_back(tmp);
    return out;
}
void trim(string &str)
{
    int i = 0;
    while(i < (int)str.length() - 1 && str[i] == '0')
        i++;
    str = str.substr(i, str.length());
}

bool isWhiteSpace(char c)
{
    if (c >= 9 && c <= 13)
        return true;
    if (c == ' ')
        return true;
    return false;
}

string trimWhitSpace(const string &str)
{
    int i;
    int j;

    i = 0;
    j = str.length()-1;
    while(i < (int)str.length() && isWhiteSpace(str[i]))
        i++;
    while(j >= 0 && isWhiteSpace(str[j]))
        j--;
    return str.substr(i,j-i+1); 
}

bool            isAlpha(string const &src)
{
    for (char c : src)
        if (!(c >= 'a' && c <='z') &&
            !(c >= 'A' && c <= 'Z'))
            return false;
    return true;
}

string  toLower(string const &src)
{
    string out;
    for (int idx = 0; idx < (int)src.size(); idx++)
    {
        if (src[idx] >= 'A' && src[idx] <= 'Z')
            out += src[idx] + 'a'-'A';
        else
            out += src[idx];
    }
    return out;
}

int priority(char c)
{
    string  firstOrder   =   "-+";
    string  secondOrder  =   "/*%";
    string  thirdOrder   =   "^";
    string  forthOrder   =   "()";
    if (firstOrder.find(c) != string::npos)
        return 1;
    if (secondOrder.find(c) != string::npos)
        return 2;
    if (thirdOrder.find(c) != string::npos)
        return 3;
    if (forthOrder.find(c) != string::npos)
        return 4;
    return -1;
}

int getCount(string const &str, char c)
{
    int out = 0;
    for (int idx = 0; idx < (int)str.length(); idx++)
        if (str[idx] == c)
            out++;
    return out;
}

string  highlightError(string tmp, char c, int mode)
{
    string underline;
    if (mode == 0 || c == 1)
    {
        if (c == 0)
        {
            underline = string(tmp.length(), ' ');
            underline += "$^$";
        }
        else if (c == 1)
        {
            for (int idx = 0; idx < (int)tmp.length(); idx++)
            {
                if (idx == mode)
                    underline += "$^$";
                else
                    underline += " ";
            }
        }
        else
        {
            for (int idx = 0; idx < (int)tmp.length(); idx++)
            {
                if (tmp[idx] == c)
                    underline += "$^$";
                else
                    underline += " ";
            }
        }
    }
    else if (mode == 1)
    {
        for (int idx = 0; idx < (int)tmp.length(); idx++)
        {
            if (tmp[idx] < 'A' || tmp[idx] > 'z' || (tmp[idx] > 'Z' && tmp[idx] < 'a'))
                underline += "$^$";
            else
                underline += " ";
        }
    }
    else if (mode == 2)
    {
        if (tmp[0] == ',')
            underline += "$^$";
        else
        {
            for (int idx = 0; idx < (int)tmp.length() - 1; idx++)
            {
                if (tmp[idx] == ',' && tmp[idx + 1] == ',')
                {
                    underline += "$^$";
                    break;
                }
                else
                    underline += " ";
            }
            if (underline.find("^") == string::npos && tmp.back() == ',')
                underline += "$^$";
        }
    }
    if (mode == 1)
        return tmp;
    return tmp + '\n' + underline;
}