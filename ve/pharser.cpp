#include "pharser.h"
pharser * globalPharser;

void
segfault_sigaction(int sig)
{
    globalPharser->getLexico()->nuevoError("Se llegó al fin de archivo y faltó cerrar algun(os) bloque(s)");
    globalPharser->finalizar();
}

pharser::pharser(string archivo)
{
    lex = new lexico(archivo);
    ptl = NULL;
    g_procedimiento = GLOBAL;

    globalPharser = this;
    act.sa_handler = segfault_sigaction;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGSEGV, &act, 0);
}

pharser::~pharser()
{
    if(lex!=NULL)
    {
        delete lex;
    }
    if(ptl!=NULL)
        delete ptl;
}

void
pharser::run()
{
    prog();
}

void pharser::prog()
{
    g_dondeEstoy = GLOB;
    g_interrumpe_flag = false;
    g_continua_flag = false;
    ptl = lex->siguienteToken();
    if(!(ptl->getLexema()=="paquete"))
    {
        lex->nuevoError("Se esperaba \"paquete\"");
    }
    else     //cuando si llegó paquete
    {
        ptl = lex->siguienteToken();
        if(!(ptl->getLexema()=="principal"))
        {
            lex->nuevoError("Se esperaba \"principal\"");
        }
        else     //cuando si llegó principal
        {
            ptl = lex->siguienteToken();
            while(ptl->getLexema() == "importar")
            {
                //para las importaciones
                ptl = lex->siguienteToken();
                if(ptl->getLexema() == "(")
                {
                    //cuando hay varias bibliotecas
                    ptl = lex->siguienteToken();
                    while(ptl->getToken() == "cal" )
                    {
                        //se agregan las bibliotecas ...
                        ptl = lex->siguienteToken();
                        if(ptl->getLexema() != ")" && !lex->HuboSaltoDeLinea())
                        {
                            lex->nuevoError("Esperaba \")\" para terminar de importar bibliotecas o salto de linea para importar una nueva");
                        }

                    }
                    if(ptl->getLexema() != ")" )
                    {
                        lex->nuevoError("Se esperaba \")\" para importar");
                    }
                }
                else
                {
                    //cuando solo hay una biblioteca
                    if(ptl->getToken() != "cal")
                        lex->nuevoError("Esperaba una const. alfanumérica");
                }
                ptl = lex->siguienteToken();
            }
            //para cuando no hay importaciones y una vez que se importó todo
            do
            {
                if(ptl->getLexema() == "var" || ptl->getLexema() == "funcion" ||
                        ptl->getLexema() == "const" || ptl->getLexema() == ";")
                {
                    if(ptl->getLexema() == "var")
                        vars();
                    else if(ptl->getLexema() == "funcion"){
                        int tmp = g_dondeEstoy;
                        g_dondeEstoy = FUNCION;
                        funcs();
                        g_dondeEstoy = tmp;
                    }
                    else if(ptl->getLexema() == "const")
                        cons();
                    else if(ptl->getLexema() == ";")
                        ptl = lex->siguienteToken();
                }
                else
                {
                    lex->nuevoError("Esperaba variables, funciones o constantes");
                    ptl = lex->siguienteToken();
                }
            }
            while(ptl != NULL);
        }
    }

}

