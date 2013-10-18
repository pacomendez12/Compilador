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
    /*archivoEntrada = lex->siguienteToken();*/
    parTokenLexema * ptl;
    cout << "TOKEN\t\t\t\tLEXEMA" << endl;
    cout << "----------------------------------------------" << endl;
    do{
        ptl = lex->siguienteToken();
        if(ptl != NULL){
            //if(ptl->getToken().compare("cos")!=0 && ptl->getToken().compare("com")!=0){
                cout << ptl->getToken() << "\t\t\t\t"<< ptl->getLexema() <<endl;
                cout << "----------------------------------------------" << endl;
            //}
            /*liberación de memoria*/
            ptl->~parTokenLexema();
        }
    }
    while(ptl != NULL);
    cout << endl << "se leyeron: " << lex->getLineas() << " lineas" << endl;
    if(lex->contErrores > 0){
        cout << "\n\n\t\t\t\tERRORES" << endl;
        cout << "________________________________________________________________________________" << endl;
        cout << "Linea\tColumna\t\tError\tDescripcion\t\tLinea completa" <<endl;
        cout <<  "________________________________________________________________________________" << endl;
        for(int i = 0 ; i < lex->contErrores; i++){
            cout << lex->errores[i].linea << "\t" <<lex->errores[i].columna << "\t\t" << lex->errores[i].error << "\t" << lex->errores[i].descripcion<<
            "\t" << lex->errores[i].lineaDeError;
        }
        cout <<  "________________________________________________________________________________" << endl;
    }
    lex->~lexico();
    return EXIT_SUCCESS;
}
