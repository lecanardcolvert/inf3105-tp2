/*
 * Lecteur.cpp
 *
 *  Created on: 2020-04-07
 *      Author: Bruno Malenfant
 */

#include "Lecteur.hpp"


#include "DocumentXML.hpp"
#include "ElementXML.hpp"

#include <cassert>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>


#ifdef DEBUG_STATE
#define _DEBUG_STATE_( _STR_EXP_ ) { cout << _STR_EXP_ << endl; }
#else
#define _DEBUG_STATE_( _STR_EXP_ ) ;
#endif


#ifdef DEBUG_STRUCT
#define _DEBUG_STRUCT_( _STR_EXP_ ) { cout << _STR_EXP_ << endl; }
#else
#define _DEBUG_STRUCT_( _STR_EXP_ ) ;
#endif


bool
estLettrePourDebutId( char a_c )
{
    return ( ':' == a_c ) ||
           ( '_' == a_c ) ||
           ( 'A' <= a_c && a_c <= 'Z' ) ||
           ( 'a' <= a_c && a_c <= 'z' );
}


bool
estLettrePourId( char a_c )
{
    return estLettrePourDebutId( a_c ) ||
           ( '-' == a_c ) ||
           ( '.' == a_c ) ||
           ( '0' <= a_c && a_c <= '9' );
}


bool
estEspace( char a_c )
{
    return ( ' ' == a_c ) ||
           ( '\t' == a_c ) ||
           ( '\n' == a_c ) ||
           ( '\r' == a_c );
}


enum Etat
{
    TEXTE, OUVERTURE_TAG, NOM_DEBUT_ELEMENT,
    NOM_FIN_PREMIER, LISTE_ATTRIBUTS, ATTRIBUT_NOM,
    FIN_TAG_COMPLET, ATT_EGAL, ATT_APOSTROPHE,
    ATT_GUILLEMET, PI_DEBUT, PI_FIN_POSSIBLE
} ;


struct Environnement
{
    stringstream accumulateur;
    ElementXML * courant;
    string nomTemp;

    Environnement( ElementXML * a_elementInitial ) :
        courant( a_elementInitial ),
        nomTemp( "" )
    {}
};


typedef void (* FonctionTransitionAction)( Etat &, char, Environnement * );


void
fctTA_TEXTE( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "TEXTE" )
    if( '<' == c )
    {
        etat = OUVERTURE_TAG;
        assert( nullptr != env );
        if( ! env->accumulateur.eof() )
        {
            _DEBUG_STRUCT_( env->accumulateur.str().substr( 0, 10 ) << "..." )
            assert( nullptr != env );
            assert( nullptr != env->courant );
            env->courant->ajouterTexte( env->accumulateur.str() );
            assert( nullptr != env );
            env->accumulateur.str( "" );
        }
    }
    else
    {
        assert( nullptr != env );
        env->accumulateur << c;
    }
}


void
fctTA_OUVERTURE_TAG( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "OUVERTURE_TAG" )
    if( estLettrePourDebutId( c ) )
    {
        assert( nullptr != env );
        env->accumulateur << c;
        etat = NOM_DEBUT_ELEMENT;
    }
    else if( '/' == c )
    {
        etat = NOM_FIN_PREMIER;
    }
    else if( '?' == c )
    {
        etat = PI_DEBUT;
    }
}


void
fctTA_NOM_DEBUT_ELEMENT( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "NOM_DEBUT_ELEMENT" )
    if( estLettrePourId( c ) )
    {
        assert( nullptr != env );
        env->accumulateur << c;
    }
    else if( '>' == c )
    {
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant = env->courant->ajouterElement( env->accumulateur.str() );
        _DEBUG_STRUCT_( "<" << env->courant->nom() << ">" )
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = TEXTE;
    }
    else if( estEspace( c ) )
    {
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant = env->courant->ajouterElement( env->accumulateur.str() );
        _DEBUG_STRUCT_( "<" << env->courant->nom() << ">" )
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = LISTE_ATTRIBUTS;
    }
    else if( '/' == c )
    {
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant = env->courant->ajouterElement( env->accumulateur.str() );
        _DEBUG_STRUCT_( "<" << env->courant->nom() << ">" )
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = FIN_TAG_COMPLET;
    }
}



void
fctTA_NOM_FIN_PREMIER( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "NOM_FIN_PREMIER" )
    if( estLettrePourId( c ) )
    {
        assert( nullptr != env );
        env->accumulateur << c;
    }
    else if( '>' == c )
    {
        _DEBUG_STRUCT_( "</" << env->courant->nom() << ">" )
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant = env->courant->parent();
        _DEBUG_STRUCT_( "--> <" << env->courant->nom() << ">" )
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = TEXTE;
    }
}