/**
* Se encarga de realizar todo el análisis (de forma general)
* de las definiciones de variables, además de agregarlas
* a la tabla de símbolos
*/
void
pharser::varsAux()
{
    /**bandera que indica si el simbolo encontrado
    es nuevo y por lo tanto no hay ninguna existencia
    de el en la tabla de simbolos.*/
    bool nuevoSimbolo = true;

    bool simboloGlobal = true;
    string s;
    /*Lista todas la variables declaradas en una declaración multiple*/
    list<Simbolo> variables;
    list<string> nombresSimbolos;
    list<Locales *> locales;
    //limpiamos la lista
    variables.clear();
    nombresSimbolos.clear();
    locales.clear();
    nombresSimbolos.clear();
    while(ptl->getToken() == "ide" && !esTipoDeDato(ptl->getLexema()))
    {
        //aqui se agregan a la tabla de simbolos
        //reiniciamos la lista de variables
        g_localActual = NULL;
        g_simboloActual = NULL;
        Simbolo sim;
        Locales loc;
        unordered_map<string,Simbolo>::iterator it;
        nuevoSimbolo = true;
        simboloGlobal = true;
        g_simboloActual = &sim;
        g_localActual = &loc;
        g_cantDimensiones = 0;

        //aqui se agregan a la tabla de simbolos
        /*------------------------------------------------*/
        if(g_procedimiento == GLOBAL)
        {
            //contexto global
            //buscamos en la definicion actual que no esté repetida
            bool f = true;
            list<Simbolo>::iterator i = variables.begin();
            for(; i!=variables.end(); i++)
            {
                if(i->nombre == ptl->getLexema())
                    f = false;
            }


            if(f==true && (it=TablaSimbolos.find(ptl->getLexema())) == TablaSimbolos.end())
            {
                //cuando la variable no existe en ningun contexto
                sim.nombre = ptl->getLexema();
                sim.clase = "V";

                //variables.push_back(sim);
            }
            else if(f == true)
            {
                //cuando la variable ya existe en otro contexto
                if((it->second).clase=="I" && (it->second).tipo=="I")
                {
                    it->second.clase = "V";
                    //cambiamos el apuntado de g_simboloActual al elemento
                    //del iterador, el cual sólo se debe modificar
                    g_simboloActual = &(it->second);
                    //para indicar que sólo se modificará un símbolo existente
                    nuevoSimbolo = false;
                }
                else
                {
                    //ERROR:redefiniendo variable
                    lex->nuevoError("Intento de redefinicion de variable global");
                    finalizar();
                }
            }
            else
            {
                lex->nuevoError("Intento de redefinicion de variable");
                finalizar();
            }
        } //fin del contexto global
        else
        {
            //cuando el contexto es una funcion
            simboloGlobal = false;
            sim.nombre = ptl->getLexema();
            loc.procp = g_procedimiento;

            //buscamos en la definicion actual que no esté repetida
            bool f = true;
            list<Simbolo>::iterator i = variables.begin();
            for(; i!=variables.end(); i++)
            {
                if(i->nombre == ptl->getLexema())
                    f = false;
            }


            if(f==true && (it=TablaSimbolos.find(ptl->getLexema())) == TablaSimbolos.end())
            {
                //cuando la variable no existe en ningun contexto
                sim.clase = "I";
            }
            else if(f == true)
            {
                //cuando la variable ya existe en otro contexto
                /**la clase del elemento debe ser 'v' que quiere decir que
                éxclusivamente la variable existe en un contexto global*/
                bool banderaVariableLocal = true;
                list<Locales>::iterator i = (it->second).aLocales.begin();
                for(; i!=(it->second).aLocales.end(); i++)
                {
                    if(i->procp==g_procedimiento)
                        banderaVariableLocal = false;
                }

                //if((it->second).clase=="V" && (it->second).tipo!="I")
                if(banderaVariableLocal && (it->second).clase=="V")
                {
                    it->second.clase = "V";
                    //cambiamos el apuntado de g_simboloActual al elemento
                    //del iterador, el cual sólo se debe modificar
                    g_simboloActual = &(it->second);
                    //para indicar que sólo se modificará un símbolo existente
                    nuevoSimbolo = false;
                }
                else if(banderaVariableLocal)
                {
                    it->second.clase = "I";
                    g_simboloActual = &(it->second);
                    nuevoSimbolo = false;
                }
                else
                {
                    //ERROR:redefiniendo variable
                    lex->nuevoError("Intento de redefinicion de variable local");
                    finalizar();
                }

            }
        }//fin del contexto de función
        /*------------------------------------------------*/

        ptl=lex->siguienteToken();
        s = ptl->getLexema();

        //variable sin dimensiones
        if(simboloGlobal && nuevoSimbolo)
        {
            g_simboloActual->dimen1 = "0";
            g_simboloActual->dimen2 = "0";
        }
        else if(nuevoSimbolo)
        {
            sim.dimen1 = "0";
            sim.dimen2 = "0";
        }
        if(!simboloGlobal)
        {
            g_localActual->dimen1 = "0";
            g_localActual->dimen2 = "0";
            g_localActual->clase = "L";
        }

        //nos servira de refencia para saber si se está llenando
        //una variable global o local
        if(simboloGlobal)//si no es una función, el local se setea a null
            g_localActual = NULL;
        else
            g_simboloActual = NULL;
        if(ptl->getLexema() == "[")
        {
            //cuando la variable si está dimensionada
            g_cantDimensiones = 0;
            dimenDef();
        }

        /*agregamos la variable a la lista (solo falta indicar el tipo)*/
        if(nuevoSimbolo && simboloGlobal)
        {
            TablaSimbolos.insert(std::pair<string,Simbolo>(sim.nombre,sim));
            nombresSimbolos.push_back(sim.nombre);
        }
        else if(nuevoSimbolo && !simboloGlobal)
        {
            Locales * aploc;
            loc.tipo="";
            sim.aLocales.push_back(loc);
            string nom = sim.nombre;
            nombresSimbolos.push_back(nom);
            TablaSimbolos.insert(std::pair<string,Simbolo>(nom,sim));
            Simbolo * simb = &(TablaSimbolos.find(sim.nombre)->second);
            aploc = &(simb->aLocales.back());
            locales.push_back(aploc);
        }
        else if(!nuevoSimbolo && !simboloGlobal)
        {
            it->second.aLocales.push_back(loc);
            g_localActual = &(it->second.aLocales.back());
            locales.push_back(g_localActual);
        }


        s = ptl->getLexema();
        if(ptl->getLexema() != "," && !esTipoDeDato(ptl->getLexema()))
            lex->nuevoError("Esperaba una \",\" o tipo de dato");
        else
        {
            if(!esTipoDeDato(ptl->getLexema()))
            {
                ptl=lex->siguienteToken();
                s = ptl->getLexema();
                if(esTipoDeDato(ptl->getLexema()))
                {
                    lex->nuevoError("Se esperaba identificador");
                }
            }
        }
    }//fin del while que agrega variables del mismo tipo

    if(!esTipoDeDato(ptl->getLexema()))
        lex->nuevoError("Se esperaba un tipo de dato");
    string tipoDeDato = ptl->getLexema();
    //cambiamos el valor del tipo
    if(simboloGlobal)
    {
        //list<Simbolo>::iterator i = variables.begin();
        list<string>::iterator i = nombresSimbolos.begin();
        for(; i!=nombresSimbolos.end(); i++)
        {
            TablaSimbolos.find(*i)->second.tipo = representaTipoDato(tipoDeDato);
            //i->tipo = representaTipoDato(tipoDeDato);
            //TablaSimbolos.insert(std::pair<string,Simbolo>(i->nombre,*i));
        }
    }
    else
    {
        //list<Simbolo>::iterator i = variables.begin();
        list<string>::iterator i = nombresSimbolos.begin();
        for(; i!=nombresSimbolos.end(); i++)
        {
            TablaSimbolos.find(*i)->second.tipo = "I";
        }

        list<Locales *>::iterator ii = locales.begin();

        for(; ii!=locales.end(); ii++)
        {
            (*ii)->tipo = representaTipoDato(tipoDeDato);
        }
    }

    //obtenemos nuevo lexema
    ptl=lex->siguienteToken();
}

