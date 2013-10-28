#include <sstream>
#include "lexico.h"


/*constantes*/
const int TAM_MAX_BUFFER = 2048;
const int ACP = 999;
const int ERR = -1;
/*matrix de transiciones*/

/**
 *@var int Define la matriz de transición para el analizador léxico
 *el cual es definido con el autómata
 */
const int mat_trans[38][24] =
{
    {1,2,6,6,1,1,1,23,23,1,12,12,29,35,17,19,21,15,13,24,26,ACP,ACP,36},
    {1,1,1,1,1,1,1,ACP,ACP,1,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,3,3,ACP,ACP,4,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,3,3,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ERR,5,5,5,ERR,ERR,5,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ACP,5,5,5,ACP,ACP,5,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,6,6,6,ACP,ACP,ACP,ACP,7,9,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ERR,8,8,8,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ACP,8,8,8,ACP,ACP,ACP,ACP,ACP,9,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ERR,11,11,11,ERR,ERR,ERR,ERR,ERR,ERR,10,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ERR,11,11,11,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ACP,11,11,11,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,14,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,16,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,18,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,20,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,22,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,25,24,24,24,24},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,24},
    {27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,28,ERR,ERR,ERR},
    {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,28,ERR,ERR,ERR},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,30,32,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,31,30,30},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {32,32,32,32,32,32,32,32,32,32,32,32,32,33,32,32,32,32,32,32,32,32,32,32},
    {32,32,32,32,32,32,32,32,32,32,32,32,34,32,32,32,32,32,32,32,32,32,32,32},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,37,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP}
};

const string palRes[]= { "interrumpe","valor","canal","const","continua",
                         "defecto","diferir","sino","desde","funcion","ir","ir_a","si",
                         "importar","interfaz","mapa","paquete","rango","regresa",
                         "selecciona","estructura","tipo","variable","caso","principal",
                         "caracter","entero","real","alfanumerico","logico","enteross",
                         "div","mod","var"
                       };

/* Constructor de la clase */
lexico::lexico(string nombre)
{
    sArchivoEntrada_ = nombre;
    flujoDeEntrada_.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        flujoDeEntrada_.open(sArchivoEntrada_.c_str(), ifstream::in);
    }
    catch(ifstream::failure e)
    {
        cerr << "Error intentando abrir el archivo fuente" << endl;
        this->~lexico();
        exit(EXIT_FAILURE);
    }
    nLinea_ = 1;
    nColumna_ = 0;
    nIdx_= 0;
    nColumna_ = 0;
    beof_ = false;
    contErrores = 0;
    errorComentario = false;
}/*fin del constructor*/

/*destructor*/
lexico::~lexico()
{
    if(flujoDeEntrada_.is_open())
    {
        flujoDeEntrada_.close();
    }
}/*fin del destructor*/

parTokenLexema * lexico::siguienteToken()
{
    while(!analiza());
    if(!beof_)
    {
        parTokenLexema * ptl = new parTokenLexema(sToken_,sLexema_);
        return ptl;
    }
    else
        return NULL;
}/*fin de siguienteToken*/

