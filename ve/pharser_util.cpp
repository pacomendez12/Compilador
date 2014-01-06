#include "pharser.h"


lexico *
pharser::getLexico()
{
    return lex;
}

bool
pharser::esTipoDeDato(string c)
{
    if(c != "caracter" && c != "entero" &&
            c != "real" && c != "alfabetico" &&
            c != "enteross" && c != "logico")
        return false;
    return true;
}

void
pharser::setEditor(bool e)
{
    getLexico()->setEditor(e);
}

void
pharser::finalizar()
{
    /*veficamos que exista la función principal*/
    if(!existeSimbolo("principal")){
        lex->nuevoError("No ha definido la función \"principal\"");
    }
    if(!getLexico()->getEditor())
        cout << endl << "\n\nse leyeron: " << getLexico()->getLineas() << " lineas" << endl;
    getLexico()->imprimeErrores();
    if(!getLexico()->getEditor())
        imprimeTablaDeSimbolos();
    exit(EXIT_SUCCESS);
}//fin de finalizar

string
pharser::representaTipoDato(const string d)
{
    if(d == "real" || d == "rea")
        return "R";
    if(d == "entero" || d == "dec" ||
            d == "hex" || d == "oct")
        return "E";
    if(d == "logico" || d == "clg")
        return "L";
    if(d == "alfabetico" || d == "cal")
        return "A";
    if(d == "caracter" || d == "cca")
        return "C";
    return "I";
}

string
pharser::expecificaTipoDato(const string d){
    if(d == "R")
        return "real";
    if(d == "E")
        return "entero";
    if(d == "L")
        return "logico";
    if(d == "A")
        return "alfabetico";
    if(d == "C")
        return "caracter";
    return "";
}

void
pharser::imprimeTablaDeSimbolos()
{
    cout << "\n\n************************************************"<<endl;
    cout << "\t\tTabla de Simbolos" << endl;
    cout << "************************************************"<<endl;
    unordered_map<string,Simbolo> tmpMap;
    std::unordered_map<string,Simbolo>::iterator it = TablaSimbolos.begin();
    for(; it!=TablaSimbolos.end(); it++)
    {
        //cout << (it->second).formateaSimbolo()<<endl;
        tmpMap.insert(*it);
    }

    it = tmpMap.begin();
    for(; it!=TablaSimbolos.end(); it++)
    {
        cout << (it->second).formateaSimbolo()<<endl;
    }
}


bool
pharser::existeSimbolo(string n){
    return (TablaSimbolos.find(n) != TablaSimbolos.end())?true:false;
}


std::unordered_map<string,Simbolo>::iterator
pharser::dameSimboloDeTabla(string n){
    return TablaSimbolos.find(n);
}


