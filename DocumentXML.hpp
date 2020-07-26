/*
 * DocumentXML.h
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef DOCUMENTXML_H_
#define DOCUMENTXML_H_

#include "ElementXML.hpp"

#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;


class DocumentXML
{
private :
	ElementXML * _racine;
public :
	DocumentXML();
	DocumentXML( DocumentXML const & a_documentXML );
	virtual ~DocumentXML( void );

	ElementXML * racine( void ) const;

    friend ostream & operator << ( ostream &, DocumentXML const & );
};


#endif /* DOCUMENTXML_H_ */