void
pharser::vars()
{
    while(ptl->getLexema() == "var")
    {
        ptl=lex->siguienteToken();
        if(ptl->getLexema() == "(")
        {
            //multiples declaraciones de tipo

            ptl=lex->siguienteToken();
            while((ptl->getLexema() != ")" && ptl->getToken() != "del" &&
                    ptl->getToken() != "plr") || ptl->getToken() =="ide")
            {
                /*IMPORTANTE LLAMADA*/
                varsAux();
#ifdef DEBUG
                string s = ptl->getLexema();
                bool a = lex->HuboSaltoDeLinea();
#endif
                if(ptl->getLexema() != ")" && !lex->HuboSaltoDeLinea())
                {
                    lex->nuevoError("Esperaba \")\" para terminar declaración de variables o salto de linea para un nuevo tipo");
                }
            }


            if((ptl->getToken() == "del" && ptl->getLexema() != ")") || ptl->getToken() == "plr")
            {
                lex->nuevoError("Esperaba \")\" para cerrar declaración");
            }
            else
                ptl = lex->siguienteToken();

        }
        else
        {
            //declaraciones de un sólo tipo
            /*IMPORTANTE LLAMADA*/
            varsAux();
        }//fin del else principal
    }//fin del while principal
}//fin del método vars


/**
* Auxiliar que realiza el análisis de las constantes
* y agrega el símbolo a la tabla de símbolos
*/
void
pharser::consAux()
{
    Simbolo sim;
    sim.nombre = ptl->getLexema();
    sim.clase = "C";
    sim.dimen1 = "0";
    sim.dimen2 = "0";
    ptl = lex->siguienteToken();
    if(ptl->getLexema() == ":=" || ptl->getLexema() == "=")
    {
        ptl=lex->siguienteToken();
        if(ptl->getToken() == "cal" || ptl->getToken() == "cca" ||
                ptl->getToken() == "dec" || ptl->getToken() == "hex" ||
                ptl->getToken() == "oct" || ptl->getToken() == "clg" ||
                ptl->getToken() == "rea")
        {
            sim.tipo = representaTipoDato(ptl->getToken());
            TablaSimbolos.insert(std::pair<string,Simbolo>(sim.nombre,sim));
            ptl = lex->siguienteToken();
        }
        else
        {
            lex->nuevoError("Esperaba valor constante para declaración de constante");
        }
    }
    else
    {
        lex->nuevoError("Esperaba \"=\" para inicializar constante");
    }
}

void
pharser::cons()
{
    //ptl=lex->siguienteToken();
    string s;
    while(ptl->getLexema() == "const")
    {
        ptl=lex->siguienteToken();
        if(ptl->getLexema() == "(")
        {
            /**multiples declaraciones de tipo*/
            ptl = lex->siguienteToken();
            while(ptl->getToken() == "ide")
            {
                //aqui se agregan a la tabla de simbolos
                consAux();
                if(ptl->getLexema() == ",")
                    ptl = lex->siguienteToken();

                if(ptl->getLexema() != ")" && !lex->HuboSaltoDeLinea())
                {
                    lex->nuevoError("Esperaba \")\" para terminar declaración de constante o salto de linea para una nueva declaración");
                }
            }
            if(ptl->getLexema() != ")")
            {
                lex->nuevoError("Esperaba \")\" para cerrar declaración de constante");
            }
            else
                ptl = lex->siguienteToken();

        }
        else
        {
            //declaraciones de un sólo tipo
            //aqui se agregan a la tabla de simbolos
            //ptl=lex->siguienteToken();
            if(ptl->getToken() == "ide")
            {
                consAux();
            }
            else
            {
                lex->nuevoError("Esperaba un identificador");
            }
            //ptl = lex->siguienteToken();
        }//fin del else principal
    }//fin del while principal
}//fin del método cons


void
pharser::funcs()
{
    Simbolo sim;
    string nombrecompleto ="";
    g_string_global = "";
    g_cantDimensiones = 0;
    g_regresa_flag = false;
    if(ptl->getLexema() == "funcion")
    {
        ptl = lex->siguienteToken();
        if(ptl->getToken() != "ide" && ptl->getLexema() != "principal")
            lex->nuevoError("Se esperaba un ident. para la funcion");
        else
        {
            /*aqui se implementa la talba de simbolos para las funciones*/
            /*Guardamos el nombre de la funcion*/
            /*------------------------------------------------*/
            g_procedimiento = ptl->getLexema();
            sim.nombre = ptl->getLexema();
            nombrecompleto = ptl->getLexema();
            sim.clase = "F";
            sim.dimen1 = "0";
            sim.dimen2 = "0";
            /*------------------------------------------------*/

            ptl = lex->siguienteToken();
            params();
            nombrecompleto+=g_string_global;
            sim.nombreFuncion = nombrecompleto;
            if(esTipoDeDato(ptl->getLexema()))
            {
                sim.tipo = representaTipoDato(ptl->getLexema());
                ptl = lex->siguienteToken();
            }
            else
            {
                sim.tipo = "I";
            }


            if((TablaSimbolos.find(sim.nombre)) == TablaSimbolos.end())
            {
                TablaSimbolos.insert(std::pair<string,Simbolo>(sim.nombre,sim));
            }
            else
            {
                lex->nuevoError("No se puede redefinir la función");
                finalizar();
            }

            block();

            if(!g_regresa_flag && dameSimboloDeTabla(g_procedimiento)->second.tipo!="I"){
                lex->nuevoError("Falta indicar \"regresa\" para la funcion "+g_procedimiento);
            }
        }
    }
    else
    {
        lex->nuevoError("Se esperaba \"funcion\"");
    }
    g_procedimiento = GLOBAL;
}//fin de funcs

