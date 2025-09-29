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

        int encontrarIndice(const string& titulo) {
            int idx = 0;
            while (idx < (int)claves.size() && claves[idx]->getTitulo() < titulo) {
                idx++;
            }
            return idx;
        }
};

#endif 