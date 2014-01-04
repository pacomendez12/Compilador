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
    string clase;
    string tipo;
    string dimen1;
    string dimen2;
    list<Locales> aLocales;
    string formateaSimbolo();
};
#endif // TABLADESIMBOLOS_H
