#include "pharser.h"

void
pharser::expr()
{
    g_binario = true;
    opy();
    if(ptl->getLexema() == "||")
    {
        g_fcall = false;
        g_op = ptl->getLexema();
        ptl = lex->siguienteToken();
        expr();
        g_e2 = g_tipo;
    }
    /*if(ptl->getToken() == "dec")
        ptl = lex->siguienteToken();
    else
        lex->nuevoError("Expresión inválida");
    string s = ptl->getLexema();*/
} //fin de expr

void
pharser::opy()
{
    opno();
    if(ptl->getLexema() == "&&")
    {
        g_op = ptl->getLexema();
        ptl = lex->siguienteToken();
        g_fcall = false;
        opy();
    }
}//fin de opy


void
pharser::opno()
{
    if(ptl->getLexema() == "!"){
        g_binario = false;
        g_e1 = "";
        g_op = ptl->getLexema();
        ptl= lex->siguienteToken();
    }
    oprel();
}//fin de opno

void
pharser::oprel()
{
    suma();
    if(ptl->getLexema() == "<" || ptl->getLexema() == ">" ||
            ptl->getLexema() == "<=" || ptl->getLexema() == ">=" ||
            ptl->getLexema() == "!=" || ptl->getLexema() == "==")
    {
        g_op = ptl->getLexema();
        g_e1 = g_tipo;
        ptl = lex->siguienteToken();
        g_fcall = false;
        oprel();
    }else
    if(ptl->getLexema()=="=" || ptl->getLexema()==":="){
        ptl = lex->siguienteToken();
        lex->nuevoError("No debe utilizar el operador de asignación para una comparación");
    }
}//fin de oprel

void
pharser::suma()
{
    multi();
    if(ptl->getLexema() == "+" || ptl->getLexema() == "-")
    {
        g_op = ptl->getLexema();
        g_e1 = g_tipo;
        ptl = lex->siguienteToken();
        g_fcall = false;
        suma();
    }
}//fin de suma

void
pharser::multi()
{
    expo();
    if(ptl->getLexema() == "*" || ptl->getLexema() == "/" ||
            ptl->getLexema() == "%")
    {
        g_op = ptl->getLexema();
        ptl = lex->siguienteToken();
        g_fcall = false;
        multi();
    }
}//fin de multi

void
pharser::expo()
{
    signo();
    if(ptl->getLexema() == "^")
    {
        g_op = ptl->getLexema();
        ptl = lex->siguienteToken();
        g_fcall = false;
        expo();
    }
}//fin de expo

void
pharser::signo()
{
    if(ptl->getLexema() == "-"){
        g_binario = false;
        g_e1 = "";
        g_op = ptl->getLexema();
        ptl=lex->siguienteToken();
    }
    termino();
}//fin de signo

void
pharser::termino()
{
    if(ptl->getLexema() == "(")
    {
        g_nPar++;
        string e1,op,e2;
        int n = g_nPar;
        e1=g_e1;
        op=g_op;
        ptl = lex->siguienteToken();
        g_fcall = true;
        expr();
        if(ptl->getLexema() == ")"){
            if(!g_binario && g_fpar && g_nPar == n){
                /*g_e1=e1;*/
                g_op=op;
                g_e2 = g_tipo;
            }else
            if(g_fpar && g_nPar == n){
                g_e1 = g_tipo;
                g_op = "";
                g_e2 = "";
            }else{
                g_e1=e1;
                g_op=op;
                g_e2 = g_tipo;
            }
            g_tipo = TablaTipos::validaTipos(g_e1,g_op,g_e2,(&mostrarErrorTipos));
            g_fpar = false;
            ptl = lex->siguienteToken();
            g_nPar--;
        }
        else
            lex->nuevoError("Esperaba \")\" para cerrar expresion");
    }
    else{
        if(ptl->getToken() == "cca" || ptl->getToken() == "cal" ||
                ptl->getToken() == "dec" || ptl->getToken() == "oct" ||
                ptl->getToken() == "hex" || ptl->getToken() == "rea" ||
                lex->esConstanteLogica(ptl->getLexema()))
        {
            string token = ptl->getToken();
            /*if(lex->esConstanteLogica(ptl->getLexema()))
                token = "clg";*/
            if(!g_binario && g_fcall){
                g_e2 = representaTipoDato(token);
            }else
            if(g_fcall){
                g_e1 = representaTipoDato(token);
                g_op = "";
                g_e2 = "";
            }else{
                g_e2 = representaTipoDato(token);
            }
            g_tipo = TablaTipos::validaTipos(g_e1,g_op,g_e2,(&mostrarErrorTipos));
            ptl = lex->siguienteToken();
        }
        else if(ptl->getToken() == "ide")
        {
            string nom = ptl->getLexema();
            if(existeSimbolo(nom) && dameSimboloDeTabla(nom)->second.obtenTipo(g_procedimiento)!=""){
                g_sim = dameSimboloDeTabla(nom)->second;
                ptl = lex->siguienteToken();
                string cls = dameSimboloDeTabla(nom)->second.clase;
                if(ptl->getLexema() == "("){
                    //
                    string cls = dameSimboloDeTabla(nom)->second.clase;
                    if(cls == "F"){
                        string e1,op,e2;
                        string ti = dameSimboloDeTabla(nom)->second.tipo;
                        if(!g_binario && g_fcall){
                            g_e2 = ti;
                        }else
                        if(g_fcall){
                            g_e1 = ti;
                            g_op = "";
                            g_e2 = "";
                        }else{
                            g_e2 = ti;
                        }
                        e1=g_e1;
                        op=g_op;
                        e2=g_e2;
                        lfunc();
                        g_tipo = TablaTipos::validaTipos(e1,op,e2,(&mostrarErrorTipos));
                        g_e1=e1;
                        g_op=op;
                        g_e2=e2;
                    }else{
                        lex->nuevoError("No existe una función llamada \""+nom+"\" es probable que se trate de una variable, "+
                                        "utilice "+nom+" en vez de "+nom+"()");
                    }
                }
                else{
                    if(cls!="F"){
                        string e1,op,e2;
                        string ti = dameSimboloDeTabla(nom)->second.obtenTipo(g_procedimiento);
                        g_sim = dameSimboloDeTabla(nom)->second;
                        if(!g_binario && g_fcall){
                            g_e2 = ti;
                        }else
                        if(g_fcall){
                            g_e1 = ti;
                            g_op = "";
                            g_e2 = "";
                        }else{
                            g_e2 = ti;
                        }
                        e1=g_e1;
                        op=g_op;
                        e2=g_e2;
                        if(ptl->getLexema() == "["){
                            g_cantDimensiones = 0;
                            dimen();
                        }else{
                            if(dameSimboloDeTabla(nom)->second.obtenNDimension(g_procedimiento,1)!="0"){
                                lex->nuevoError("La variable fue declarado como arreglo y se intentó acceder a ella como variable normal");
                            }
                        }
                        g_e1=e1;
                        g_op=op;
                        g_e2=e2;
                        g_tipo = TablaTipos::validaTipos(g_e1,g_op,g_e2,(&mostrarErrorTipos));
                    }else{
                        lex->nuevoError("Cuando utilice una funcion como termino de una expresion debe indicar los parámetros");
                    }
                }
            }else{
                lex->nuevoError("No existe el identificador "+nom+" que intentó utilizar, verifique que haya sido declarado");
            }
        }else{
            lex->nuevoError("Falta indicar el segundo elemento de la expresión");
        }
    }
    /*else
    {
        lex->nuevoError("Token no válido para expresión");
    }*/
}//fin de termino
