#ifndef NODOB_H
#define NODOB_H
#include <vector>
#include "Libro.h"
#include <string>
using namespace std;


class NodoB {
    public:
        bool esHoja;
        vector<Libro*> claves;
        vector<NodoB*> hijos;
        NodoB(bool esHoja = true) : esHoja(esHoja) {}

        int encontrarIndice(const string& genero) {
            int idx = 0;
            while (idx < (int)claves.size() && claves[idx]->getGenero() < genero) {
                idx++;
            }
            return idx;
        }
};

#endif 