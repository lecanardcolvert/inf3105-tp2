#include "DocumentXML.hpp"
#include "ElementXML.hpp"
#include "Histoire.hpp"
#include "Lecteur.hpp"
#include "arbremap.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

vector<Histoire*>*
lireDocuments( string a_nomFichier ) {
    vector< Histoire * > * histoires = new vector< Histoire * >();
    DocumentXML * listeFichiers = lireFichierXML( a_nomFichier );

    assert( nullptr != listeFichiers );
    ElementXML * courant = listeFichiers->racine();
    assert( nullptr != courant );

    courant = courant->prochainElement();

    assert( nullptr != courant );
    for( NoeudXML * contenu : * courant )
    {
        assert( nullptr != contenu );
        if( contenu->estElement() )
        {
            ElementXML * element = ( ElementXML * )contenu;

            assert( nullptr != element );
            DocumentXML * doc = lireFichierXML( element->attribut( 
                    string( "fichier" ) ) );

            assert( nullptr != doc );
            vector< Histoire * > * h = extraireHistoires( * doc );

            assert( nullptr != h );
            assert( nullptr != histoires );
            histoires->insert( histoires->end(), h->begin(), h->end() );
        }
    }

    return histoires;
}

 /**
 * Calcule la fr�quence d'un mot dans une histoire
 * 
 * @param   histoire        L'histoire � analyser
 * @param   motRecherche    Le mot recherch�
 * @returns Un int pour la fr�quence du mot dans l'histoire
 */
int
calculTfMot(const Histoire& histoire, const string motRecherche) {
    int tf = 0;
    
    for (Phrase phrase : histoire) {
        for (string mot : phrase) {
            if (mot == motRecherche) {
                tf++;
            }
        }
    }
    
    return tf;
}

 /**
 * G�n�re un ArbreMap qui contient les mots et leur fr�quence, dans une histoire
 * 
 * @param   histoire    L'histoire � analyser
 * @returns Un ArbreMap qui contient pour cl� un mot de l'histoire et
 *          pour valeur leur fr�quence
 */
ArbreMap<string, int>*
calculTfHistoire(const Histoire& histoire) {
        ArbreMap<string, int>* motTfMap = new ArbreMap<string, int>();
    
    for (const Phrase phrase : histoire) {
        for (const string mot : phrase) {
            if (!motTfMap->contient(mot)) {
                int tf = calculTfMot(histoire, mot);
                (*motTfMap)[mot] = tf;
            }
        }
    }
    
    return motTfMap;
}

 /**
 * G�n�re un Vector de paires qui contiennent comme cl� une histoire et comme 
 * valeur un ArbreMap des tf (term frequency ou fr�quence des termes)
 * 
 * @param   histoires   Les histoires � analyser
 * @returns Un Vector qui contient les paires d'histoires analys�es et leur
 *          ArbreMap des tf/mot
 */
vector<pair<Histoire*, ArbreMap<string, int>*>>*
calculTfHistoires(const vector<Histoire*>& histoires) {
    
    vector<pair<Histoire*, ArbreMap<string, int>*>>* histoireMotTfTableau = 
            new vector<pair<Histoire*, ArbreMap<string, int>*>>();
    
    for (Histoire* histoire : histoires) {
        pair<Histoire*, ArbreMap<string, int>*> paire = 
                make_pair(histoire, calculTfHistoire(*histoire));
        histoireMotTfTableau->push_back(paire);
    }
    
    return histoireMotTfTableau;
}

/**
 * G�n�re un ArbreAVL qui contient l'ensemble des mots uniques de toutes les
 * histoires analys�es
 * 
 * @param   histoiresMotsTfMaps Le vector des paires d'histoires et leur
 *          ArbreMap des tf/mot
 * @returns Un ArbreAVL qui contient les mots uniques de toutes les histoires
 */
ArbreAVL<string>*
listeMotsUniques(const vector<pair<Histoire*, ArbreMap<string, int>*>>& 
        histoireMotTfTableau) {

    ArbreAVL<string>* motsUniques = new ArbreAVL<string>();
    
    // It�ration 1
    for (const auto& paire : histoireMotTfTableau) {
        ArbreMap<string, int>* motTfMap = paire.second;
        
        // It�ration 2
        for (ArbreMap<string, int>::
                Iterateur iter2 = motTfMap->debut(); iter2; iter2++) {
            
            string mot = iter2.cle();
        
            // Ajouter le mot unique � la liste si n�cessaire
            if (!motsUniques->contient(mot)) {
                motsUniques->inserer(mot);
            }
        }
    }
    
    return motsUniques;
}

 /**
 * G�n�re un ArbreMap qui contient comme cl� un mot unique et comme valeur
 * son idf (inverse document frequency, ou fr�quence inverse de document)
 * 
 * @param   histoiresMotsTfMaps Le vector des paires d'histoires et leur
 *          ArbreMap des tf/mot
 * @param   motsUniquesAvl      L'ArbreAVL des mots uniques
 * @returns Un ArbreMap qui contient pour cl� un mot et pour valeur son idf
 */
