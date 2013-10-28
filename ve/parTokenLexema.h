/** \file parTokenLexema.h
 * \brief Definición de la clase que define un Token (tipo, valor).
 * \date Oct - 2013
 * \author Francisco de Jesús Méndez Pelayo
 * \copyright GNU Public License.
 */
#ifndef PARTOKENLEXEMA_H
#define PARTOKENLEXEMA_H

#include <string>

using namespace std;

/** \brief Define la clase para representar un token del lenguaje
 * \class parTokenLexema
 */
class parTokenLexema{
public:
    /**
     * \param string token representa el token
     * \param string lexema representa el lexema
     */
    parTokenLexema(string token,string lexema);
    /** \brief obtiene el Token del objeto
     * \return string token
     */
    string getToken();
    /** \brief obtiene el Lexema del objeto
     * \return string lexema
     */
    string getLexema();

private:
    string sToken_;
    string sLexema_;
};

#endif // PARTOKENLEXEMA_H
