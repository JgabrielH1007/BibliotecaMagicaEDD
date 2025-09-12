#ifndef LISTALIBRO_H
#define LISTALIBRO_H
#include "NodoLista.h"
#include "Libro.h"
class ListaLibro{
    private:
        NodoLista* inicio;
        int tamanio;
    public:
        ListaLibro();
        void agregarLibro(Libro* libro);
        void eliminarLibro(string isbn);
        void imprimirLista();
        Libro* buscarPorIsbn(string isbn);
        Libro* buscarPorTitulo(string titulo);
        bool estaVacia();
        int getTamanio();

};

#endif
