/*
 * Phrase.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#include "Phrase.hpp"

#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>


using namespace std;



bool 
estCaractereMot( char c )
{
    return ( 'A' <= c && c <= 'Z' ) ||
           ( 'a' <= c && c <= 'z' ) ||
           ( '-' == c );
}



Phrase::Phrase( void ) :
    _original( string( "" ) )
{}


Phrase::Phrase( string a_original ) :
    _original( a_original )
{
    stringstream texte( a_original );
    stringstream accumulateur;
    char c;

    while( ! texte.eof() )
    {
        do
        {
            texte.get( c );
        }
        while( ! estCaractereMot( c ) && ! texte.eof() );

        while( estCaractereMot( c ) && ! texte.eof() )
        {
            accumulateur << c;
            texte.get( c );
        }

        string mot = accumulateur.str();
        accumulateur.str( "" );

        if( mot.length() != 0 )
        {
            transform( mot.begin(), mot.end(), mot.begin(), ::tolower );
            _mots.push_back( mot );
        }
    }
}


Phrase::Phrase( Phrase const & a_phrase ) :
    _original( a_phrase._original ),
    _mots( a_phrase._mots )
{
}


Phrase::~Phrase( void )
{
}


int 
Phrase::nMot( void ) const
{
    return _mots.size();
}


vector< string >::const_iterator
Phrase::begin( void ) const
{
    return _mots.begin();
}


vector< string >::const_iterator
Phrase::end( void ) const
{
    return _mots.end();
}


ostream & 
operator << ( ostream & a_out, const Phrase & a_phrase )
{
    bool premier = true;
    
    for( string mot : a_phrase._mots )
    {
        if( premier )
        {
            premier = false;
        }
        else
        {
            a_out << " ";
        }
        a_out << mot;
    }
    
    return a_out;
}
