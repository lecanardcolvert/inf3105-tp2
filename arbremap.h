/* UQAM / Département d'informatique
   INF3105 - Structures de données et algorithmes
   Squelette pour classe générique ArbreMap<K,V> pour le Lab8 et le TP2.

   AUTEUR:
   Alexandre H. Bourdeau (HAMA12128907)
*/

#if !defined(__ARBREMAP_H__)
#define __ARBREMAP_H__

#include "arbreavl.h"

template <class K, class V>
class ArbreMap {
public:
    // Annonce l'existance d'une classe Iterateur.
    class Iterateur; 
    
    void inserer(const K& c, const V& v);
    void enlever(const K&);
    bool contient(const K&) const;
    unsigned int nEntrees() const;
    void vider();
    bool vide() const;
    const V& operator[] (const K&) const;
    V& operator[] (const K&);

    // Accès aux éléments de l'arbre via un intérateur.
    const V& operator[](const Iterateur&) const;
    V& operator[](const Iterateur&);
    
    private:
        class Entree {
        public:
            Entree(const K& c) : cle(c), valeur() {}
            Entree(const K& c, const V& v) : cle(c), valeur(v) {}
            K cle;
            V valeur;
            bool operator < (const Entree& e) const {
                return cle < e.cle;
            }
        };
        ArbreAVL<Entree> entrees;
        unsigned int taille;
    
    public:
        // Fonctions pour obtenir un itérateur (position dans l'arbre)    
        Iterateur debut() const;
        Iterateur fin() const;
        Iterateur rechercher(const K&) const;
        
        class Iterateur {
        public:
            Iterateur(const ArbreMap& a) : iterateur(a.entrees.debut()) {};
            Iterateur(typename ArbreAVL<Entree>::Iterateur i) : iterateur(i) {};
            
            const K& cle() const;
            const V& valeur();

            operator bool() const;
            bool operator!() const;
            bool operator==(const Iterateur&) const;
            bool operator!=(const Iterateur&) const;
            const K& operator*() const;

            Iterateur& operator++();
            Iterateur operator++(int);
            Iterateur& operator = (const Iterateur&);

        private:
            typename ArbreAVL<Entree>::Iterateur iterateur;
            
        friend class ArbreMap;
        };
};

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    // À compléter
    this->taille--;
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    Entree entree(c);
    return entrees.contient(c);
}

template <class K, class V>
unsigned int ArbreMap<K,V>::nEntrees() const {
    return this->taille;
}

template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c)  const
{
    typename ArbreAVL<Entree>::Iterateur iter = entrees.rechercher(c);
    
    return entrees[iter].valeur;

}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    typename ArbreAVL<Entree>::Iterateur iter=entrees.rechercher(Entree(c));
    
    if (!iter) {
        entrees.inserer(Entree(c));
        iter = entrees.rechercher(c);
        taille++;
    }
    
    return entrees[iter].valeur;
}

// Fonctions pour obtenir un itérateur (position dans l'arbre)

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::debut() const {
    return Iterateur(*this);
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::fin() const
{
    return Iterateur((entrees.fin()));
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::rechercher(const K& c) const
{

    return Iterateur(entrees.rechercher(c));
}

template <class K, class V>
const K& ArbreMap<K,V>::Iterateur::cle() const {
    return (*iterateur).cle;
}

template <class K, class V>
const V& ArbreMap<K,V>::Iterateur::valeur() {
    return (*iterateur).valeur;
}

template <class K, class V>
ArbreMap<K,V>::Iterateur::operator bool() const
{
    return iterateur.operator bool();
}

// Post-incrément
template <class K, class V>
typename ArbreMap<K,V>::Iterateur& ArbreMap<K,V>::Iterateur::operator++() {
    iterateur++;
    return *this;
}

template <class K, class V>
typename ArbreMap<K,V>::Iterateur ArbreMap<K,V>::Iterateur::operator++(int) {
    Iterateur copie(*this);
    operator++();
    return copie;
}

#endif
