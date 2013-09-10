#include "lexico.h"

/* Constructor de la clase */
lexico::lexico(string nombre){
    archivoEntrada_ = nombre;
    flujoDeEntrada_.exceptions(ifstream::failbit | ifstream::badbit);
    try{
        flujoDeEntrada_.open(archivoEntrada_.c_str(), ifstream::in);
    }catch(ifstream::failure e){
        cerr << "Error intentando abrir el archivo fuente";
        this->~lexico();
        exit(EXIT_FAILURE);
    }

}/*fin del constructor*/

/*destructor*/
lexico::~lexico(){

}/*fin del destructor*/

string lexico::siguienteToken(){
    return NULL;
}/*fin de siguienteToken*/
