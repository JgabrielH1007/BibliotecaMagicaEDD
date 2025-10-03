#include "ArbolAVL.h"
#include "NodoAVL.h"
#include "Libro.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

NodoAVL* ArbolAVL::rotacionDD(NodoAVL* ref){
    NodoAVL* nodo = ref;
    NodoAVL* nodo1 = ref->getDerecho();
    nodo->setDerecho(nodo1->getIzquierdo());
    nodo1->setIzquierdo(nodo);
    actualizarAlturaYFactor(nodo);
    actualizarAlturaYFactor(nodo1);
    return nodo1;   
}

NodoAVL* ArbolAVL::rotacionII(NodoAVL* ref){
    NodoAVL* nodo = ref;
    NodoAVL* nodo1 = ref->getIzquierdo();
    nodo->setIzquierdo(nodo1->getDerecho());
    nodo1->setDerecho(nodo);
    actualizarAlturaYFactor(nodo);
    actualizarAlturaYFactor(nodo1);
    return nodo1;   
}

NodoAVL* ArbolAVL::rotacionDI(NodoAVL* ref){
    NodoAVL* nodo = ref;
    NodoAVL* nodo1 = ref->getDerecho();
    NodoAVL* nodo2 = nodo1->getIzquierdo();
    nodo->setDerecho(nodo2->getIzquierdo());
    nodo1->setIzquierdo(nodo2->getDerecho());
    nodo2->setIzquierdo(nodo);
    nodo2->setDerecho(nodo1);
    actualizarAlturaYFactor(nodo);
    actualizarAlturaYFactor(nodo1);
    actualizarAlturaYFactor(nodo2);
    return nodo2;   
}

NodoAVL* ArbolAVL::rotacionID(NodoAVL* ref){
    NodoAVL* nodo = ref;
    NodoAVL* nodo1 = ref->getIzquierdo();
    NodoAVL* nodo2 = nodo1->getDerecho();
    nodo->setIzquierdo(nodo2->getDerecho());
    nodo1->setDerecho(nodo2->getIzquierdo());
    nodo2->setDerecho(nodo);
    nodo2->setIzquierdo(nodo1);
    actualizarAlturaYFactor(nodo);
    actualizarAlturaYFactor(nodo1);
    actualizarAlturaYFactor(nodo2);
    return nodo2;   
}

int ArbolAVL::obtenerAltura(NodoAVL* nodo){
    if(nodo == nullptr){
        return 0;
    }
    return nodo->getAltura();
}

int ArbolAVL::obtenerFactorEquilibrio(NodoAVL* nodo){
    if(nodo == nullptr){
        return 0;
    }
    return obtenerAltura(nodo->getIzquierdo()) - obtenerAltura(nodo->getDerecho());
}

void ArbolAVL::actualizarAlturaYFactor(NodoAVL* nodo){
    if(nodo != nullptr){
        nodo->setAltura(1 + max(obtenerAltura(nodo->getIzquierdo()), obtenerAltura(nodo->getDerecho())));
        nodo->setFactorEquilibrio(obtenerFactorEquilibrio(nodo));
    }
}

