/** \file pharser.h
 * \brief Definición de la clase que realiza el análisis sintáctico
 * \date Oct - 2013
 * \author Francisco de Jesús Méndez Pelayo
 * \copyright GNU Public License.
 */

#ifndef PHARSER_H
#define PHARSER_H
#include <unordered_map>
#include <string>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include <list>
#include <stdio.h>
#include "lexico.h"
#include "parTokenLexema.h"
#include "TablaDeSimbolos.h"

#define GLOBAL "global"

class pharser{
private:
    lexico * lex;
    parTokenLexema * ptl;
    unordered_map<string, Simbolo> TablaSimbolos;
    string g_procedimiento;
    Simbolo * g_simboloActual;
    Locales * g_localActual;
    int g_cantDimensiones;
    int g_indiceDimensionActual;


    /*metodos*/
    void prog();
    void vars();
    void cons();
    void funcs();
    void params();
    void pars();
    void block();
    void estatutos();
    void comando();
    void lfunc();
    void asigna();
    void uparam();
    void imprime();
    void lee();
    void expr();
    void dimen();
    void dimenDef();
    void regresa();
    void si();
    void desde();
    void caso();
    void opy();
    void opno();
    void oprel();
    void suma();
    void multi();
    void expo();
    void signo();
    void termino();
    void tlfunc();


    bool esTipoDeDato(string c);
    string representaTipoDato(const string);

    //void segfault_sigaction(int sig);

public:
    /**
     * \param string Nombre del archivo que se compilará
     */
    pharser(string archivo);
    ~pharser();
    /**
     * Método que inicia el análisis sintáctico del código
     */
    void run();
    void setEditor(bool);

    lexico * getLexico();

    void finalizar();

    void imprimeTablaDeSimbolos();

    struct sigaction act;
};


#endif // PHARSER_H
