/*
 * TexteXML.hpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef TP2_TEXTEXML_HPP_
#define TP2_TEXTEXML_HPP_

#include "NoeudXML.hpp"

#include <ostream>
#include <string>


using namespace std;


class TexteXML : public NoeudXML
{
private :
    string _texte;
public :
    TexteXML( void );
    TexteXML( string a_texte );
    TexteXML( TexteXML const & a_texte );
    virtual ~TexteXML( void );

    string texte( void ) const;

    ostream & afficher( ostream & ) const;
};



#endif /* TP2_TEXTEXML_HPP_ */
