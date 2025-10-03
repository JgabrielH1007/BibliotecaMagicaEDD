#ifndef NODOBPLUS_H
#define NODOBPLUS_H
#include <vector>
#include "Libro.h"
#include <string>
using namespace std;

class NodoBPlus {
    public:
        bool esHoja;
        vector<int> llaves;
        vector<NodoBPlus*> hijos;
        vector<vector<Libro*>> registros; 
        NodoBPlus* siguiente; 
    
        NodoBPlus(bool hoja = false) : esHoja(hoja), siguiente(nullptr) {}

    };

    
#endif