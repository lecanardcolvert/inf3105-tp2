/*
 * AttributNonDefinieXML.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */


#include "AttributNonDefinieXML.hpp"

#include <exception>
#include <string>


using namespace std;


AttributNonDefinieXML::AttributNonDefinieXML( void ) :
    _nom( "" )
{
}


AttributNonDefinieXML::AttributNonDefinieXML( string a_nom ) :
    _nom( a_nom )
{
}


AttributNonDefinieXML::AttributNonDefinieXML( AttributNonDefinieXML const & a_e ) :
    _nom( a_e._nom )
{
}


AttributNonDefinieXML::~AttributNonDefinieXML( void )
{
}


const char*
AttributNonDefinieXML::what() const throw()
{
    return ( string( "L'attribut " ) + _nom + string( " n'est pas defini." ) ).c_str();
}



