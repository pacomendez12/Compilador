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
                    else if(ptl->getLexema() == "funcion")
                        funcs();
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

void
pharser::vars()
{
    //ptl=lex->siguienteToken();

    /**bandera que indica si el simbolo encontrado
    es nuevo y por lo tanto no hay ninguna existencia
    de el en la tabla de simbolos.*/
    bool nuevoSimbolo = true;

    bool simboloGlobal = true;
    string s;


    list<Simbolo> variables;
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
                //limpiamos la lista
                variables.clear();
                while(ptl->getToken() == "ide" && !esTipoDeDato(ptl->getLexema()))
                {
                    //reiniciamos la lista de variables
                    Simbolo sim;
                    Locales loc;
                    unordered_map<string,Simbolo>::iterator it;
                    nuevoSimbolo = true;
                    simboloGlobal = true;
                    g_simboloActual = &sim;
                    g_localActual = & loc;
                    g_cantDimensiones = 0;

                    //aqui se agregan a la tabla de simbolos
                    /*------------------------------------------------*/
                    if(g_procedimiento == GLOBAL)
                    {
                        //contexto global
                        bool f = true;
                        list<Simbolo>::iterator i = variables.begin();
                        for(; i!=variables.end(); i++){
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
                        }else{
                            lex->nuevoError("Intento de redefinicion de variable global");
                                finalizar();
                        }
                    } //fin del contexto global
                    else
                    {
                        //cuando el contexto es una funcion

                        simboloGlobal = false;
                        sim.nombre = ptl->getLexema();
                        sim.clase = "L";
                    }//fin del contexto de función
                    /*------------------------------------------------*/

                    //nos servira de refencia para saber si se está llenando
                    //una variable global o local
                    if(simboloGlobal)
                        g_localActual = NULL;
                    else
                        g_simboloActual = NULL;

                    ptl=lex->siguienteToken();
                    s = ptl->getLexema();
                    if(ptl->getLexema() == "[")
                        dimen();
                    else
                    {
                        //variable sin dimensiones
                        if(simboloGlobal || nuevoSimbolo)
                        {
                            g_simboloActual->dimen1 = "0";
                            g_simboloActual->dimen2 = "0";
                        }

                        if(!simboloGlobal)
                        {
                            g_localActual->dimen1 = "0";
                            g_localActual->dimen2 = "0";
                        }
                    }

                    /*agregamos la variable a la lista (solo falta indicar el tipo)*/
                    if(nuevoSimbolo && simboloGlobal)
                        variables.push_back(sim);
                    else if(nuevoSimbolo && !simboloGlobal)
                    {
                        sim.aLocales.push_back(loc);
                        variables.push_back(sim);
                    }
                    else if(!nuevoSimbolo && !simboloGlobal)
                    {
                        it->second.aLocales.push_back(loc);
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
                else
                {
                    //cambiamos el valor del tipo
                    list<Simbolo>::iterator it = variables.begin();
                    for(; it!=variables.end(); it++)
                    {
                        it->tipo = representaTipoDato(ptl->getLexema());
                        TablaSimbolos.insert(std::pair<string,Simbolo>(it->nombre,*it));
                    }
                }

                //obtenemos nuevo lexema
                ptl=lex->siguienteToken();
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
            while(ptl->getToken() == "ide" && !esTipoDeDato(ptl->getLexema()))
            {
                //aqui se agregan a la tabla de simbolos
                ptl=lex->siguienteToken();
                s = ptl->getLexema();
                if(ptl->getLexema() == "[")
                    dimen();
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
            }
            if(!esTipoDeDato(ptl->getLexema()))
                lex->nuevoError("Se esperaba un tipo de dato");

            ptl = lex->siguienteToken();
        }//fin del else principal
    }//fin del while principal
}//fin del método vars

