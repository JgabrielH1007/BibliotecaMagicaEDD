#ifndef ARBOLB_H
#define ARBOLB_H
#include "NodoB.h"
#include "Libro.h"
#include "ListaLibro.h"
#include <iostream>
#include <list>
using namespace std;

class ArbolB {
    public:
        nodoB* raiz;
        int grado;
        ArbolB(int _grado);
        void buscar(string genero, ListaLibro &resultados);
        void insertar(Libro* libro);
        void eliminar(string genero, Libro* libroEspecifico);
        void inOrden();
        void generarGraphvizRec(nodoB* nodo, ofstream &archivo);
        void generarGraphviz(const string &nombreArchivoDot, const string &nombreArchivoImagen);
    };
    

#endif