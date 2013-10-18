#ifndef ERRORES_H
#define ERRORES_H
#include <string>

struct error{
    int linea;
    int columna;
    string error;
    string descripcion;
    string lineaDeError;
};

#endif // ERRORES_H
