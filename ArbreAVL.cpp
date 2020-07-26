#include <string>

class ArbreAVL {
    class Noeud {
        public:
            std::string mot;
            int occurence;
            Noeud *gauche, *droite;
            bool operator <(const Noeud& noeud) {
                return mot < noeud.mot;
            }
    };
    
    ArbreAVL<Noeud> noeuds;

public:
    bool contient(const K&) const;
    void enlever(const K&);
    void vider();
    const V& operator[] (const K&) const;
    V& operator[] (const K&);
};

bool ArbreAVL;
<K,V>::contient(const K& cle) const {
    Noeud noeud(cle);
    
    return noeuds.contient(noeud);
}

const V& ArbreMap<K,V>::operator[] (const K& cle) const {
    typename ArbreAVL<Entree>::Iterate
