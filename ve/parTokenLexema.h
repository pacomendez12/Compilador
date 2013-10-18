#ifndef PARTOKENLEXEMA_H
#define PARTOKENLEXEMA_H

#include <string>

using namespace std;

class parTokenLexema{
public:
    parTokenLexema(string,string);
    string getToken();
    string getLexema();

private:
    string sToken_;
    string sLexema_;
};

#endif // PARTOKENLEXEMA_H