void
pharser::params()
{
    g_cantDimensiones = 0;
    if(ptl->getLexema() == "(")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() == ")")
            ptl= lex->siguienteToken();
        else
        {
            if(ptl->getToken() == "ide")
            {
                g_variables.clear();
                g_nombresSimbolos.clear();
                g_locales.clear();
                pars();
                if(esTipoDeDato(ptl->getLexema()))
                {
                    string tipoDeDato = ptl->getLexema();
                    list<string>::iterator i = g_nombresSimbolos.begin();
                    for(; i!=g_nombresSimbolos.end(); i++)
                    {
                        TablaSimbolos.find(*i)->second.tipo = "I";
                    }

                    list<Locales *>::iterator ii = g_locales.begin();

                    for(; ii!=g_locales.end(); ii++)
                    {
                        (*ii)->tipo = representaTipoDato(tipoDeDato);
                        g_string_global+= "$"+representaTipoDato(tipoDeDato);
                    }
                    ptl = lex->siguienteToken();
                    if(ptl->getLexema() == ",")
                    {
                        ptl = lex->siguienteToken();
                        g_locales.clear();
                        g_nombresSimbolos.clear();
                        g_variables.clear();
                        pars();
                        if(esTipoDeDato(ptl->getLexema()))
                            {
                                string tipoDeDato = ptl->getLexema();
                                list<string>::iterator i = g_nombresSimbolos.begin();
                                for(; i!=g_nombresSimbolos.end(); i++)
                                {
                                    TablaSimbolos.find(*i)->second.tipo = "I";
                                }

                                list<Locales *>::iterator ii = g_locales.begin();

                                for(; ii!=g_locales.end(); ii++)
                                {
                                    (*ii)->tipo = representaTipoDato(tipoDeDato);
                                    g_string_global+= "$"+representaTipoDato(tipoDeDato);
                                }
                                ptl = lex->siguienteToken();
                                if(ptl->getLexema() == ",")
                                {
                                    g_locales.clear();
                                    g_nombresSimbolos.clear();
                                    g_variables.clear();
                                    pars();
                                }
                            }
                    }
                }
                else
                {
                    lex->nuevoError("Esperaba un tipo de dato o una \",\"");
                }
                if(ptl->getLexema() != ")")
                {
                    lex->nuevoError("Falta \")\" para los parámetros");
                }
                else
                    ptl = lex->siguienteToken();
            }
            else
            {
                lex->nuevoError("Se esperaba un identificador");
            }
        }
    }
    else
    {
        lex->nuevoError("Se esperaba el caracter \"(\"");
    }
}//fin params


void
pharser::pars()
{
    Simbolo sim;
    Locales loc;
    g_cantDimensiones = 0;
    unordered_map<string,Simbolo>::iterator it;
    bool nuevoSimbolo = true;
    if(ptl->getToken() == "ide")
    {
        //cuando el contexto es una funcion
        sim.nombre = ptl->getLexema();
        sim.clase = "I";
        sim.tipo = "I";
        sim.dimen1 = "0";
        sim.dimen2 = "0";
        loc.procp = g_procedimiento;
        loc.clase = "P";
        loc.dimen1 = "0";
        loc.dimen2 = "0";

        //buscamos en la definicion actual que no esté repetida
        bool f = true;
        list<Simbolo>::iterator i = g_variables.begin();
        for(; i!=g_variables.end(); i++)
        {
            if(i->nombre == ptl->getLexema())
                f = false;
        }
        it=TablaSimbolos.find(ptl->getLexema());
        if(it == TablaSimbolos.end());
        else if(f == true)
        {
            //cuando la variable ya existe en otro contexto
            /**la clase del elemento debe ser 'v' que quiere decir que
            éxclusivamente la variable existe en un contexto global*/
            bool banderaVariableLocal = true;
            list<Locales>::iterator i = (it->second).aLocales.begin();
            for(; i!=(it->second).aLocales.end(); i++)
            {
                if(i->procp==g_procedimiento)
                    banderaVariableLocal = false;
            }

            if(banderaVariableLocal && (it->second).clase=="V")
            {
                //cambiamos el apuntado de g_simboloActual al elemento
                //del iterador, el cual sólo se debe modificar
                g_simboloActual = &(it->second);
                //para indicar que sólo se modificará un símbolo existente
                nuevoSimbolo = false;
            }
            else if(banderaVariableLocal)
            {
                it->second.clase = "I";
                g_simboloActual = &(it->second);
                nuevoSimbolo = false;
            }
            else
            {
                //ERROR:redefiniendo variable
                lex->nuevoError("Intento de redefinicion de variable local o un parámetro");
                finalizar();
            }
        }
        else
        {
            lex->nuevoError("Intento de redefinicion de variable");
            finalizar();
        }


        ptl = lex->siguienteToken();
        if(nuevoSimbolo)
        {
            Locales * aploc;
            loc.tipo="";
            sim.aLocales.push_back(loc);
            string nom = sim.nombre;
            g_nombresSimbolos.push_back(nom);
            TablaSimbolos.insert(std::pair<string,Simbolo>(nom,sim));
            Simbolo * simb = &(TablaSimbolos.find(sim.nombre)->second);
            aploc = &(simb->aLocales.back());
            g_locales.push_back(aploc);
        }
        else
        {
            it->second.aLocales.push_back(loc);
            g_localActual = &(it->second.aLocales.back());
            g_locales.push_back(g_localActual);
        }

        if(ptl->getLexema() == ",")
        {
            ptl = lex->siguienteToken();
            pars();
        }



    }
    else
    {
        lex->nuevoError("Se esperaba un identificador");
    }
}//fin pars

