/*
 * NoeudXML.hpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef TP2_NOEUDXML_HPP_
#define TP2_NOEUDXML_HPP_


#include <ostream>
#include <string>


using namespace std;


class NoeudXML
{
public :
    NoeudXML( void );
    NoeudXML( NoeudXML const & a_noeudXML );
    virtual ~NoeudXML( void );

    virtual bool estElement( void ) const;
    virtual string texte( void ) const;
    virtual ostream & afficher( ostream & ) const;

    friend ostream & operator << ( ostream &, NoeudXML const & );
};



#endif /* TP2_NOEUDXML_HPP_ */
