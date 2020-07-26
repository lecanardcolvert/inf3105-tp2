/*
 * ElementXML.hpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef TP2_ELEMENTXML_HPP_
#define TP2_ELEMENTXML_HPP_


#include "AttributNonDefinieXML.hpp"
#include "NoeudXML.hpp"
#include "TexteXML.hpp"

#include <map>
#include <ostream>
#include <string>
#include <vector>


using namespace std;


class ElementXML : public NoeudXML
{
private :
    string _nom;
    ElementXML * _parent;
    map< string, string > _attributs;
    vector< NoeudXML * > _enfants;
public :
    ElementXML( void );
    ElementXML( ElementXML * a_parent, string a_nom );
    ElementXML( ElementXML const & a_element );
    virtual ~ElementXML( void );

    TexteXML * ajouterTexte( string a_texte );
    ElementXML * ajouterElement( string a_nom );
    void ajouterAttribut( string a_nom, string a_valeur );

    bool estElement( void ) const;
    string texte( void ) const;
    ElementXML * parent( void ) const;
    ElementXML * prochainElement( void ) const;
    string nom( void ) const;
    string attribut( string a_nom ) const;

    vector< NoeudXML * >::const_iterator begin( void ) const;
    vector< NoeudXML * >::const_iterator end( void ) const;

    virtual ostream & afficher( ostream & ) const;
};



#endif /* TP2_ELEMENTXML_HPP_ */
