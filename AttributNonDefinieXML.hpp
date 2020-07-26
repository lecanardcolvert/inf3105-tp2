/*
 * AttributNonDefinieXML.hpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#ifndef TP2_ATTRIBUTNONDEFINIEXML_HPP_
#define TP2_ATTRIBUTNONDEFINIEXML_HPP_


#include <exception>
#include <string>


using namespace std;


class AttributNonDefinieXML : public exception
{
private :
  string _nom;

public :
  AttributNonDefinieXML( void );
  AttributNonDefinieXML( string a_nom );
  AttributNonDefinieXML( AttributNonDefinieXML const & a_e );
  virtual ~AttributNonDefinieXML( void );

  virtual const char* what() const throw();
};




#endif /* TP2_ATTRIBUTNONDEFINIEXML_HPP_ */
