#include <iostream>
#include "lexico.h"


int main()
{
    /*aqui comienza la ejecución del compilador*/
    lexico * lex;

    string archivoEntrada;
    cout << "ingresa el nombre del archivo a compilar: ";
    cin >>archivoEntrada;
    lex = new lexico(archivoEntrada);
    archivoEntrada = lex->siguienteToken();

    return 0;
}
