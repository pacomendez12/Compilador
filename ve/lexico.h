/** \file lexico.h
 * \brief Definición del analizador léxico.
 *
 * \name ve compiler
 *
 * Aquí se define la clase léxico que realiza el analisis
 * léxico del archivo que se especifíca.
 * \date Oct - 2013
 * \author Francisco de Jesús Méndez Pelayo
 * \copyright GNU Public License.
 * \dontinclude lexico.h
 */

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



using namespace std;

/**
*Class que define el análisis léxico para el compilador
*de Go (ve)
*
*@author Francisco de Jesús Méndez Pelayo
*@version 0.0.1
*/
class lexico{
public:
    /**Constructor: recibe el nombre del archivo
    (incluyendo la ruta) desde donde se analizará
    *@param string nombre del archivo incluyendo ruta
    */
    lexico(string nombre);
    ~lexico();
    /**Devuelve el siguiente token válido a considerar
    *@return parTokenLexema * Un apuntador a un objeto parTokenLexema
    */
    parTokenLexema * siguienteToken();

    /** Lee el archivo origen y estable el nuevo token encontrado, así como su tipo
     * \warning No es recomendable utilizar este método directamente, en su lugar se
     * recomienda siguienteToken()
     * \see sToken_
     * \see sLexema_
     * \return bool true si se pudo obtener un token válido, false en caso contrario.
     */
    bool analiza();

    /*setters getters*/
    /** Indica la cantidad de lineas que se obtuvieron
     * del archivo origen
     * @return int la cantidad de lineas
     */
    int getLineas();

    /*comodines*/
    error errores[5];
    int contErrores;
    bool errorComentario;

private:
    string sArchivoEntrada_;
    string sLineaLeida_;
    string sLineaLeidaAnterior_;
    string sLexema_; /**< Elemento que guarda el Lexema obtenido*/
    string sToken_; /**< Elemento que guarda el Token obtenido*/
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