bool lexico::analiza()
{
    sToken_ = "";
    sLexema_ = "";
    int estado = 0;
    int estAnt = 0;
    int col = 0;
    bool flagEscape = false;

    while(estado != ACP && estado != ERR && beof_ == false) /*ciclo para determinar tokens completos*/
    {
        c = sigCaracter();
        nColumna_++;
        while(estado == 0 && (c == '\n' || c == '\t' || c == ' '))
        {
            c = sigCaracter();
            nColumna_++;
        }/*eliminador de blancos*/
        if(beof_ == true)
        {
            if(estado == 32 || estado == 33 || estado == 24)
            {
                errorComentario = true;
                errores[contErrores].linea = nLinea_ - 1;
                errores[contErrores].columna = nIdx_;
                errores[contErrores].error = "";
                errores[contErrores].lineaDeError = sLineaLeida_;

                if(estado == 32 || estado == 33)
                    errores[contErrores].descripcion = "EOF sin cerrar comentario";
                else if(estado == 24)
                    errores[contErrores].descripcion = "EOF sin cerrar cadena alfanumérica";
                contErrores++;
            }
            break;
        }

        if(c == '\\' && flagEscape == false && estado == 26){
            flagEscape = true;
            sLexema_ += c;
            continue;
        }

        col = colCar(c,estado);
        if(col >= 0 && estado != ERR && estado != ACP)
        {
            estAnt = estado;
            estado = mat_trans[estado][col];
            if(estado != ERR && estado != ACP)
            {
                sLexema_ += c;
            }
            else
            {
                if(estado == ACP)
                    antCaracter();
                else  //errores de construcción
                {
                    errores[contErrores].linea = nLinea_ - 1;
                    errores[contErrores].columna = nIdx_;
                    errores[contErrores].error = sLexema_;
                    errores[contErrores].lineaDeError = sLineaLeida_;
                    string s;
                    if(estAnt == 4)
                        s = "Símbolo no válido para hexadecimal";
                    else if(estAnt == 7 || estAnt == 10)
                        s = "Real incompleto se esperaba dígito";
                    else if(estAnt == 9)
                        s = "Real incompleto se esperaba + o -";
                    else if(estAnt == 19)
                        s = "Se esperaba | para operador lógico";
                    else if(estAnt == 21)
                        s = "Se esperaba & para operador lógico";
                    else if(estAnt == 27 || estAnt == 26)
                        s = "Se esperaba ' para cerrar caracter";
                    errores[contErrores].descripcion = s;
                    contErrores++;
                }
            }
        }
        if((c == ' ' || c == '\n' || c == '\t') && estado != 32 && estado != 31 &&
                estado != 24 && estado != 30)
        {
            estado = ACP;
        }
    }/*fin del ciclo principal de tokens*/

    if(estado == ACP)  /*cuando se llegó a estado de aceptación*/
    {
        switch(estAnt)
        {
        case 1:
            sToken_ = "ide";
            if(esCteLog(sLexema_))
                sToken_ = "clg";
            else if(esPalRes(sLexema_))
                sToken_ = "plr";
            break;
        case 2:
        case 6:
            sToken_ = "dec";
            break;
        case 3:
            sToken_ = "oct";
            break;
        case 5:
            sToken_ = "hex";
            break;
        case 8:
        case 11:
            sToken_ = "rea";
            break;
        case 12:
            sToken_ = "opa";
            break;
        case 13:
        case 14:
        case 16:
        case 18:
            sToken_ = "opr";
            break;
        case 15:
        case 20:
        case 22:
            sToken_ = "opl";
            break;
        case 17:
            sToken_ = "ops";
            break;
        case 23:
            sToken_ = "del";
            break;
        case 25:
            sToken_ = "cal";
            break;
        case 28:
            sToken_ = "cca";
            break;
        case 29:
            sToken_ = "opa";
            break;
        case 31:
            sToken_ = "cos";
            return false;
            break;
        case 34:
            sToken_ = "com";
            return false;
            break;
        case 35:
            sToken_ = "opa";
            break;
        case 36:
            sToken_ = "del";
            break;
        case 37:
            sToken_ = "asi";
            break;
        default:
            sToken_ = "ERR";
            break;
        }
    }
    else     /*cuando sé lleǵa a estado de error*/
    {
        //cerr << "Error en [" <<nLinea_ <<" , " << nColumna_ << "]" << endl;
        estado = 0;
    }
    return true;
}

string lexico::Serror(string des, string lex)
{
    stringstream ss;
    ss << "Error: " << des << " en " << lex << " [Reng: " << nLinea_ << ", Col: " << nColumna_;
    return ss.str();
}

bool lexico::esPalRes(string pal)
{
    for(int i = 0; i< CANTPALRES; i++)
    {
        if(pal.compare(palRes[i]) == 0)
        {
            return true;
        }
    }
    return false;
}/*fin del esPalRes*/

