
NOM=TP2

CC=g++

OBJ=$(NOM).o\
    AttributNonDefinieXML.o\
    DocumentXML.o\
    ElementXML.o\
    Histoire.o\
    Lecteur.o\
    NoeudXML.o\
    Phrase.o\
    TexteXML.o

.cpp.o :
	$(CC) $(FLAGS) -c $<

$(NOM) : $(OBJ)
	$(CC) -o $(NOM) $(OBJ)

AttributNonDefinieXML.cpp : AttributNonDefinieXML.hpp

DocumentXML.cpp : DocumentXML.hpp ElementXML.hpp

ElementXML.cpp : ElementXML.hpp AttributNonDefinieXML.hpp NoeudXML.hpp

Histoire.cpp : Histoire.hpp Phrase.hpp

Lecteur.cpp : Lecteur.hpp DocumentXML.hpp ElementXML.hpp

NoeudXML.cpp : NoeudXML.hpp

Phrase.cpp : Phrase.hpp

TexteXML.cpp : TexteXML.hpp NoeudXML.hpp

arbremap.h : arbreavl.h

$(NOM).cpp : Histoire.hpp Lecteur.hpp Phrase.hpp arbremap.h

clean :
	\rm -f *.o
	\rm -f $(NOM)

archive : clean
	\rm -f $(NOM).tar
	\rm -f $(NOM).tar.gz
	tar cvf $(NOM).tar *
	gzip $(NOM).tar
