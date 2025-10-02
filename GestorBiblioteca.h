#ifndef GESTOR_BIBLIOTECA
#define GESTOR_BIBLIOTECA
#include "Libro.h"
#include "ListaLibro.h"
#include "ArbolAVL.h"
#include "ArbolB.h"
#include "ArbolB+.h"
#include <string>

class GestorBiblioteca{
    private:
        int duracionBusquedaBISBN = 0;
        int duracionBusquedaBTitulo = 0;
        int duracionBusquedaSISBN = 0;
        int duracionBusquedaSTitulo = 0;
        ListaLibro listaLibros;
        ArbolAVL arbolISBN;
        ArbolAVL arbolTitulo;
        ArbolB arbolGenero{3};
        ArbolBPlus arbolAnio{3};
    public: 
        void bienvenidaInicial();
        void menuBiblioteca();
        void realizarAccion(int& opcion);
        void menuFinAccion();
        void cargarLibros(Libro* libro);
        Libro* crearLibro(string linea);
        bool validarISBN(string isbn);
        long long int convertirISBN(string isbn);
        void leerArchivo();
        void buscarLibro();
        int validarOpcion(int minimo, int maximo);
        void eliminarLibro();
        void mostrarLibros();
        void generarGraphvizArboles();
        void buscarArbolB();
        void compararTiempos();
        void cargarLibroManual();
        void buscarPorRangoAnio();
};

#endif 