NodoAVL* ArbolAVL::insertarRecursivoPorTitulo(NodoAVL* nodo, string titulo, Libro* libro){
    if(nodo == nullptr){
        NodoAVL* nuevoNodo = new NodoAVL();
        nuevoNodo->setLibro(libro);
        return nuevoNodo;
    }
    
    if(titulo < nodo->getLibro()->getTitulo()){
        nodo->setIzquierdo(insertarRecursivoPorTitulo(nodo->getIzquierdo(), titulo, libro));
    }else if(titulo > nodo->getLibro()->getTitulo()){
        nodo->setDerecho(insertarRecursivoPorTitulo(nodo->getDerecho(), titulo, libro));
    }else{
        return nodo;
    }

    actualizarAlturaYFactor(nodo);

    if(nodo->getFactorEquilibrio() > 1 && titulo < nodo->getIzquierdo()->getLibro()->getTitulo()){
        return rotacionII(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && titulo > nodo->getDerecho()->getLibro()->getTitulo()){
        return rotacionDD(nodo);
    }
    if(nodo->getFactorEquilibrio() > 1 && titulo > nodo->getIzquierdo()->getLibro()->getTitulo()){
        return rotacionID(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && titulo < nodo->getDerecho()->getLibro()->getTitulo()){
        return rotacionDI(nodo);
    }

    return nodo;
}

NodoAVL* ArbolAVL::insertarRecursivoPorIsbn(NodoAVL* nodo, long long int isbn, Libro* libro){
    if(nodo == nullptr){
        NodoAVL* nuevoNodo = new NodoAVL();
        nuevoNodo->setLibro(libro);
        return nuevoNodo;
    }

    if(isbn < nodo->getLibro()->getISBNNum()){
        nodo->setIzquierdo(insertarRecursivoPorIsbn(nodo->getIzquierdo(), isbn, libro));
    }else if(isbn > nodo->getLibro()->getISBNNum()){
        nodo->setDerecho(insertarRecursivoPorIsbn(nodo->getDerecho(), isbn, libro));
    }else{
        return nodo;
    }

    actualizarAlturaYFactor(nodo);

    if(nodo->getFactorEquilibrio() > 1 && isbn < nodo->getIzquierdo()->getLibro()->getISBNNum()){
        return rotacionII(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && isbn > nodo->getDerecho()->getLibro()->getISBNNum()){
        return rotacionDD(nodo);
    }
    if(nodo->getFactorEquilibrio() > 1 && isbn > nodo->getIzquierdo()->getLibro()->getISBNNum()){
        return rotacionID(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && isbn < nodo->getDerecho()->getLibro()->getISBNNum()){
        return rotacionDI(nodo);
    }

    return nodo;
}

void ArbolAVL::insertarPorIsbn(long long int isbn, Libro* libro){
    raiz = insertarRecursivoPorIsbn(raiz, isbn, libro);
}

void ArbolAVL::insertarPorTitulo(string titulo, Libro* libro){
    raiz = insertarRecursivoPorTitulo(raiz, titulo, libro);
}

NodoAVL* ArbolAVL::buscarPorTituloRecursivo(NodoAVL* nodo, string titulo){
    if(nodo == nullptr || nodo->getLibro()->getTitulo() == titulo){
        return nodo;
    }
    if(titulo < nodo->getLibro()->getTitulo()){
        return buscarPorTituloRecursivo(nodo->getIzquierdo(), titulo);
    }
    return buscarPorTituloRecursivo(nodo->getDerecho(), titulo);
}

NodoAVL* ArbolAVL::buscarPorIsbnRecursivo(NodoAVL* nodo, long long int isbn){
    if(nodo == nullptr || nodo->getLibro()->getISBNNum() == isbn){
        return nodo;
    }
    if(isbn < nodo->getLibro()->getISBNNum()){
        return buscarPorIsbnRecursivo(nodo->getIzquierdo(), isbn);
    }
    return buscarPorIsbnRecursivo(nodo->getDerecho(), isbn);
}

NodoAVL* ArbolAVL::eliminarRecursivoTitulo(NodoAVL* nodo, string titulo){
    if(nodo == nullptr){
        return nodo;
    }
    if(titulo < nodo->getLibro()->getTitulo()){
        nodo->setIzquierdo(eliminarRecursivoTitulo(nodo->getIzquierdo(), titulo));
    }else if(titulo > nodo->getLibro()->getTitulo()){
        nodo->setDerecho(eliminarRecursivoTitulo(nodo->getDerecho(), titulo));
    }else{
        if (nodo->getIzquierdo() == nullptr || nodo->getDerecho() == nullptr) {
            NodoAVL* temp = nodo->getIzquierdo() ? nodo->getIzquierdo() : nodo->getDerecho();
            if (temp == nullptr) {
                delete nodo;
                nodo = nullptr;
            } else {
                NodoAVL* viejo = nodo;
                nodo = temp;
                delete viejo;
            }
        }else{
            NodoAVL* temp = nodo->getDerecho();
            while(temp->getIzquierdo() != nullptr){
                temp = temp->getIzquierdo();
            }
            nodo->setLibro(temp->getLibro());
            nodo->setDerecho(eliminarRecursivoTitulo(nodo->getDerecho(), temp->getLibro()->getTitulo()));
        }
    }

    if(nodo == nullptr){
        return nodo;
    }

    actualizarAlturaYFactor(nodo);

    if(nodo->getFactorEquilibrio() > 1 && obtenerFactorEquilibrio(nodo->getIzquierdo()) >= 0){
        return rotacionII(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && obtenerFactorEquilibrio(nodo->getDerecho()) <= 0){
        return rotacionDD(nodo);
    }
    if(nodo->getFactorEquilibrio() > 1 && obtenerFactorEquilibrio(nodo->getIzquierdo()) < 0){
        return rotacionID(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && obtenerFactorEquilibrio(nodo->getDerecho()) > 0){
        return rotacionDI(nodo);
    }

    return nodo;
}

NodoAVL* ArbolAVL::eliminarRecursivoIsbn(NodoAVL* nodo, long long int isbn){
    if(nodo == nullptr){
        return nodo;
    }
    if(isbn < nodo->getLibro()->getISBNNum()){
        nodo->setIzquierdo(eliminarRecursivoIsbn(nodo->getIzquierdo(), isbn));
    }else if(isbn > nodo->getLibro()->getISBNNum()){
        nodo->setDerecho(eliminarRecursivoIsbn(nodo->getDerecho(), isbn));
    }else{
        if (nodo->getIzquierdo() == nullptr || nodo->getDerecho() == nullptr) {
            NodoAVL* temp = nodo->getIzquierdo() ? nodo->getIzquierdo() : nodo->getDerecho();
            if (temp == nullptr) {
                delete nodo;
                nodo = nullptr;
            } else {
                NodoAVL* viejo = nodo;
                nodo = temp;
                delete viejo;
            }
        }else{
            NodoAVL* temp = nodo->getDerecho();
            while(temp->getIzquierdo() != nullptr){
                temp = temp->getIzquierdo();
            }
            nodo->setLibro(temp->getLibro());
            nodo->setDerecho(eliminarRecursivoIsbn(nodo->getDerecho(), temp->getLibro()->getISBNNum()));
        }
    }

    if(nodo == nullptr){
        return nodo;
    }

    actualizarAlturaYFactor(nodo);

    if(nodo->getFactorEquilibrio() > 1 && obtenerFactorEquilibrio(nodo->getIzquierdo()) >= 0){
        return rotacionII(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && obtenerFactorEquilibrio(nodo->getDerecho()) <= 0){
        return rotacionDD(nodo);
    }
    if(nodo->getFactorEquilibrio() > 1 && obtenerFactorEquilibrio(nodo->getIzquierdo()) < 0){
        return rotacionID(nodo);
    }
    if(nodo->getFactorEquilibrio() < -1 && obtenerFactorEquilibrio(nodo->getDerecho()) > 0){
        return rotacionDI(nodo);
    }

    return nodo;
}

NodoAVL* ArbolAVL::buscarPorTitulo(string titulo){
    return buscarPorTituloRecursivo(raiz, titulo);
}
NodoAVL* ArbolAVL::buscarPorIsbn(long long int isbn){
    return buscarPorIsbnRecursivo(raiz, isbn);
}
void ArbolAVL::eliminarTitulo(string titulo){
    raiz = eliminarRecursivoTitulo(raiz, titulo);
}

void ArbolAVL::eliminarIsbn(long long int isbn){
    raiz = eliminarRecursivoIsbn(raiz, isbn);
}

void ArbolAVL::inordenRecursivo(NodoAVL* nodo){
    if(nodo != nullptr){
        inordenRecursivo(nodo->getIzquierdo());
        cout << "Titulo: " << nodo->getLibro()->getTitulo() << ", ISBN: " << nodo->getLibro()->getISBN() << ", Genero: " << nodo->getLibro()->getGenero() << ", Anio: " << nodo->getLibro()->getAnio() << ", Autor: " << nodo->getLibro()->getAutor() << endl;
        inordenRecursivo(nodo->getDerecho());
    }
}
void ArbolAVL::inorden(){
    inordenRecursivo(raiz);
}

void ArbolAVL::generarGraphvizRecursivo(NodoAVL* nodo, ofstream& archivo) {
    if (nodo != nullptr) {
        archivo << "    \"" << nodo->getLibro()->getTitulo() 
                << "\" [label=\"" << nodo->getLibro()->getTitulo() 
                << "\\nISBN: " << nodo->getLibro()->getISBN() << "\"];" << endl;

        if (nodo->getIzquierdo() != nullptr) {
            archivo << "    \"" << nodo->getLibro()->getTitulo() 
                    << "\" -> \"" << nodo->getIzquierdo()->getLibro()->getTitulo() << "\";" << endl;
            generarGraphvizRecursivo(nodo->getIzquierdo(), archivo);
        }

        if (nodo->getDerecho() != nullptr) {
            archivo << "    \"" << nodo->getLibro()->getTitulo() 
                    << "\" -> \"" << nodo->getDerecho()->getLibro()->getTitulo() << "\";" << endl;
            generarGraphvizRecursivo(nodo->getDerecho(), archivo);
        }
    }
}

void ArbolAVL::generarGraphviz(const string& nombreArchivoDOT, const string& nombreImagen) {
    ofstream archivo(nombreArchivoDOT);
    if (archivo.is_open()) {
        archivo << "digraph G {" << endl;
        archivo << "    node [shape=box];" << endl;
        generarGraphvizRecursivo(raiz, archivo);
        archivo << "}" << endl;
        archivo.close();

        string comando = "dot -Tsvg " + nombreArchivoDOT + " -o " + nombreImagen;
        int resultado = system(comando.c_str());
        if (resultado == 0) {
        } else {
            cout << "Error al generar la imagen con Graphviz." << endl;
        }
    } else {
        cout << "No se pudo abrir el archivo DOT para escribir." << endl;
    }
}

