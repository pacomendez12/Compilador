#include "TablaDeSimbolos.h"

string
Simbolo::formateaSimbolo(){
    string aux;
    aux = nombre+",";
    aux += clase+",";
    aux += tipo+",";
    aux += dimen1+",";
    aux += dimen2+",";

    //para los locales
    if(!aLocales.empty()){
        list<Locales>::iterator it = aLocales.begin();
        for(;it!=aLocales.end();it++){
            aux += it->clase+",";
            aux += it->tipo+",";
            aux += it->dimen1+",";
            aux += it->dimen2+",";
            aux += it->procp+",";
        }
    }
    aux += "#,";
    return aux;
}