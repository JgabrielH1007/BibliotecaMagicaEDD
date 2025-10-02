#include "ArbolB.h"
#include <cstddef>
#include <iostream>
#include <vector>
#include "ListaLibro.h"
#include "NodoB.h"
#include "Libro.h"
#include <fstream>
using namespace std;

ArbolB::ArbolB(int _grado) {
    raiz = nullptr;
    grado = _grado;
}

void ArbolB::buscar(string genero, ListaLibro &resultados) {
    if (raiz != nullptr) {
        raiz->buscarPorGenero(genero, resultados);
    }
}

void ArbolB::insertar(Libro* libro) {
    if (raiz == nullptr) {
        raiz = new nodoB(grado, true);
        raiz->llaves.push_back(libro);
        return; 
    } 
    if ((int)raiz->llaves.size() == 2 * grado - 1) {
        nodoB* nuevo = new nodoB(grado, false);
        nuevo->hijos.push_back(raiz);
        nuevo->dividir(0, raiz);
        int i = 0;
        if (nuevo->llaves[0]->getGenero().compare(libro->getGenero()) < 0) ++i;
        nuevo->hijos[i]->insertarNoLleno(libro);
        raiz = nuevo;
    } else {
        raiz->insertarNoLleno(libro);
    }
    
}

void ArbolB::eliminar(string genero, Libro* libroEspecifico) {
    if (!raiz) {
        cout << "El árbol está vacío. No se puede eliminar.\n";
        return;
    }

    raiz->eliminar(genero, libroEspecifico);

    if (raiz->llaves.empty()) {
        nodoB* temp = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete temp;
    }
}


void ArbolB::inOrden() {
    if (raiz){
        raiz->inOrden(); cout << endl;
    } else {
        cout << "El árbol está vacío.\n";
    }
}

void ArbolB::generarGraphvizRec(nodoB* nodo, ofstream &archivo) {
    if (!nodo) return;

    archivo << "node" << nodo << " [label=\"";
    for (size_t i = 0; i < nodo->llaves.size(); ++i) {
        archivo << nodo->llaves[i]->getGenero();
        if (i < nodo->llaves.size() - 1) archivo << " | ";
    }
    archivo << "\"];\n";

    for (size_t i = 0; i < nodo->hijos.size(); ++i) {
        if (nodo->hijos[i]) {
            archivo << "node" << nodo << " -> node" << nodo->hijos[i] << ";\n";
            generarGraphvizRec(nodo->hijos[i], archivo);
        }
    }
}

void ArbolB::generarGraphviz(const string &nombreArchivoDot, const string &nombreArchivoImagen) {
    ofstream archivo(nombreArchivoDot);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para escribir.\n";
        return;
    }

    archivo << "digraph G {\n";
    archivo << "node [shape=record];\n";
    generarGraphvizRec(raiz, archivo);
    archivo << "}\n";
    archivo.close();

    string comando = "dot -Tpng " + nombreArchivoDot + " -o " + nombreArchivoImagen;
    system(comando.c_str());
}
