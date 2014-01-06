#ifndef TABLADESIMBOLOS_H
#define TABLADESIMBOLOS_H
#include <list>
#include <string>
using namespace std;

class Locales
{
public:
    /*string nom;*/
    string clase;
    string tipo;
    string dimen1;
    string dimen2;
    string procp;//proceso de pertenencia
    string formateaLocal();
    Locales();
};

class Simbolo
{
public:
    string nombre;
    string nombreFuncion;
    string clase;
    string tipo;
    string dimen1;
    string dimen2;
    list<Locales> aLocales;
    string formateaSimbolo();
    Simbolo();
    string obtenTipo(string proc);//busca el tipo del local que pertenezca al procedimiento indicado
    string obtenClase(string proc);
    string obtenNDimension(string,int);
};
#endif // TABLADESIMBOLOS_H
