#include "tipos.h"
#define ERR "ERR"

string
TablaTipos::validaTipos(string e1,string op,string e2 ,void (*err)(string,string,string,string)){
    if(e1 == "I" || op == "I" || e2 == "I"){
        if(err!=NULL){
            string a = (e1=="I")?"primer":"segundo";
            err(e1,op,e2,"El "+a+" elemento de la expresion es indeterminado");
        }
        return ERR;
    }
    if(e1 != "" && op == "" && e2 == "")
        return e1;
    else
    if(e1 != "" && op != "" && e2==""){
        /*error indicando el segundo valor*/
        if(err!=NULL)
            err(e1,op,e2,"Falta indicar el segundo elemento de la expresión ");
        return ERR;
    }
    if(op == "+" || op == "*" || (op == "-" && e1 != "")){
        if(e1=="E"){
            if(e2 == "E"){
                return "E";
            }else
            if(e2 == "R"){
                return "R";
            }
        }else
        if(e1 == "R"){
            if(e2 == "R"){
                return "R";
            }else
            if(e2 == "R"){
                return "R";
            }
        }else
        if(e1 == "A" && op == "+"){
            if(e2 == "A"){
                return "A";
            }else
            if(e2 == "C"){
                return "A";
            }
        }else
        if(e1 == "C" && op == "+"){
            if(e2 == "A"){
                return "A";
            }else
            if(e2 == "C"){
                return "A";
            }
        }
    }else
    if(op == "/" || op == "^"){
        if(e1 == "E"){
            if(e2 == "E"){
                return "R";
            }else
            if(e2 == "R"){
                return "R";
            }
        }else
        if(e1 == "R"){
            if(e2 == "E"){
                return "R";
            }else
            if(e2 == "R"){
                return "R";
            }
        }
    }else
    if(op == ":=" || op == "="){
        if(e1 == "E"){
            if(e2 == "E"){
                return "";
            }
        }else
        if(e1 == "R"){
            if(e2 == "E"){
                return "";
            }else
            if(e2 == "R"){
                return "";
            }
        }else
        if(e1 == "A"){
            if(e2 == "A"){
                return "";
            }else
            if(e2 == "C"){
                return "";
            }
        }else
        if(e1 == "C"){
            if(e2 == "C"){
                return "";
            }
        }else
        if(e1 == "L"){
            if(e2 == "L"){
                return "";
            }
        }
    }else
    if(op == ">" || op == "<" || op == ">=" ||
       op == "<=" || op == "!=" || op == "=="){
        if(e1 == "E"){
            if(e2 == "E"){
                return "L";
            }else
            if(e2 == "R"){
                return "L";
            }
        }else
        if(e1 == "R"){
            if(e2 == "E"){
                return "L";
            }else
            if(e2 == "R"){
                return "L";
            }
        }else
        if(e1 == "A"){
            if(e2 == "A"){
                return "L";
            }else
            if(e2 == "C"){
                return "L";
            }
        }else
        if(e1 == "C"){
            if(e2 == "C"){
                return "L";
            }else
            if(e2 == "A"){
                return "L";
            }
        }else
        if(e1 == "L" && (op=="==" || op == "!=")){
            if(e2 == "L"){
                return "L";
            }
        }
    }else
    if(op == "!" || op == "||" || op == "&&"){
        if(e1 == "" && op == "!"){
            if(e2 == "L"){
                return "L";
            }
        }else
        if(e1 == "L"){
            if(e2 == "L"){
                return "L";
            }
        }
    }else
    if(op == "-"){
        if(e1 == ""){
            if(e2 == "E"){
                return "E";
            }
        }else
        if(e1 == ""){
            if(e2 == "R"){
                return "R";
            }
        }
    }
    if(err!=NULL)
        err(e1,op,e2,"");
    return ERR;
}
