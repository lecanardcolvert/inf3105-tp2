/*
 * ElementXML.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#include "ElementXML.hpp"

#include "AttributNonDefinieXML.hpp"
#include "NoeudXML.hpp"
#include "TexteXML.hpp"

#include <cassert>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>


using namespace std;



ElementXML::ElementXML( void ) :
    _parent( nullptr ),
    _nom( "" )
{
}


ElementXML::ElementXML( ElementXML * a_parent, string a_nom ) :
    _parent( a_parent ),
    _nom( a_nom )
{
}


ElementXML::ElementXML( ElementXML const & a_element ) :
    _parent( a_element._parent ),
    _nom( a_element._nom ),
    _attributs( a_element._attributs ),
    _enfants( a_element._enfants )
{
}


ElementXML::~ElementXML( void )
{
}


bool ElementXML::estElement( void ) const
{
    return true;
}


string
ElementXML::texte( void ) const
{
    string resultat = "";

    for( NoeudXML * c : _enfants )
    {
        assert( nullptr != c );

        resultat += c->texte();
    }

    return resultat;
}


ostream &
ElementXML::afficher( ostream & a_out) const
{
    a_out << '<' << _nom;

    for( pair< string, string > att : _attributs )
    {
        a_out << ' ' << att.first
              << '=';
        if( string::npos == att.second.find('"') )
        {
            a_out << '"' << att.second << '"';
        }
        else
        {
            a_out << '\'' << att.second << '\'';
        }
    }

    a_out << '>' << endl;

    for( NoeudXML * contenu : _enfants )
    {
        assert( nullptr != contenu );

        a_out << ( * contenu );
    }

    a_out << "</" << _nom << ">" << endl;

    return a_out;
}


TexteXML *
ElementXML::ajouterTexte( string a_texte )
{
    TexteXML * texte = new TexteXML( a_texte );

    _enfants.push_back( texte );

    return texte;
}


ElementXML *
ElementXML::ajouterElement( string a_nom )
{
    ElementXML * element = new ElementXML( this, a_nom );

    _enfants.push_back( element );

    return element;
}

void
ElementXML::ajouterAttribut( string a_nom, string a_valeur )
{
    _attributs[ a_nom ] = a_valeur;
}


ElementXML *
ElementXML::parent( void ) const
{
    return _parent;
}


ElementXML *
ElementXML::prochainElement( void ) const
{
    vector< NoeudXML * >::const_iterator it = begin();

    while( ! ( * it )->estElement() )
    {
        ++ it;
    }

    return ( ElementXML * )( * it );
}


string
ElementXML::nom( void ) const
{
    return _nom;
}


string
ElementXML::attribut( string a_nom ) const
{
    map< string, string >::const_iterator it = _attributs.find( a_nom );

    if( it == _attributs.end() )
    {
        throw AttributNonDefinieXML( a_nom );
    }

    return ( * it ).second;
}


vector< NoeudXML * >::const_iterator
ElementXML::begin( void ) const
{
    return _enfants.begin();
}


vector< NoeudXML * >::const_iterator
ElementXML::end( void ) const
{
    return _enfants.end();
}