void
pharser::block()
{
    if(ptl->getLexema() == "{")
    {
        do
        {
            if(ptl->getLexema() == "funcion")
            {
                lex->nuevoError("Falta \"}\" para cerrar bloque");
                return;//para evitar continuar con el bloque
            }
            if(ptl->getLexema() == "{")
                //if(ptl->getLexema() != "var")
                /*para cuando hay una declaracion
                de variables dada despues de una declaración de estatutos, se verifica
                que no sea el lexema var*/
                ptl = lex->siguienteToken();
            if(ptl->getLexema() == "valor")
            {
                ptl=lex->siguienteToken(); // evitamos que se cicle en casos mal formados
            }
            if(ptl->getLexema() == "var")
            {
                vars();
                string s = ptl->getLexema();
                if(ptl->getLexema() != ";" && ptl->getLexema() != "}")
                    lex->nuevoError("Esperaba \";\"");
            }
            else
            {
                estatutos();
            }
        }
        while(ptl->getLexema() != "}");
        ptl = lex->siguienteToken();
    }
    else
    {
        lex->nuevoError("Esperaba \"{\" para inicio de bloque");
    }
}//fin block

void
pharser::estatutos()
{
    g_cantDimensiones = 0;
    if(ptl->getLexema() != "}" &&
            ptl->getLexema() != "valor" && ptl->getLexema() != "defecto")
    {
        comando();
        //cout << "\nlex: " << ptl->getLexema() << endl;
        if(ptl->getLexema() == "var")//para sacar de estatutos una declaracion posterior
            return;
        if(ptl->getLexema() == ";")
        {
            ptl = lex->siguienteToken();
            estatutos();
        }
        else if(lex->tokenAnterior()->getLexema() != "}" &&
                ptl->getLexema() != "}")/*para corregir cuando no hay ;
        despues de un bloque*/
            lex->nuevoError("Esperaba \";\" para fin de linea");
    }
    else
    {
        //cout << "\nllego: " << ptl->getLexema() << endl;
    }
}//fin de estatutos

//no terminada
void
pharser::comando()
{
    g_cantDimensiones = 0;
    string s = ptl->getLexema();
    if(ptl->getLexema() == "interrumpe" || ptl->getLexema() == "continua")
    {
        /*if(ptl->getLexema() == "interrumpe" && g_dondeEstoy!=DESDE && g_dondeEstoy!=CASO){
            lex->nuevoError("Sólo se permite utilizar \"interrumpe\" dentro de un ciclo o un caso");
        }else if(ptl->getLexema() == "continua" && g_dondeEstoy!= DESDE){
            lex->nuevoError("Sólo se permite utilizar \"continua\" dentro de un ciclo");
        }*/

        if(ptl->getLexema() == "interrumpe" && !g_interrumpe_flag){
            lex->nuevoError("Sólo se permite utilizar \"interrumpe\" dentro de un ciclo o un caso");
        }else if(ptl->getLexema() == "continua" && !g_continua_flag){
            lex->nuevoError("Sólo se permite utilizar \"continua\" dentro de un ciclo");
        }

        ptl = lex->siguienteToken();
    }
    else if(ptl->getLexema() == "fmt")
    {
        imprime();
    }
    else if(ptl->getLexema() == "con")
    {
        lee();
    }
    else if(ptl->getLexema() == "regresa")
    {
        regresa();
        g_regresa_flag = true;
    }
    else if(ptl->getLexema() == "si")
    {
        int tmp=g_dondeEstoy;
        g_dondeEstoy = SI;
        si();
        g_dondeEstoy = tmp;
    }
    else if(ptl->getLexema() == "desde")
    {
        int tmp=g_dondeEstoy;
        g_dondeEstoy = DESDE;
        desde();
        g_dondeEstoy = tmp;
    }
    else if(ptl->getLexema() == "caso")
    {
        int tmp=g_dondeEstoy;
        g_dondeEstoy = CASO;
        caso();
        g_dondeEstoy = tmp;
    }
    else if(ptl->getToken() == "ide" )
    {
        ptl=lex->siguienteToken();
        if(ptl->getLexema() == "(")
        {
            //codigo para llamada a funcion
            lfunc();
        }
        else if(ptl->getLexema() == "[" ||
                ptl->getLexema() == ":=" || ptl->getLexema() == "=")
        {
            //codigo para asignacion
            asigna();
        }
        else  //tal vez variables
            lex->nuevoError("Esperaba \":=\" para asignación");
    }
    else if(ptl->getLexema() == "var");
    else if(ptl->getToken() == "plr")
        lex->nuevoError("Esperaba \"}\"");
    else if(ptl->getLexema() != ";" && ptl->getLexema() != "}")
    {
        ptl = lex->siguienteToken();//para evitar ciclos infinitos
        lex->nuevoError("Comando no válido, revise que haya escrito correctamente el nombre del comando");
        finalizar();
    }

}//fin de comando

void
pharser::asigna()
{
    string nom = lex->tokenAnterior()->getLexema();
    string e1,e2,op;
    parTokenLexema * ant = lex->tokenAnterior();
    unordered_map<string,Simbolo>::iterator it;
    if((it=TablaSimbolos.find(ant->getLexema())) != TablaSimbolos.end()){
        g_sim = it->second;
        list<Locales>::iterator i = (it->second).aLocales.begin();
        bool eslocal = false;
        for(;i!=(it->second).aLocales.end();i++){
            if(i->procp==g_procedimiento){
                eslocal = true;
                break;
            }
        }
        if(eslocal){
            //cuando si existe de forma local
            e1 = it->second.obtenTipo(g_procedimiento);
        }else if(it->second.clase=="I"){
            lex->nuevoError("La variable \""+ant->getLexema()+"\" no existe en este contexto");
        }else{
            //si está definida globalmente
            if(it->second.clase!="C"){
                if(it->second.clase == "F"){
                    lex->nuevoError("El lado izquierdo de una asignacion no puede ser una función");
                }else{
                    //cuando es una variable
                    e1 = it->second.obtenTipo(g_procedimiento);
                }
            }else{
                //si es una constante
                lex->nuevoError("La constante \""+ant->getLexema()+"\" no puede cambiar su valor");
            }
        }

        if(ptl->getLexema() == "["){
            g_cantDimensiones = 0;
            dimen();
        }else{
            if(dameSimboloDeTabla(nom)->second.obtenNDimension(g_procedimiento,1)!="0"){
                lex->nuevoError("La variable fue declarado como arreglo y se intentó acceder a ella como variable normal");
            }
        }
        if(ptl->getLexema() == ":=" || ptl->getLexema() == "=")
        {
            op = ptl->getLexema();
            ptl = lex->siguienteToken();
            g_fcall = true;
            g_fpar = true;
            g_nPar = 0;
            expr();
            e2 = g_tipo;
            //g_tipo = TablaTipos::validaTipos(e1,op,e2,(&mostrarErrorTipos));
            /*Envio como parámetro NULL en el puntero a la función
            para yo indicar un error más pesonalizado*/
            g_tipo = TablaTipos::validaTipos(e1,op,e2,NULL);
            if(g_tipo == "ERR"){
                lex->nuevoError("No se puede asignar un valor del tipo "+expecificaTipoDato(e2)+" a uno "+
                    expecificaTipoDato(e1));
            }
        }
        else
            lex->nuevoError("Esperaba \":=\" para realizar asignación");
    }else{
        lex->nuevoError("La variable \""+ant->getLexema()+"\" no existe en este contexto");
    }
} //fin de asigna

