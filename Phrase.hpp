/*
 * Phrase.h
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef PHRASE_H_
#define PHRASE_H_


#include <iterator>
#include <ostream>
#include <string>
#include <vector>

using namespace std;


class Phrase
{
private :
    string _original;
    vector< string > _mots;
public :
    typedef vector< string >::iterator iterator;
    typedef vector< string >::const_iterator const_iterator;

    Phrase( void );
    Phrase( string a_original );
    Phrase( Phrase const & a_phrase );
    virtual ~Phrase( void );

    int nMot( void ) const;

    vector< string >::const_iterator begin( void ) const;
    vector< string >::const_iterator end( void ) const;

    friend ostream & operator << ( ostream & a_out, const Phrase & a_phrase );
};


#endif /* PHRASE_H_ */
