/*
 * Histoire.h
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef HISTOIRE_H_
#define HISTOIRE_H_

#include "DocumentXML.hpp"
#include "Phrase.hpp"


#include <iterator>
#include <ostream>
#include <string>
#include <vector>


using namespace std;


const int TAILLE_LIGNE = 80;


class Histoire
{
public :
    typedef vector< Phrase >::iterator iterator;
    typedef vector< Phrase >::const_iterator const_iterator;

    Histoire( void );
    Histoire( string a_titre );
    Histoire( Histoire const & a_histoire );
    virtual ~Histoire( void );

    string titre( void ) const;

    void ajouterPhrase( Phrase a_phrase );

    vector< Phrase >::const_iterator begin( void ) const;
    vector< Phrase >::const_iterator end( void ) const;

    friend ostream & operator << ( ostream &, Histoire const & );
private :
    string _titre;
    vector< Phrase > _phrases;
};

vector< Histoire * > * extraireHistoires( DocumentXML const & a_document );


#endif /* HISTOIRE_H_ */
