#include "parTokenLexema.h"


parTokenLexema::parTokenLexema(string sToken_,string sLexema_){
    this->sToken_ = sToken_;
    this->sLexema_ = sLexema_;
}

string parTokenLexema::getLexema(){
    return sLexema_;
}

string parTokenLexema::getToken(){
    return sToken_;
}