ArbreMap<string, double>*
calculIdfHistoires(const vector<pair<Histoire*, ArbreMap<string, int>*>>&
                   histoireMotTfTableau,
                   const ArbreAVL<string>& motsUniquesAvl) {
    
    ArbreMap<string, double>* motIdfMap = new ArbreMap<string, double>();
    unsigned int nombreHistoires = histoireMotTfTableau.size();

    // It�ration 1
    for (ArbreAVL<string>::
            Iterateur iter1 = motsUniquesAvl.debut(); iter1; iter1++) {
        int nombreHistoiresIncluantMot = 0;
        string motUnique = motsUniquesAvl[iter1];

        // It�ration 2
        for (const auto& paire : histoireMotTfTableau) {

            // Compter le nombre d'histoires o� le mot est trouv�
            ArbreMap<string, int>* motTfMap = paire.second;
            if (motTfMap->contient(motUnique)) {
                nombreHistoiresIncluantMot++;
                continue;
            }
        }
        
        double idf = log2(double(nombreHistoiresIncluantMot)/nombreHistoires);

        (*motIdfMap)[motUnique] = idf;
        
    }
        
    return motIdfMap;
}

 /**
 * Comparateur personnalis� qui permet de trier un tableau de paires par 
 * valeur d�croissante
 * 
 * @param   a   La premi�re paire � comparer
 * @param   b   La deuxi�me paire � comparer
 * @returns VRAI si a est plus grand que b, sinon FAUX
 */
bool
triTableauParValeurDecroissante(const pair<Histoire*, double> &a,
                                const pair<Histoire*, double> &b) {
    
    return (a.second > b.second);
}

 /**
 * G�n�re un Vector de paires qui contiennent comme cl� une histoire et comme 
 * valeur une m�trique selon les mots recherch�s
 * 
 * @param   motsRecherche       La phrase qui contient les mots recherch�s
 * @param   histoiresMotsTfMaps Le vector des paires d'histoires et leur
 *          ArbreMap des tf/mot
 * @param   motIdfMap          L'ArbreMap de tous les mots uniques et de leur
 *          fr�quence inverse de document (idf)
 * @returns Un vector qui contient les paires d'histoires et leur m�trique
 */
vector<pair<Histoire*, double>>*
calculMetrique(const Phrase& motsRecherche,
               const vector<pair<Histoire*, ArbreMap<string, int>*>>&
                    histoireMotTfTableau,
               const ArbreMap<string, double>& motIdfMap) {
    
    vector<pair<Histoire*, double>>* metriqueTableau = 
            new vector<pair<Histoire*, double>>();
    
    // It�ration 1
    for (const auto& paire : histoireMotTfTableau) {
        
        Histoire* histoire = paire.first;
        ArbreMap<string, int>* motTfMap = paire.second;
        double metriqueMots = 0.00;

        // It�ration 2 
        for (string mot : motsRecherche) {
            double idf = 0.0;
            int tf = 0;
            
            // Calculer la m�trique si le mot est dans la liste des mots uniques
            if (motIdfMap.contient(mot)) {
                idf = motIdfMap[mot];
                tf = (*motTfMap)[mot];
            }
            
            metriqueMots += (double)-1*idf*tf;
        }
        
        metriqueTableau->push_back(make_pair(histoire, metriqueMots));
    }
    
    return metriqueTableau;
}
 
 /**
 * Affiche les meilleures histoires et leur m�trique sur la sortie standard
 * 
 * @param   metriqueTableau Le vector des paires d'histoires et leur m�trique
 */
void afficherResultat(vector<pair<Histoire*, double>>& metriqueTableau) {
    int nbHistoiresPourImpression = 5;
    
    sort(metriqueTableau.begin(), metriqueTableau.end(), 
            triTableauParValeurDecroissante);
    
    // It�ration 1
    for (vector<pair<Histoire*, double>>::const_iterator iter1 = 
            metriqueTableau.begin(); iter1 != metriqueTableau.end(); iter1++) {
        
        string nomHistoire = iter1.base()->first->titre();
        double metrique = iter1.base()->second;
        
        cout << metrique << " : " << nomHistoire << endl;
        
        nbHistoiresPourImpression--;
        if (nbHistoiresPourImpression == 0) {
            break;
        }
    }
}

int main() {
    vector<Histoire*>* histoires = lireDocuments(string("listeDocument.xml"));

    vector<pair<Histoire*, ArbreMap<string, int>*>>* histoiresMotsTfMaps = 
            calculTfHistoires(*histoires);

    ArbreAVL<string>* motsUniquesAvl = listeMotsUniques(*histoiresMotsTfMaps);
    
    ArbreMap<string, double>* motIdfMap = 
        calculIdfHistoires(*histoiresMotsTfMaps, *motsUniquesAvl);
        
    delete histoires;
    delete motsUniquesAvl;
    
    string mots;
    Phrase* phrase;
    do {    
        cout << "Entrez votre requete : " << endl;
        getline(cin, mots);
        phrase = new Phrase(mots);
        
        if (phrase->nMot() > 0) {
            vector<pair<Histoire*, double>>* metriqueTableau =
                    calculMetrique(*phrase, *histoiresMotsTfMaps, *motIdfMap);
            afficherResultat(*metriqueTableau);
            delete metriqueTableau;
        }
    } while (phrase->nMot() > 0);

    delete phrase;
    delete histoiresMotsTfMaps;
    delete motIdfMap;

    return 0;
}



