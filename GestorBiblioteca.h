#ifndef GESTOR_BIBLIOTECA
#define GESTOR_BIBLIOTECA
#include "Libro.h"
#include "ListaLibro.h"

class GestorBiblioteca{
    private:
        ListaLibro listaLibros;
    
    public: 
        void bienvenidaInicial();
        void menuJuego();
        void realizarAccion(int& opcion);
        void menuFinAccion();
        void cargarLibros(Libro* libro);
        Libro* crearLibro(string linea);
        bool validarISBN(string isbn);
        void leerArchivo();

};

#endif 