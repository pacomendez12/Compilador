#ifndef LEXICO_H
#define LEXICO_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cctype>
#include "parTokenLexema.h"
#include "Errores.h"

#define CANTPALRES      34
#define CADENA_VACIA    "#>>##END##<<#"



using namespace std;

class lexico{
public:
    /*el constructor recibe el nombre del archivo
    (incluyendo la ruta) desde donde se analizará*/
    lexico(string nombre);
    ~lexico();
    /*devuelve el siguiente token válido a considerar*/
    parTokenLexema * siguienteToken();
    bool analiza();

    /*setters getters*/
    int getLineas();

    /*comodines*/
    error errores[5];
    int contErrores;
    bool errorComentario;

private:
    string sArchivoEntrada_;
    string sLineaLeida_;
    string sLineaLeidaAnterior_;
    string sLexema_;
    string sToken_;
    ifstream flujoDeEntrada_;
    char c;
    int nLinea_, nColumna_;
    int nIdx_;
    int nEstado_;
    bool bLineaCompleta_; /*indica si la linea leida está
    completa y por tanto no se trata de una linea incompleta*/
    bool beof_;


    /*métodos privados*/
    bool esPalRes(string);
    bool esCteLog(string);
    void leeDeArchivo();
    char sigCaracter();
    char antCaracter();
    int colCar(char, int);
    string Serror(string,string);
};
#endif // LEXICO_H
