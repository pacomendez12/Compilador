#ifndef ERRORES_H
#define ERRORES_H
#include <string>
#include "definiciones.h"

using namespace std;

struct error{
    int linea;
    int columna;
    string error;
    string descripcion;
    string lineaDeError;
};


class errores{
public:
    void nuevoError(int linea,int columna,
                    string error,string descripcion,
                    string lineaDeError);
    error * getError(int idx);
    errores();
    int getCantErrores();

private:
    error err[CONT_ERRORES];
    int cntErr;
};

#endif // ERRORES_H
