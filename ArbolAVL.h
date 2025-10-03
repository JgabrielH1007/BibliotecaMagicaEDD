#ifndef ARBOLAVL_H
#define ARBOLAVL_H
#include "Libro.h"
#include "NodoAVL.h"
#include <iostream>
#include <chrono>
using namespace std;

class ArbolAVL{
    private:
        NodoAVL* raiz;
        NodoAVL* rotacionDD(NodoAVL* nodo);
        NodoAVL* rotacionII(NodoAVL* nodo);
        NodoAVL* rotacionDI(NodoAVL* nodo);
        NodoAVL* rotacionID(NodoAVL* nodo);
        int obtenerAltura(NodoAVL* nodo);
        int obtenerFactorEquilibrio(NodoAVL* nodo);
        void actualizarAlturaYFactor(NodoAVL* nodo);
        NodoAVL* insertarRecursivoPorTitulo(NodoAVL* nodo, string titulo, Libro* libro);
        NodoAVL* insertarRecursivoPorIsbn(NodoAVL* nodo, long long int isbn, Libro* libro);
        NodoAVL* buscarPorTituloRecursivo(NodoAVL* nodo, string titulo);
        NodoAVL* buscarPorIsbnRecursivo(NodoAVL* nodo, long long int isbn);
        NodoAVL* eliminarRecursivoTitulo(NodoAVL* nodo, string titulo);
        NodoAVL* eliminarRecursivoIsbn(NodoAVL* nodo, long long int isbn);
        void inordenRecursivo(NodoAVL* nodo);
        void generarGraphvizRecursivo(NodoAVL* nodo, ofstream& archivo);
    public:
        ArbolAVL();
        void insertarPorIsbn(long long int isbn, Libro* libro);
        void insertarPorTitulo(string titulo, Libro* libro);
        NodoAVL* buscarPorTitulo(string titulo);
        NodoAVL* buscarPorIsbn(long long int isbn);
        void eliminarTitulo(string titulo);
        void eliminarIsbn(long long int isbn);
        void inorden();
        void generarGraphviz(const string& nombreArchivo, const string& nombreImagen);
};

#endif