#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H

#include "Libro.h"
#include "ListaLibro.h"
#include <vector>
#include <string>
#include <queue>
#include <optional>

class ArbolBPlus {
public:
    explicit ArbolBPlus(int orden = 4);
    ~ArbolBPlus();
    void insertar(Libro* libro);
    void buscar(int anioInicial, int anioFinal, ListaLibro &resultados) const;
    bool eliminarPorTitulo(const std::string &titulo);
    void imprimirArbol() const;
    void imprimirHojas() const;
    void generarGraphviz(const std::string &nombreArchivoDot, const std::string &nombreArchivoImagen);

private:
    struct Nodo {
        bool esHoja;
        std::vector<int> keys;                       // claves (años)
        std::vector<std::vector<Libro*>> records;    // solo hojas: records[i] son libros con keys[i]
        std::vector<Nodo*> children;                 // solo internos
        Nodo* next;                                  // enlace entre hojas

        explicit Nodo(bool hoja) : esHoja(hoja), next(nullptr) {}
        ~Nodo() {}
    };

    Nodo* raiz;
    int orden;
    int maxKeys;
    int minKeys;

    void borrarSubArbol(Nodo* nodo);
    Nodo* encontrarHoja(Nodo* nodo, int key) const;
    std::pair<Nodo*, int> dividirHoja(Nodo* hoja);
    std::pair<Nodo*, int> dividirInterno(Nodo* node);
    void insertarEnHoja(Nodo* hoja, Libro* libro);
    void insertarRecursivo(Nodo* node, int key, Libro* libro, int &promoKey, Nodo* &promoNode, bool &splitHappened);
    void recolectarRangoDesdeHoja(Nodo* hoja, int anioInicial, int anioFinal, ListaLibro &resultados) const;
    bool eliminarEnHojasPorTitulo(Nodo* node, const std::string &titulo);
    void generarGraphvizRec(Nodo* nodo, std::ofstream &archivo);
};

#endif