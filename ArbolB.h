#ifndef ARBOLB_H
#define ARBOLB_H
#include "NodoB.h"
#include "Libro.h"
#include "ListaLibro.h"
#include <iostream>
using namespace std;

class ArbolB {
    private:
        NodoB* raiz;
        int t;
        void dividirHijo(NodoB* padre, int indice, NodoB* hijo);
        void insertarNoLleno(NodoB* nodo, Libro* libro);
        void eliminar(NodoB* nodo, string genero);
        int encontrarIndice(string genero);
        void mostrar(NodoB* nodo, int nivel = 0);
        void generarGraphvizRecursivo(NodoB* nodo, ofstream& archivo, int& contador);
        void buscarPorGenero(NodoB* nodo, const std::string& genero, ListaLibro& lista, Libro*& primerEncontrado);
    public:
        ArbolB(int t);
        void insertar(Libro* libro);
        void eliminar(string genero);
        Libro* buscar(string genero, ListaLibro& lista);
        void mostrarlo();
        void generarGraphviz(const string& nombreArchivo, const string& nombreImagen);
    };
    

#endif