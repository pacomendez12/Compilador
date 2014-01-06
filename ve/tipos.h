#ifndef TIPOS_H
#define TIPOS_H
#include <string>

using namespace std;
class TablaTipos{
public:
    static string validaTipos(string,string,string,void (*err)(string,string,string,string));
};



#endif // TIPOS_H
