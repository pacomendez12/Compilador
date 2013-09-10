#ifndef LEXICO_H
#define LEXICO_H
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

/*definiciones de constantes*/
#define ERR -1
#define ACP 1000


using namespace std;

class lexico{
public:
    /*el constructor recibe el nombre del archivo
    (incluyendo la ruta) desde donde se analizará*/
    lexico(string nombre);
    ~lexico();
    /*devuelve el siguiente token válido a considerar*/
    string siguienteToken();


private:
    string archivoEntrada_;
    string lexema_;
    ifstream flujoDeEntrada_;
    int linea_, columna_;


};
#endif // LEXICO_H
