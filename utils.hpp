#ifndef UTILS_HPP
# define UTILS_HPP
#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <set>
#include <codecvt>
#include <sstream>

using namespace std;


void            trim(string &str);
string          removeWhiteSpace(string const &str);
vector<string>  splitString(string const &str, char sep);
string          toLower(string const &src);
int             getCount(string const &str, char c);
int             priority(char c);
bool            isAlpha(string const &src);
string          highlightError(string tmp, char c, int mode = 0);
bool            isWhiteSpace(char c);
string          trimWhitSpace(const string &str);

#endif
