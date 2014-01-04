#include <sstream>
#include <cstdio>
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
    /*0 */ {1,2,6,6,1,1,1,23,23,1,12,12,29,35,17,19,21,15,13,24,26,ACP,ACP,36},
    /*1 */ {1,1,1,1,1,1,1,ACP,ACP,1,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*2 */ {ACP,3,3,ACP,ACP,4,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*3 */ {ACP,3,3,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*4 */ {ERR,5,5,5,ERR,ERR,5,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*5 */ {ACP,5,5,5,ACP,ACP,5,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*6 */ {ACP,6,6,6,ACP,ACP,ACP,ACP,7,9,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*7 */ {ERR,8,8,8,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*8 */ {ACP,8,8,8,ACP,ACP,ACP,ACP,ACP,9,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*9 */ {ERR,11,11,11,ERR,ERR,ERR,ERR,ERR,ERR,10,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*10*/ {ERR,11,11,11,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*11*/ {ACP,11,11,11,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*12*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*13*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,14,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*14*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*15*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,16,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*16*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*17*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,18,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*18*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*19*/ {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,20,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR},
    /*20*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*21*/ {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,22,ERR,ERR,ERR,ERR,ERR,ERR,ERR}, /*tenía error al usar &&*/
    /*22*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*23*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*24*/ {24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,25,24,24,24,24},
    /*25*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},/*tenia error alfabeticas*/
    /*26*/ {27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,28,ERR,ERR,ERR},
    /*27*/ {ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,ERR,28,ERR,ERR,ERR},
    /*28*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*29*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,30,32,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*30*/ {30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,31,30,30},
    /*31*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*32*/ {32,32,32,32,32,32,32,32,32,32,32,32,32,33,32,32,32,32,32,32,32,32,32,32},
    /*33*/ {32,32,32,32,32,32,32,32,32,32,32,32,34,32,32,32,32,32,32,32,32,32,32,32},
    /*34*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*35*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*36*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,37,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP},
    /*37*/ {ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP,ACP}
};

const string palRes[]= { "interrumpe","valor","canal","const","continua",
                         "defecto","diferir","sino","desde","funcion","ir","ir_a","si",
                         "importar","interfaz","mapa","paquete","rango","regresa",
                         "selecciona","estructura","tipo","variable","caso","principal",
                         "caracter","entero","real","alfabetico","logico","enteross",
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
        if(!editor_)
            cout << "Error intentando abrir el archivo fuente" << endl;
        this->~lexico();
        exit(EXIT_FAILURE);
    }
    nLinea_ = 1;
    nColumna_ = 0;
    nIdx_= 0;
    nColumna_ = 0;
    beof_ = false;
    errorComentario = false;
    simboloActual_ = simboloAnterior_ = NULL;
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
        if(simboloAnterior_ != NULL)
            delete simboloAnterior_;
        simboloAnterior_ = simboloActual_;
        parTokenLexema * ptl = new parTokenLexema(sToken_,sLexema_);
        simboloActual_ = ptl;
#ifdef PRINT
        if(!editor_)
            cout << " " << sLexema_;
#endif
        return ptl;
    }
    else
        return NULL;
}/*fin de siguienteToken*/

parTokenLexema * lexico::tokenAnterior()
{
    return simboloAnterior_;
}

parTokenLexema * lexico::tokenActual()
{
    return simboloActual_;
}


bool lexico::analiza()
{
    sToken_ = "";
    sLexema_ = "";
    int estado = 0;
    int estAnt = 0;
    int col = 0;
    bool flagEscape = false;
    saltoDeLinea_ = false;

    while(estado != ACP && estado != ERR && beof_ == false) /*ciclo para determinar tokens completos*/
    {
        c = sigCaracter();
        //nColumna_++;
        while(estado == 0 && (c == '\n' || c == '\t' || c == ' '))
        {
            if(c == '\n')
                saltoDeLinea_ = true;
            c = sigCaracter();
            //nColumna_++;
        }/*eliminador de blancos*/
        if(beof_ == true)
        {
            if(estado == 32 || estado == 33 || estado == 24)
            {
                string des;
                errorComentario = true;

                if(estado == 32 || estado == 33)
                    des = "EOF sin cerrar comentario";
                else if(estado == 24)
                    des = "EOF sin cerrar cadena alfanumérica";
                stringstream ss;
                string s;
                ss << c;
                ss >> s;
                e.nuevoError(nLinea_ - 1, nIdx_, s,des,sLineaLeida_);

            }
            break;
        }

        if(c == '\\' && flagEscape == false && estado == 26)
        {
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

                    e.nuevoError(nLinea_ - 1, nIdx_, sLexema_,s,sLineaLeida_);
                }
            }
        }
        if((c == ' ' || c == '\n' || c == '\t') && estado != 32 && estado != 31 &&
                estado != 24 && estado != 30)
        {
            estado = ACP;
        }
        else
        if((estAnt == 24 || estado == 24) && c =='\n'){
            string s = "no se cerró la declaración de cadena";
            sLexema_ = sLexema_.substr(0,sLexema_.length()-1);
            e.nuevoError(nLinea_ - 1, nIdx_, sLexema_,s,sLineaLeida_);
        }
    }/*fin del ciclo principal de tokens*/

    nColumnaAnt_ = nColumna_;
    nColumna_ = nIdx_;

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
            nColumnaAnt_ = 0;
            char tmp[TAM_MAX_BUFFER];
            try
            {
                nColumna_ = 0; // inicializamos la columna
                flujoDeEntrada_.getline(tmp,TAM_MAX_BUFFER);
                sLineaLeida_ = string(tmp);
                sLineaLeida_+='\n';
                nLinea_++;
#ifdef PRINT
                if(!editor_)
                    cout << "\n["<< nLinea_ - 1 <<"] ";
#endif
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
        stringstream ss;
        ss << "Caracter inválido {" << x << "}";
        //cout << "ERROR: Caracter inválido <" << x << "> en la linea " << nLinea_ - 1 << " columna " << nColumna_ << endl;
        //cout << "----------------------------------------------" << endl;
        if(x < 0)// se avanza un caractér cuando se trata de una caractér Unicode
            sigCaracter();


        e.nuevoError(nLinea_ - 1, nIdx_,sLexema_,ss.str(),sLineaLeida_);
    }
    return ERR;
}

void
lexico::nuevoError(string descripcion)
{
    e.nuevoError(nLinea_ - 1, nColumna_, sLexema_,descripcion,sLineaLeida_);
}

void
lexico::imprimeErrores()
{
    if(e.getCantErrores() > 0)
    {
        if(!editor_)
        {
            /*Cuando no se activó la bandera de editor*/
            cout << "\n\t\t\t\tERRORES" << endl;
            cout <<  "+-------+---------+-----------+---------------------------------+--------------+" << endl;
            cout <<  "| Linea | Columna |Error      |Descripcion\t\t\t|Linea completa|" <<endl;
            //cout <<  "________________________________________________________________________________" << endl;
            cout <<  "+-------+---------+-----------+---------------------------------+--------------+" << endl;
            cout <<  "+-------+---------+-----------+---------------------------------+--------------+" << endl;
            for(int i = 0 ; i < e.getCantErrores(); i++)
            {
                /*cout << errores[i].linea << "\t" <<errores[i].columna << "\t\t" << errores[i].error << "\t" << errores[i].descripcion<<
                "\t" << errores[i].lineaDeError;*/
                if(e.getError(i)->error.size() > 10)
                    e.getError(i)->error = e.getError(i)->error.substr(0,10);
                if(e.getError(i)->descripcion.size() > 34)
                    e.getError(i)->descripcion = e.getError(i)->descripcion.substr(0,34);
                if(e.getError(i)->lineaDeError.size() > 13)
                    e.getError(i)->lineaDeError = e.getError(i)->lineaDeError.substr(0,13);
                else
                    e.getError(i)->lineaDeError = e.getError(i)->lineaDeError.substr(0,e.getError(i)->lineaDeError.size() - 1);
                string str = e.getError(i)->lineaDeError;

                printf("| %5d | %7d |%11s|%33s|%14s|\n",e.getError(i)->linea,e.getError(i)->columna,e.getError(i)->error.c_str(),e.getError(i)->descripcion.c_str(),e.getError(i)->lineaDeError.c_str());
            }
            cout <<  "+-------+---------+-----------+---------------------------------+--------------+" << endl;
        }
        else
        {
            /*cuando se especificó un editor*/
            /*salida formateada*/
            for(int i = 0 ; i < e.getCantErrores(); i++)
            {
                /*trim*/
                e.getError(i)->lineaDeError = trim(e.getError(i)->lineaDeError);
                printf("%d~%d~%s~%s~%s\n",e.getError(i)->linea,e.getError(i)->columna,e.getError(i)->error.c_str(),e.getError(i)->descripcion.c_str(),e.getError(i)->lineaDeError.c_str());
            }
        }
    }
}

bool
lexico::esConstanteLogica(string val)
{
    return esCteLog(val);
}

void
lexico::setEditor(bool e)
{
    editor_ = e;
}

string
lexico::trim(string c){
    c.erase(c.find_last_not_of(" \n\r\t")+1);
    unsigned int i;
    for(i=0;i< c.length();i++){
        if(c[i]!=' ' && c[i]!='\t' && c[i]!='\n')
            break;
    }
    return c.substr(i);
}

bool
lexico::HuboSaltoDeLinea(){
    return saltoDeLinea_;
}

bool
lexico::getEditor()
{
    return editor_;
}

void
lexico::avanzarHasta(string c){
    string leido="";
    do{
        leido = siguienteToken()->getLexema();
    }while(c != leido);
}

/*Setter and getters ----------------------------------------------*/

int lexico::getLineas()
{
    return nLinea_ - 1;
}
