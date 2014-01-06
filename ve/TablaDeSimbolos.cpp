#include "TablaDeSimbolos.h"

string
Simbolo::formateaSimbolo(){
    string aux;
    aux = nombre+",";
    //aux += nombreFuncion+",";
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

string
Locales::formateaLocal(){
    string aux = "";
    /*aux = nom+",";*/
    aux += clase+",";
    aux += tipo+",";
    aux += dimen1+",";
    aux += dimen2+",";
    aux += procp+",";
    return aux;
}

Locales::Locales(){
    dimen1 = "0";
    dimen2 = "0";
    clase  = "I";
}

Simbolo::Simbolo(){
    nombre = "";
    nombreFuncion = "";
}

string
Simbolo::obtenTipo(string proc){
    list<Locales>::iterator i = aLocales.begin();
    for(;i!=aLocales.end();i++){
        if(i->procp == proc)
            return i->tipo;
    }
    if(tipo!="I")
        return tipo;
    else
    return "";
}

string
Simbolo::obtenClase(string proc){
    list<Locales>::iterator i = aLocales.begin();
    for(;i!=aLocales.end();i++){
        if(i->procp == proc)
            return i->clase;
    }
    if(tipo!="I")
        return clase;
    else
    return "";
}

string
Simbolo::obtenNDimension(string proc,int ndim){
    list<Locales>::iterator i = aLocales.begin();
    for(;i!=aLocales.end();i++){
        if(i->procp == proc){
            if(ndim == 1)
                return i->dimen1;
            else if(ndim == 2){
                return i-> dimen2;
            }
        }
    }
    if(tipo!="I"){
        if(ndim == 1)
            return dimen1;
        else if(ndim == 2){
            return dimen2;
        }
    }
    return "";
}
