/*
 * NoeudXML.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */


#include "NoeudXML.hpp"

#include <ostream>
#include <string>


using namespace std;


NoeudXML::NoeudXML( void )
{
}


NoeudXML::NoeudXML( NoeudXML const & a_noeudXML )
{
}


NoeudXML::~NoeudXML( void )
{
}


bool
NoeudXML::estElement( void ) const
{
    return false;
}


string
NoeudXML::texte( void ) const
{
    return "";
}


ostream &
NoeudXML::afficher( ostream & a_out ) const
{
    a_out << "allo : ne devrait pas etre ici." << endl;
    return a_out;
}


ostream &
operator << ( ostream & a_out, NoeudXML const & a_noeudXML )
{
    return a_noeudXML.afficher( a_out );
}