void // posiblemente mal
pharser::lfunc()
{
    string nom = lex->tokenAnterior()->getLexema();
    g_string_global = "";
    g_string_global1 = "";
    Simbolo sim;
    if(existeSimbolo(nom))
        sim = dameSimboloDeTabla(nom)->second;
    if(existeSimbolo(nom) && sim.clase=="F"){
        if(ptl->getLexema() == "(")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() != ")")
            {
                uparam();
            }
            /*Error en los tipos y cantidad de parámetros*/
            if(sim.nombreFuncion != nom+g_string_global){
                lex->nuevoError("No existe una función llamada "+ nom + " que tenga ("+g_string_global1+") como parámetros");
                finalizar();
            }
            if(ptl->getLexema() == ")")
            {
                ptl = lex->siguienteToken();
            }
            else
            {
                lex->nuevoError("Esperaba \")\", llamada a función");
            }
        }
        else
        {
            lex->nuevoError("Esperaba \"(\" para los param.");
        }
    }else{
        lex->nuevoError("No existe la función "+nom+" que se intentó llamar");
    }
}//fin de lfunc

void
pharser::uparam()
{
    string aux1,aux2;
    g_fcall = true;
    g_fpar = true;
    g_nPar = 0;
    aux1 = g_string_global;
    aux2 = g_string_global1;
    expr();
    /*g_string_global += "$"+representaTipoDato(ptl->getToken());
    g_string_global1 += ptl->getToken()+",";*/
    g_string_global = aux1;
    g_string_global1 = aux2;
    g_string_global += "$"+g_tipo;
    g_string_global1 += expecificaTipoDato(ptl->getToken())+",";
    if(ptl->getLexema() == ",")
    {
        ptl = lex->siguienteToken();
        uparam();
    }
}//fin de uparam

void
pharser::imprime()
{
    if(ptl->getLexema() == "fmt")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() == ".")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() == "Imprime" || ptl->getLexema() == "Imprimenl")
            {
                ptl = lex->siguienteToken();
                if(ptl->getLexema() == "(")
                {
                    do
                    {
                        ptl = lex->siguienteToken();
                        g_fcall = true;
                        g_nPar = 0;
                        g_fpar = true;
                        expr();
                    }
                    while(ptl->getLexema() == ",");
                    if(ptl->getLexema() == ")")
                        ptl = lex->siguienteToken();
                    else
                    {
                        ptl = lex->siguienteToken();
                        lex->nuevoError("Esperaba \")\" para terminar imprime");
                    }
                }
                else
                {
                    ptl = lex->siguienteToken();
                    lex->nuevoError("Esperaba \"(\" para indicar expresion");
                }
            }
            else
            {
                lex->nuevoError("Función no válida en \"fmt\"");
                ptl = lex->siguienteToken();
            }
        }
        else
        {
            ptl = lex->siguienteToken();
            lex->nuevoError("Esperaba \".\" para indicar miembro");
        }
    }
}

void
pharser::lee()
{
    if(ptl->getLexema() == "con")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema()==".")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() == "Lee")
            {
                ptl = lex->siguienteToken();
                if(ptl->getLexema() == "(")
                {
                    ptl = lex->siguienteToken();
                    if(ptl->getToken() == "ide")
                    {
                        string nom = ptl->getLexema();
                        unordered_map<string,Simbolo>::iterator it;

                        if((it=TablaSimbolos.find(nom)) != TablaSimbolos.end()){
                            g_sim = it->second;
                            if(g_sim.obtenClase(g_procedimiento) != "C"){
                                ptl = lex->siguienteToken();
                                if(ptl->getLexema() == "["){
                                    g_cantDimensiones = 0;
                                    dimen();
                                }
                                if(ptl->getLexema() == ")")
                                    ptl = lex->siguienteToken();
                                else
                                    lex->nuevoError("Faltó cerrar instrucción \")\"");
                            }else{
                                lex->nuevoError("No se puede almacenar un valor en una constante");
                            }
                        }else{
                            lex->nuevoError("La variable \""+nom+"\" no se ha declarado");
                        }
                    }
                    else
                        lex->nuevoError("Esperaba el nombre de una variable");
                }
                else
                    lex->nuevoError("Esperaba un \"(\" para indicar variable");
            }
            else
                lex->nuevoError("Esperaba un método válido para \"con\"");
        }
        else
        {
            lex->nuevoError("Esperaba \".\" para indicar miembro");
        }
    }
}//fin de lee