void
fctTA_LISTE_ATTRIBUTS( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "LISTE_ATTRIBUTS" )
    if( estLettrePourDebutId( c ) )
    {
        assert( nullptr != env );
        env->accumulateur << c;
        etat = ATTRIBUT_NOM;
    }
    else if( '>' == c )
    {
        etat = TEXTE;
    }
    else if( '/' == c )
    {
        etat = FIN_TAG_COMPLET;
    }
}



void
fctTA_ATTRIBUT_NOM( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "ATTRIBUT_NOM" )
    if( estLettrePourId( c ) )
    {
        assert( nullptr != env );
        env->accumulateur << c;
    }
    else if( '=' == c )
    {
        assert( nullptr != env );
        env->nomTemp = env->accumulateur.str();
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = ATT_EGAL;
    }
}



void
fctTA_FIN_TAG_COMPLET( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "FIN_TAG_COMPLET" )
    if( '>' == c )
    {
        _DEBUG_STRUCT_( "</" << env->courant->nom() << ">" )
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant = env->courant->parent();
        _DEBUG_STRUCT_( "--> <" << env->courant->nom() << ">" )
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = TEXTE;
    }
}



void
fctTA_ATT_EGAL( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "ATT_EGAL" )
    if( '\'' == c )
    {
        etat = ATT_APOSTROPHE;
    }
    else if( '"' == c )
    {
        etat = ATT_GUILLEMET;
    }
}



void
fctTA_ATT_APOSTROPHE( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "ATT_APOSTROPHE" )
    if( '\'' == c )
    {
        _DEBUG_STRUCT_( nomTemp << "='" << env->accumulateur.str() << "'" )
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant->ajouterAttribut( env->nomTemp, env->accumulateur.str() );
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = LISTE_ATTRIBUTS;
    }
    else
    {
        env->accumulateur << c;
    }
}



void
fctTA_ATT_GUILLEMET( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "ATT_GUILLEMET" )
    if( '"' == c )
    {
        _DEBUG_STRUCT_( env->nomTemp << "=\"" << env->accumulateur.str() << "\"" )
        assert( nullptr != env );
        assert( nullptr != env->courant );
        env->courant->ajouterAttribut( env->nomTemp, env->accumulateur.str() );
        assert( nullptr != env );
        env->accumulateur.str( "" );
        etat = LISTE_ATTRIBUTS;
    }
    else
    {
        assert( nullptr != env );
        env->accumulateur << c;
    }
}



void
fctTA_PI_DEBUT( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "PI_DEBUT" )
    if( '?' == c )
    {
        etat = PI_FIN_POSSIBLE;
    }
}



void
fctTA_PI_FIN_POSSIBLE( Etat & etat, char c, Environnement * env )
{
    _DEBUG_STATE_( "PI_FIN_POSSIBLE" )
    if( '>' == c )
    {
        etat = TEXTE;
    }
}


const FonctionTransitionAction fonctionsTransitionAction[] =
{
    fctTA_TEXTE,
    fctTA_OUVERTURE_TAG,
    fctTA_NOM_DEBUT_ELEMENT,
    fctTA_NOM_FIN_PREMIER,
    fctTA_LISTE_ATTRIBUTS,
    fctTA_ATTRIBUT_NOM,
    fctTA_FIN_TAG_COMPLET,
    fctTA_ATT_EGAL,
    fctTA_ATT_APOSTROPHE,
    fctTA_ATT_GUILLEMET,
    fctTA_PI_DEBUT,
    fctTA_PI_FIN_POSSIBLE
};


DocumentXML *
lireFichierXML( string a_nom )
{
    Etat etat = TEXTE;
    ifstream fichier( a_nom );
    DocumentXML * document = new DocumentXML();
    char c;
    assert( nullptr != document );
    Environnement * env = new Environnement( document->racine() );

    fichier.get( c );
    _DEBUG_STATE_( c << " : " )

    while( ! fichier.eof() )
    {
        assert( 0 <= etat );
        assert( etat <= PI_FIN_POSSIBLE );
        assert( nullptr != fonctionsTransitionAction[ etat ] );
        fonctionsTransitionAction[ etat ]( etat, c, env );

        fichier.get( c );
        _DEBUG_STATE_( c << " : " )
    }

    return document;
}

