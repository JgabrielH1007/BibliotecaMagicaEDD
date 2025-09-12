#include "ListaLibro.h"
#include "NodoLista.h"
#include "Libro.h"
#include <iostream>

ListaLibro::ListaLibro() : inicio(nullptr),tamanio(0){}

void ListaLibro::agregarLibro(Libro* libro) {
    if (libro == nullptr) {
        return;
    }
    NodoLista* nuevoNodo = new NodoLista();
    nuevoNodo->libro = libro;
    nuevoNodo->siguiente = nullptr; 

    if (estaVacia()) {
        inicio = nuevoNodo;
    } else {
        NodoLista* actual = inicio;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoNodo;
    }
    tamanio++;
}

void ListaLibro::eliminarLibro(string isbn) {
    if (estaVacia()) {
        cout << "La lista está vacía." << endl;
        return;
    }
    NodoLista* actual = inicio;
    NodoLista* anterior = nullptr;
    while (actual != nullptr && actual->libro->getISBN() != isbn) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == nullptr) {
        cout << "Libro con ISBN " << isbn << " no encontrado." << endl;
        return;
    }

    if (anterior == nullptr) {
        inicio = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }

    delete actual->libro;
    delete actual;
    tamanio--;

    cout << "Libro con ISBN " << isbn << " eliminado correctamente." << endl;
}

void ListaLibro::imprimirLista() {
    if (estaVacia()) {
        cout << "La lista está vacía." << endl;
        return;
    }

    NodoLista* actual = inicio;
    while (actual != nullptr) {
        cout << "Título: " << actual->libro->getTitulo()
             << " | ISBN: " << actual->libro->getISBN()
             << " | Género: " << actual->libro->getGenero()
             << " | Año: " << actual->libro->getAnio()
             << " | Autor: " << actual->libro->getAutor()
             << endl;
        actual = actual->siguiente;
    }
}

Libro* ListaLibro::buscarPorIsbn(string isbn){
    NodoLista* actual = inicio;
    while (actual != nullptr) {
        if (actual->libro->getISBN() == isbn) {
            return actual->libro;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Libro* ListaLibro::buscarPorTitulo(string titulo){
    NodoLista* actual = inicio;
    while (actual != nullptr) {
        if (actual->libro->getTitulo() == titulo) {
            return actual->libro;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

bool ListaLibro::estaVacia(){
    return tamanio==0;
}

int ListaLibro::getTamanio(){
    return tamanio;
}