void
pharser::dimen()
{
    if(g_cantDimensiones == 0)
        g_sim_uni = g_sim;
    g_cantDimensiones++;
    if(g_cantDimensiones <= 2)
    {
        if(ptl->getLexema() == "[")
        {
            if(g_sim_uni.obtenNDimension(g_procedimiento,g_cantDimensiones) != "0"){
                ptl = lex->siguienteToken();
                if(ptl->getToken()!="del"){
                    g_fcall = true;
                    g_nPar = 0;
                    g_fpar = true;
                    expr();
                    if(g_tipo!="E"){
                        lex->nuevoError("Sólo puede indicar numeros enteros en las dimensiones de un arreglo");
                    }
                    if(ptl->getLexema() == "]")
                    {
                        ptl = lex->siguienteToken();
                        if(ptl->getLexema() == "["){

                            dimen();
                        }else{
                            if(g_sim_uni.obtenNDimension(g_procedimiento,g_cantDimensiones + 1)!="0" && g_cantDimensiones + 1 <= 2){
                                lex->nuevoError("Hacen falta indicar dimensiones para una variable de "+to_string(g_cantDimensiones + 1)+" dimensiones");
                            }
                        }
                    }
                    else{
                        lex->nuevoError("Esperaba \"]\" para cerrar dimension");
                    }
                }else{
                    lex->nuevoError("El valor de la dimensión deber ser entero, puede comprobar que no haya dejado \"[]\"");
                }
            }else{
                lex->nuevoError("Se ha intentado indicar la dimension "+to_string(g_cantDimensiones + 1)+
                    " pero, la variable es de "+to_string(g_cantDimensiones));
            }
        }
        else
            lex->nuevoError("Esperaba \"[\" para indicar dimensiones");
    }
    else
    {
        lex->nuevoError("El máximo de dimensiones a indicar es \"2\"");
    }
}//fin de dimen

void
pharser::dimenDef()
{
    if(g_cantDimensiones < 2)
    {
        string nom = lex->tokenAnterior()->getLexema();
        if(ptl->getLexema() == "[")
        {
           ptl = lex->siguienteToken();
            //expr();
            //aquí solo se aceptan enteros

            if(ptl->getToken() == "dec" || ptl->getToken()=="ide")
            {
                string nom,c,t;
                nom = c = t = "";
                if(ptl->getToken() == "ide"){
                    nom = ptl->getLexema();
                    c = dameSimboloDeTabla(nom)->second.obtenClase(g_procedimiento);
                    t = dameSimboloDeTabla(nom)->second.obtenTipo(g_procedimiento);

                }
                if(ptl->getToken() == "dec" || (c == "C" && t == "E")){
                    if(g_cantDimensiones == 0)
                    {
                        if(g_simboloActual!=NULL)
                        {
                            g_simboloActual->dimen1 = ptl->getLexema();
                        }
                        else
                        {
                            g_localActual->dimen1 = ptl->getLexema();
                        }
                    }
                    else if(g_cantDimensiones == 1)
                    {
                        if(g_simboloActual!=NULL)
                        {
                            g_simboloActual->dimen2 = ptl->getLexema();
                        }
                        else
                        {
                            g_localActual->dimen2 = ptl->getLexema();
                        }
                    }
                    ptl = lex->siguienteToken();
                }else{
                    ptl = lex->siguienteToken();
                    lex->nuevoError("El tamaño de las dimensiones debe ser indicado como un valor entero");
                }
            }
            else
            {
                /*adelanteamos para visualizar el error correctamente*/
                ptl = lex->siguienteToken();
                lex->nuevoError("El tamaño de las dimensiones debe ser indicado como un valor entero");
            }

            if(ptl->getLexema() == "]")
            {
                ptl = lex->siguienteToken();
                g_cantDimensiones++;
                if(ptl->getLexema() == "[")
                    dimenDef();
            }
            else
                lex->nuevoError("Esperaba \"]\" para cerrar dimension");
        }
        else
            lex->nuevoError("Esperaba \"[\" para indicar dimensiones");
    }
    else
    {
        lex->nuevoError("Sólo se pueden utilizar \"2\" dimensiones para un arreglo");
    }
}//fin de dimenDef

void
pharser::regresa()
{
    if(ptl->getLexema() == "regresa")
    {
        string ret = dameSimboloDeTabla(g_procedimiento)->second.tipo;
        ptl = lex->siguienteToken();
        if(ptl->getToken() != "del")
        {
            string tipo;
            g_fcall = true;
            g_fpar = true;
            g_nPar = 0;
            expr();
            tipo = g_tipo;
            if(tipo!=ret){
                lex->nuevoError("No puedes regresar un \""+expecificaTipoDato(tipo)+"\" en una función que regresa \""+
                                expecificaTipoDato(ret)+"\"");
            }
        }else{
            if(ret != "I"){
                lex->nuevoError("No puedes regresar vacío, ya que la funcíon debe regresar "+expecificaTipoDato(ret));
            }
        }
    }
    else
        lex->nuevoError("Esperaba un \"regresa\"");
}

void
pharser::si()
{
    if(ptl->getLexema() == "si")
    {
        ptl = lex->siguienteToken();
        g_fcall = true;
        g_fpar = true;
        g_nPar = 0;
        expr();
        if(g_tipo != "L"){
            lex->nuevoError("La expresión de un \"si\" siempre debe ser de tipo lógica");
        }
        block();
        string s = ptl->getLexema();
        if(ptl->getLexema() == "sino")
        {
            ptl = lex->siguienteToken();
            block();
        }
    }
    else
        lex->nuevoError("Esperaba \"si\" para condicion");
}//fin del si

