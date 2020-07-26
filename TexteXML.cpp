/*
 * TexteXML.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */


#include "TexteXML.hpp"

#include "NoeudXML.hpp"

#include <ostream>
#include <string>


using namespace std;


TexteXML::TexteXML( void ) :
    _texte( "" )
{
}


TexteXML::TexteXML( string a_texte ) :
    _texte( a_texte )
{
}


TexteXML::TexteXML( TexteXML const & a_texte ) :
    _texte( a_texte._texte )
{
}


TexteXML::~TexteXML( void )
{
}


string
TexteXML::texte( void ) const
{
    return _texte;
}


ostream &
TexteXML::afficher( ostream & a_out ) const
{
    a_out << _texte;

    return a_out;
}