/*falta corregir*/
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
            //multiples declaraciones de tipo
            ptl = lex->siguienteToken();
            while(ptl->getToken() == "ide")
            {
                //aqui se agregan a la tabla de simbolos
                ptl=lex->siguienteToken();
                if(ptl->getLexema() == ":=" || ptl->getLexema() == "=")
                {
                    ptl=lex->siguienteToken();
                    if(ptl->getToken() == "cal" || ptl->getToken() == "cca" ||
                            ptl->getToken() == "dec" || ptl->getToken() == "hex" ||
                            ptl->getToken() == "oct" || ptl->getToken() == "clg" ||
                            ptl->getToken() == "rea")
                        ptl = lex->siguienteToken();
                    else
                    {
                        lex->nuevoError("Esperaba valor constante para declaración de constante");
                    }
                }
                else
                {
                    lex->nuevoError("Esperaba \"=\" para inicializar constante");
                }
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
                ptl = lex->siguienteToken();
                if(ptl->getLexema() == ":=" || ptl->getLexema() == "=")
                {
                    ptl=lex->siguienteToken();
                    if(ptl->getToken() == "cal" || ptl->getToken() == "cca" ||
                            ptl->getToken() == "dec" || ptl->getToken() == "hex" ||
                            ptl->getToken() == "oct" || ptl->getToken() == "clg" ||
                            ptl->getToken() == "rea")
                        ptl = lex->siguienteToken();
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
            sim.clase = "F";
            sim.dimen1 = "0";
            sim.dimen2 = "0";
            /*------------------------------------------------*/

            ptl = lex->siguienteToken();
            params();
            if(esTipoDeDato(ptl->getLexema()))
            {
                sim.tipo = representaTipoDato(ptl->getLexema());
                ptl = lex->siguienteToken();
                block();
            }
            else
            {
                sim.tipo = "I";
                block();
            }

            /*Insertamos el elemento a la tabla*/
            TablaSimbolos.insert(std::pair<string,Simbolo>(sim.nombre,sim));


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
    if(ptl->getLexema() == "(")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() == ")")
            ptl= lex->siguienteToken();
        else
        {
            if(ptl->getToken() == "ide")
            {
                pars();
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
    if(ptl->getToken() == "ide")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() == ",")
        {
            ptl = lex->siguienteToken();
            pars();
        }
        else
        {
            if(esTipoDeDato(ptl->getLexema()))
            {
                ptl = lex->siguienteToken();
                if(ptl->getLexema() == ",")
                {
                    ptl = lex->siguienteToken();
                    pars();
                }
            }
            else
            {
                lex->nuevoError("Esperaba un tipo de dato");
            }
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
    string s = ptl->getLexema();
    if(ptl->getLexema() == "interrumpe" || ptl->getLexema() == "continua")
    {
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
    }
    else if(ptl->getLexema() == "si")
    {
        si();
    }
    else if(ptl->getLexema() == "desde")
    {
        desde();
    }
    else if(ptl->getLexema() == "caso")
    {
        caso();
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
        lex->nuevoError("Comando no válido");
    }

}//fin de comando

void
pharser::asigna()
{
    if(ptl->getLexema() == "[")
        dimen();
    if(ptl->getLexema() == ":=" || ptl->getLexema() == "=")
    {
        ptl = lex->siguienteToken();
        expr();
    }
    else
        lex->nuevoError("Esperaba \":=\" para realizar asignación");
} //fin de asigna

void // posiblemente mal
pharser::lfunc()
{
    if(ptl->getLexema() == "(")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() != ")")
            uparam();
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
}//fin de lfunc

void
pharser::uparam()
{
    expr();
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
pharser::expr()
{
    opy();
    if(ptl->getLexema() == "||")
    {
        ptl = lex->siguienteToken();
        expr();
    }
    /*if(ptl->getToken() == "dec")
        ptl = lex->siguienteToken();
    else
        lex->nuevoError("Expresión inválida");
    string s = ptl->getLexema();*/
} //fin de expr

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
                        ptl = lex->siguienteToken();
                        if(ptl->getLexema() == "[")
                            dimen();
                        if(ptl->getLexema() == ")")
                            ptl = lex->siguienteToken();
                        else
                            lex->nuevoError("Faltó cerrar instrucción \")\"");
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
    if(ptl->getLexema() == "[")
    {
        ptl = lex->siguienteToken();
        expr();
        if(ptl->getLexema() == "]")
        {
            ptl = lex->siguienteToken();
            if(ptl->getLexema() == "[")
                dimen();
        }
        else
            lex->nuevoError("Esperaba \"]\" para cerrar dimension");
    }
    else
        lex->nuevoError("Esperaba \"[\" para indicar dimensiones");
}//fin de dimen

void
pharser::regresa()
{
    if(ptl->getLexema() == "regresa")
    {
        ptl = lex->siguienteToken();
        if(ptl->getToken() != "del")
        {
            expr();
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
        expr();
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
    }
    else
        lex->nuevoError("Esperaba \"para ciclo\"");
}//fin del desde

void
pharser::caso()
{
    if(ptl->getLexema() == "caso")
    {
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
                lex->nuevoError("Esperaba \"{\" para bloque");
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
pharser::opy()
{
    opno();
    if(ptl->getLexema() == "&&")
    {
        ptl = lex->siguienteToken();
        opy();
    }
}//fin de opy


void
pharser::opno()
{
    if(ptl->getLexema() == "!")
        ptl= lex->siguienteToken();
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
        ptl = lex->siguienteToken();
        oprel();
    }
}//fin de oprel

void
pharser::suma()
{
    multi();
    if(ptl->getLexema() == "+" || ptl->getLexema() == "-")
    {
        ptl = lex->siguienteToken();
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
        ptl = lex->siguienteToken();
        multi();
    }
}//fin de multi

void
pharser::expo()
{
    signo();
    if(ptl->getLexema() == "^")
    {
        ptl = lex->siguienteToken();
        expo();
    }
}//fin de expo

void
pharser::signo()
{
    if(ptl->getLexema() == "-")
        ptl=lex->siguienteToken();
    termino();
}//fin de signo

void
pharser::termino()
{
    if(ptl->getLexema() == "(")
    {
        ptl = lex->siguienteToken();
        expr();
        if(ptl->getLexema() == ")")
            ptl = lex->siguienteToken();
        else
            lex->nuevoError("Esperaba \")\" para cerrar expresion");

    }
    else if(ptl->getToken() == "cca" || ptl->getToken() == "cal" ||
            ptl->getToken() == "dec" || ptl->getToken() == "oct" ||
            ptl->getToken() == "hex" || ptl->getToken() == "rea" ||
            lex->esConstanteLogica(ptl->getLexema()))
    {
        ptl = lex->siguienteToken();
    }
    else if(ptl->getToken() == "ide")
    {
        ptl = lex->siguienteToken();
        if(ptl->getLexema() == "(")
            lfunc();
        else if(ptl->getLexema() == "[")
            dimen();
    }
    else
    {
        lex->nuevoError("Token no válido para expresión");
    }
}//fin de termino

/*funciona igual que lfunc ya que la verificacion
del identificador se hace antes de entrar a lfunc o a
tlfunc*/
void
pharser::tlfunc()
{
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
}//fin tlfunc


/*fin del pharser -------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

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
    if(!getLexico()->getEditor())
        cout << endl << "\n\nse leyeron: " << getLexico()->getLineas() << " lineas" << endl;
    getLexico()->imprimeErrores();
    imprimeTablaDeSimbolos();
    exit(EXIT_SUCCESS);
}//fin de finalizar

string
pharser::representaTipoDato(const string d)
{
    if(d == "real")
        return "R";
    if(d == "entero")
        return "E";
    if(d == "logico")
        return "L";
    if(d == "alfabetico")
        return "A";
    if(d == "caracter")
        return "C";
    return "I";
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