void
pharser::desde()
{
    if(ptl->getLexema() == "desde")
    {
        g_interrumpe_flag = true;
        g_continua_flag = true;
        ptl = lex->siguienteToken();
        if(ptl->getLexema() != ";")
        {
            do
            {
                if(ptl->getToken() == "ide")
                {
                    ptl = lex->siguienteToken();
                    asigna();
                }
                else
                {
                    lex->nuevoError("Esperaba un idenficador");
                    return;
                }
                if(ptl->getLexema() == ",")
                {
                    ptl = lex->siguienteToken();
                    if(ptl->getLexema() == ";")
                        lex->nuevoError("Caracter \",\" de más");
                }
                else if(ptl->getToken() == "ide")
                    lex->nuevoError("Hace falta \",\" como separador");
            }
            //while(ptl->getLexema() != "{" &&
            //      ptl->getLexema() != "}" && ptl->getLexema() != ";" && //parte agregada
            //      ptl->getToken() != "plr");//parte agregada
            while(ptl->getToken() == "ide");
        }
        //inicialización vacía
        ptl = lex->siguienteToken();
        if(ptl->getLexema() != ";")
        {
            expr();
            if(g_tipo!="L"){
                lex->nuevoError("La expresión de condición de un \"desde\" siempre debe ser de tipo lógica");
            }
        }

        ptl = lex->siguienteToken();

        //expresion vacía
        if(ptl->getLexema() != "{")
        {
            do
            {
                if(ptl->getToken() == "ide")
                {
                    ptl = lex->siguienteToken();
                    asigna();
                }
                else
                {
                    lex->nuevoError("Esperaba un idenficador");
                    return;
                }
                if(ptl->getLexema() == ",")
                {
                    ptl = lex->siguienteToken();
                    if(ptl->getLexema() == "{")
                        lex->nuevoError("Caracter \",\" de más");
                }
                else if(ptl->getToken() == "ide")
                    lex->nuevoError("Hace falta \",\" como separador");
            }
            //while(ptl->getLexema() != "{" &&
            //      ptl->getLexema() != "}" && ptl->getLexema() != ";" && //parte agregada
            //      ptl->getToken() != "plr");//parte agregada
            while(ptl->getToken() == "ide");
        }
        block();
        g_interrumpe_flag = false;
        g_continua_flag = false;
    }
    else
        lex->nuevoError("Esperaba \"para ciclo\"");
}//fin del desde

void
pharser::caso()
{
    if(ptl->getLexema() == "caso")
    {
        g_interrumpe_flag = true;
        ptl = lex->siguienteToken();
        if(ptl->getToken() == "ide")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() == "{")
            {
                ptl = lex->siguienteToken();
                do
                {
                    do
                    {
                        if(ptl->getLexema() == "valor" || ptl->getLexema() == "defecto")
                        {
                            if(ptl->getLexema() == "valor")
                            {
                                ptl = lex->siguienteToken();
                                if(ptl->getToken() == "dec" ||
                                        ptl->getToken() == "cal" || ptl->getToken() == "cca")
                                {
                                    /*debe llegar
                                        un entero, una cadena o un caracter*/
                                    //do nothing
                                }
                                else
                                {
                                    lex->nuevoError("Esperaba: entero, alfabético o caracter");
                                }
                            }
                            //despues de defecto o valor {algo}
                            ptl = lex->siguienteToken();
                            if(ptl->getLexema() == ":")
                            {
                                /*sólo se avanza el lexico para comenzar con
                                los estatutos o para buscar otro valor o defecto*/
                                ptl = lex->siguienteToken();
                            }
                            else
                            {
                                lex->nuevoError("Esperaba \":\" para indicar valor a comparar");
                            }
                        }
                        else
                        {
                            lex->nuevoError("Esperaba \"valor\" o \"defecto\"");

                        }
                    }
                    while(ptl->getLexema() == "valor" || ptl->getLexema() == "defecto");
                    /*ya que se tienen todos los valores, el valore o defecto*/
                    estatutos();
                    if(ptl->getLexema() == "}")
                    {
                        g_interrumpe_flag = false;
                        ptl = lex->siguienteToken();
                    }
                    else if(ptl->getLexema() != "valor" && ptl->getLexema() != "defecto")
                    {
                        lex->nuevoError("Falta \"}\" para cerrar bloque");
                    }
                }
                while(ptl->getLexema() == "valor" || ptl->getLexema() == "defecto");
            }
            else
            {
                lex->nuevoError("Esperaba \"{\" para bloque, no debe llegar una expresión como valor del \"caso\"");
                finalizar();
            }
        }
        else
        {
            lex->nuevoError("Esperaba un identificador");
            ptl = lex->siguienteToken();
            finalizar();
        }
    }
    else
        lex->nuevoError("Esperaba \"caso\" para selectiva multiple");

}//fin del caso


void
pharser::mostrarErrorTipos(string a,string b, string c,string cad){
    if(cad==""){
        globalPharser->lex->nuevoError("La combinación <"+globalPharser->expecificaTipoDato(a)+
        " "+b+" "+globalPharser->expecificaTipoDato(c)+"> no determina un tipo válido");
    }else{
        globalPharser->lex->nuevoError(cad+" <"+globalPharser->expecificaTipoDato(a)+
        " "+b+" "+globalPharser->expecificaTipoDato(c)+">");
    }
    globalPharser->finalizar();
}

/*funciona igual que lfunc ya que la verificacion
del identificador se hace antes de entrar a lfunc o a
tlfunc*/
void
pharser::tlfunc()
{
    parTokenLexema * ant = lex->tokenAnterior();
    string nom = ant->getLexema();
    if(existeSimbolo(nom) && dameSimboloDeTabla(nom)->second.clase=="F"){
        if(ptl->getLexema() == "(")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() != ")")
                uparam();
            else
                ptl = lex->siguienteToken();
        }
        else
        {
            lex->nuevoError("Esperaba \"(\" para indicar parámetros");
        }
    }else{
        lex->nuevoError("No existe la función "+nom);
        finalizar();
    }
}//fin tlfunc


/*fin del pharser -------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/
