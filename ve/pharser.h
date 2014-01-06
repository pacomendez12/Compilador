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
#include "tipos.h"

#define GLOBAL "global"
#define FUNCION 0
#define DESDE 1
#define SI 2
#define CASO 3
#define GLOB 4


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
    Simbolo g_sim;
    Simbolo g_sim_uni;
    Locales g_loc;
    list<Locales *> g_locales;
    list<string> g_nombresSimbolos;
    list<Simbolo> g_variables;
    string g_string_global;
    string g_string_global1;
    string g_tipo;
    string g_e1;
    string g_op;
    string g_e2;
    bool g_binario;
    bool g_fcall;
    bool g_fpar;
    int g_nPar;
    int g_dondeEstoy;
    bool g_regresa_flag;
    bool g_interrumpe_flag;
    bool g_continua_flag;

    /*metodos*/
    void prog();
    void vars();
    /**Auxiliar para no repetir código
    en para agregar elementos a la
    tabla de símbolos*/
    void varsAux();
    void cons();
    /**Auxiliar para las constantes*/
    void consAux();
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


    void static mostrarErrorTipos(string,string,string,string);
    bool esTipoDeDato(string c);
    string representaTipoDato(const string);
    string expecificaTipoDato(const string);

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

    bool existeSimbolo(string);
    std::unordered_map<string,Simbolo>::iterator dameSimboloDeTabla(string);

    struct sigaction act;
};


#endif // PHARSER_H
