#include <iostream>
#include <cstring>
#include <cstdlib>
#include "pharser.h"


/*Constantes de impresión de debug*/
//para ver variagbles con el debug
#define DEBUG



/** \brief Ve 0.0.1
 *
 * Compilador para lenguaje Ve (version 0.0.1)
 */


/*int main()
{
    //aqui comienza la ejecución del compilador
    lexico * lex;
    string archivoEntrada;
    cout << "ingresa el nombre del archivo a compilar: ";
    cin >>archivoEntrada;


    lex = new lexico(archivoEntrada);
    //archivoEntrada = lex->siguienteToken();
    parTokenLexema * ptl;
    cout << "TOKEN\t\t\t\tLEXEMA" << endl;
    cout << "----------------------------------------------" << endl;
    do{
        ptl = lex->siguienteToken();
        if(ptl != NULL){
            //if(ptl->getToken().compare("cos")!=0 && ptl->getToken().compare("com")!=0){
                cout << ptl->getToken() << "\t\t\t\t"<< ptl->getLexema() <<endl;
                //cout << "----------------------------------------------" << endl;
            //}
            //liberación de memoria
            //ptl->~parTokenLexema();
            delete ptl;
        }
    }
    while(ptl != NULL);
    cout << endl << "se leyeron: " << lex->getLineas() << " lineas" << endl;
    lex->imprimeErrores();
    lex->~lexico();
    getchar();
    getchar();
    return EXIT_SUCCESS;
}
*/

int main(int argc, char *argv[])
{
    string archivoEntrada;
    bool editor = false;
    if(argc > 1)
    {
        if(strcmp(argv[1],"-e")==0 || strcmp(argv[1],"-E")==0)
            editor = true;
        if(argc > 2)
            archivoEntrada = argv[2];
        else
        {
            cout << "Error fatal: Archivo de entrada no especificado" << endl;
            exit(EXIT_FAILURE);
        }

    }
    else
    {
        cout << "ingresa el nombre del archivo a compilar: ";
        cin >>archivoEntrada;
    }


    pharser ph(archivoEntrada);
    ph.setEditor(editor);
    ph.run();
    ph.finalizar();
    return 0;
}