bool lexico::esCteLog(string pal)
{
    if(pal.compare("verdadero") == 0 || pal.compare("falso") == 0)
        return true;
    return false;
}/*fin de esCteLog*/

void lexico::leeDeArchivo()  /*método auxiliar*/
{

    if(sLineaLeida_.size() <= (uint)nIdx_)
    {
        if(!flujoDeEntrada_.eof()  && beof_ == false)
        {
            sLineaLeidaAnterior_ = sLineaLeida_;
            nIdx_ = 0;
            char tmp[TAM_MAX_BUFFER];
            try
            {
                nColumna_ = 0; // inicializamos la columna
                flujoDeEntrada_.getline(tmp,TAM_MAX_BUFFER);
                sLineaLeida_ = string(tmp);
                sLineaLeida_+='\n';
                nLinea_++;
            }
            catch(ifstream::failure e)
            {
                //cerr << "Error leyendo del archivo"<< endl;
                beof_ = true;
            }
            catch(exception e)
            {
                cerr << "Error leyendo del archivo"<< endl;
            }
        }
        else
        {
            beof_ = true;
        }
    }
}/*fin de leeDeArchivo*/

char lexico::sigCaracter()
{
    leeDeArchivo();
    /*if(sLineaLeida_.size() > (uint)nIdx_)*/
    return sLineaLeida_[nIdx_++];
    /*else return '\n';*/
}


char lexico::antCaracter()
{
    if(nIdx_ > 0)
    {
        nIdx_--;
        return sLineaLeida_[nIdx_];
    }
    else
    {
        return sLineaLeidaAnterior_[sLineaLeidaAnterior_.size() - 1];
    }
}

int lexico::colCar(char x, int estado)
{
    char c = tolower(x);
    if(c == '\n')
        return 21;
    if(c == ' ' || c == '\t')
        return 22;
    if(c == '0')
        return 1;
    if(c == 'e')
        return 9;
    if(c >= 'a' && c <= 'f')
        return 6;
    if(c == 'x')
        return 5;
    if(c >= 'a' && c <= 'z')
        return 0;
    if(c == '_')
        return 4;
    if(c >= '1' && c <= '7')
        return 2;
    if(c == '8' || c == '9')
        return 3;
    if(c == ';' || c == '{' || c == '}' || c == ',' || c == '(' ||
            c == ')' || c == '[' || c == ']')
        return 7;
    if(c == ':')
        return 23;
    if(c == '.')
        return 8;
    if(c == '+' || c == '-')
        return 10;
    if(c == '%' || c == '^')
        return 11;
    if(c == '/')
        return 12;
    if(c == '*')
        return 13;
    if(c == '=')
        return 14;
    if(c == '|')
        return 15;
    if(c =='&') return 16;
    if(c == '!') return 17;
    if(c == '<' || c == '>') return 18;
    if(c == 34) return 19;/*"*/
    if(c == 39) return 20;/*'*/
    if(((c >= 10 && c <= 255) || (c < 0)) && (estado == 24 || estado == 26 || estado == 30 || estado == 32))
        return 0;
    if(x > 15 || x < 0)  /*para evitar los retornos de carro */
    {
        errores[contErrores].linea = nLinea_ - 1;
        errores[contErrores].columna = nIdx_;
        errores[contErrores].error = sLexema_;
        stringstream ss;
        ss << "Caracter inválido {" << x << "}";

        errores[contErrores].descripcion = ss.str();
        errores[contErrores].lineaDeError = sLineaLeida_;
        //cout << "ERROR: Caracter inválido <" << x << "> en la linea " << nLinea_ - 1 << " columna " << nColumna_ << endl;
        //cout << "----------------------------------------------" << endl;
        if(x < 0)// se avanza un caractér cuando se trata de una caractér Unicode
            sigCaracter();
        contErrores++;
    }
    return ERR;
}


/*Setter and getters ----------------------------------------------*/

int lexico::getLineas()
{
    return nLinea_ - 1;
}
