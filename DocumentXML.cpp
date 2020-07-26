/*
 * DocumentXML.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */


#include "DocumentXML.hpp"


#include "ElementXML.hpp"


#include <cassert>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>




using namespace std;






DocumentXML::DocumentXML() :
    _racine( new ElementXML( nullptr, "__root__" ) )
{
}


DocumentXML::DocumentXML( DocumentXML const & a_documentXML ) :
    _racine( new ElementXML( * ( a_documentXML._racine ) ) )
{
}


DocumentXML::~DocumentXML( void )
{
    delete _racine;
}


ElementXML *
DocumentXML::racine( void ) const
{
    return _racine;
}


ostream &
operator << ( ostream & a_out, DocumentXML const & a_documentXML )
{
    assert( nullptr != a_documentXML._racine );

    a_out << * ( a_documentXML._racine );

    return a_out;
}

