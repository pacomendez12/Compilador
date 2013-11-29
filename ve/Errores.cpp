#include "Errores.h"


errores::errores()
{
    cntErr = 0;
}

void
errores::nuevoError(int linea,int columna,
                    string errorE,string descripcion,
                    string lineaDeError)
{
    if(cntErr < CONT_ERRORES)
    {
        err[cntErr].linea = linea;
        err[cntErr].columna = columna;
        err[cntErr].error = errorE;
        err[cntErr].descripcion = descripcion;
        err[cntErr].lineaDeError = lineaDeError;
        cntErr++;
    }
}

error *
errores::getError(int idx)
{
    return (idx <= cntErr)? &err[idx]:NULL;
}

int
errores::getCantErrores()
{
    return cntErr;
}
