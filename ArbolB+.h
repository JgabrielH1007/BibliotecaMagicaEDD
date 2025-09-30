#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H
#include <vector>
#include "Libro.h"
#include <string>
#include "ListaLibro.h"
#include "NodoB+.h"
#include <unordered_map>
using namespace std;

class ArbolBPlus {
    private:
        NodoBPlus* raiz;
        int t; 
        void dividirHijo(NodoBPlus* padre, int indice);
        void dividirHoja(NodoBPlus* hoja);
        void insertarEnHoja(NodoBPlus* hoja, Libro* libro);
        NodoBPlus* buscarHoja(int anio) const;
        void generarGraphvizRecursivo(NodoBPlus* nodo, ofstream& archivo, int& contador, unordered_map<NodoBPlus*, int>& idMap);
        void encontrarPadreYPos(NodoBPlus* raiz, NodoBPlus* h, NodoBPlus*& parent, int& index);
        void clavesPromotorasRec(NodoBPlus* nodo);
    public:
        ArbolBPlus(int t);
        void insertar(Libro* libro);
        Libro* buscarPorRangoAnio(int anioInicial, int anioFinal, ListaLibro& lista) const;
        void mostrar();
        void generarGraphviz(const string& nombreArchivo, const string& nombreImagen);
        void eliminarPorTitulo(const string& titulo);
        void rebalancearInterno(NodoBPlus* nodo);
        void rebalancearHoja(NodoBPlus* hoja);
    };
 #endif