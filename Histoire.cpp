/*
 * Histoire.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#include "Histoire.hpp"

#include "Phrase.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>


using namespace std;


Histoire::Histoire( void )
{
}


Histoire::Histoire( string a_titre ) :
   _titre( a_titre )
{
}


Histoire::Histoire( Histoire const & a_histoire ) :
    _titre( a_histoire._titre ),
    _phrases( a_histoire._phrases )
{
}


Histoire::~Histoire( void )
{
}


string
Histoire::titre( void ) const
{
    return _titre;
}


void
Histoire::ajouterPhrase( Phrase a_phrase )
{
    _phrases.push_back( a_phrase );
}


vector< Phrase >::const_iterator
Histoire::begin( void ) const
{
    return _phrases.begin();
}

vector< Phrase >::const_iterator
Histoire::end( void ) const
{
    return _phrases.end();
}


bool
estCaracterePhrase( char c )
{
    return '.' != c;
}


void
extrairePhrasesHistoire( ElementXML * a_histoireXML, Histoire * a_histoire )
{
    assert( nullptr != a_histoireXML );
    stringstream texte( a_histoireXML->texte() );
    stringstream accumulateur;
    char c;

    while( ! texte.eof() )
    {
        do
        {
            texte.get( c );
        }
        while( ! estCaracterePhrase( c ) && ! texte.eof() );

        while( estCaracterePhrase( c ) && ! texte.eof() )
        {
            accumulateur << c;
            texte.get( c );
        }

        string phrase = accumulateur.str();
        accumulateur.str( "" );

        if( phrase.length() != 0 )
        {
            assert( nullptr != a_histoire );
            Phrase * nouvellePhrase = new Phrase( phrase );
            assert( nullptr != nouvellePhrase );
            if( nouvellePhrase->begin() != nouvellePhrase->end() )
            {
                a_histoire->ajouterPhrase( *nouvellePhrase );
            }
            else
            {
                delete nouvellePhrase;
            }
        }
    }
}


vector< Histoire * > *
extraireHistoires( DocumentXML const & a_document )
{
    vector< Histoire * > * resultat = new vector< Histoire * >();

    ElementXML * courant = a_document.racine();
    vector< NoeudXML * >::const_iterator it;

    // trouver <magasine>
    courant = courant->prochainElement();

    assert( nullptr != courant );
    // trouver <contenus>
    courant = courant->prochainElement();

    assert( nullptr != courant );
    it = courant->begin();
    assert( nullptr != courant );
    vector< NoeudXML * >::const_iterator fin = courant->end();

    for( ; it < fin; ++ it )
    {
        assert( nullptr != *it );
        if( ( * it )->estElement() )
        {
            ElementXML * histoireXML = ( ElementXML * )( * it );

            assert( nullptr != histoireXML );
            string titre = histoireXML->attribut( string( "titre" ) );

            try
            {
                assert( nullptr != histoireXML );
                titre += " (partie " + histoireXML->attribut( string( "partie" ) ) + ")";
            }
            catch( AttributNonDefinieXML & a_e )
            {
                // rien
            }

            Histoire * nouvelle = new Histoire( titre );

            extrairePhrasesHistoire( histoireXML, nouvelle );

            assert( nullptr != resultat );
            resultat->push_back( nouvelle );
        }
    }

    return resultat;
}


ostream &
operator << ( ostream & a_out, Histoire const & a_histoire )
{
    int compteur = 0;

    a_out << a_histoire._titre << endl << endl;

    for( Phrase phrase : a_histoire )
    {
        a_out << phrase;
    }

    return a_out;
}
