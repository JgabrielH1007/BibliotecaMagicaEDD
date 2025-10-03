#ifndef NODOB_H
#define NODOB_H
#include <vector>
#include <iostream>
#include "Libro.h"
#include "ListaLibro.h"
#include <string>
using namespace std;

class nodoB {
    public:
        int grado;
        bool esHoja;
        vector<Libro*> llaves;
        vector<nodoB*> hijos;
    
        nodoB(int _grado, bool _esHoja) : grado(_grado), esHoja(_esHoja) { }
    
        void inOrden() {
            int i;
            for (i = 0; i < (int)llaves.size(); ++i) {
                if (!esHoja) hijos[i]->inOrden();
                cout << llaves[i]->getGenero() << " - " << llaves[i]->getTitulo()
                     << " (ISBN: " << llaves[i]->getISBN() << ")\n";
            }
            if (!esHoja) hijos[i]->inOrden();
        }

        void buscarPorGenero(const string &genero, ListaLibro &resultados) {
            int i;
            for (i = 0; i < (int)llaves.size(); ++i) {
                if (!esHoja) hijos[i]->buscarPorGenero(genero, resultados);
                if (llaves[i]->getGenero() == genero)
                    resultados.agregarLibro(llaves[i]);
            }
            if (!esHoja) hijos[i]->buscarPorGenero(genero, resultados);
        }
        
        void insertarNoLleno(Libro* libro) {
            string key = libro->getGenero();
            int i = (int)llaves.size() - 1;
        
            if (esHoja) {
                while (i >= 0 && llaves[i]->getGenero().compare(key) > 0) --i;
                llaves.insert(llaves.begin() + i + 1, libro);
            } else {
                while (i >= 0 && llaves[i]->getGenero().compare(key) > 0) --i;
                i++;
                if ((int)hijos[i]->llaves.size() == 2 * grado - 1) {
                    dividir(i, hijos[i]);
                    if (llaves[i]->getGenero().compare(key) < 0) ++i;
                }
                hijos[i]->insertarNoLleno(libro);
            }
        }
        
        void dividir(int indice, nodoB* hijo) {
            Libro* midObj = hijo->llaves[grado - 1];
        
            nodoB* nuevo = new nodoB(hijo->grado, hijo->esHoja);
        
            for (int j = 0; j < grado - 1; ++j)
                nuevo->llaves.push_back(hijo->llaves[j + grado]);
        
            if (!hijo->esHoja) {
                for (int j = 0; j < grado; ++j)
                    nuevo->hijos.push_back(hijo->hijos[j + grado]);
            }
        
            hijo->llaves.resize(grado - 1);
            if (!hijo->esHoja) hijo->hijos.resize(grado);
        
            hijos.insert(hijos.begin() + indice + 1, nuevo);
        
            llaves.insert(llaves.begin() + indice, midObj);
        }               


        int encontrarLlave(const string &genero) {
            for (int i = 0; i < (int)llaves.size(); i++) {
                if (llaves[i]->getGenero() == genero)
                    return i; // posición exacta
            }
            return -1; 
        }
        
        
        void fusionar(int idx) {
            nodoB* hijo = hijos[idx];
            nodoB* hermano = hijos[idx + 1];
        
            hijo->llaves.push_back(llaves[idx]);
        
            for (int i = 0; i < (int)hermano->llaves.size(); ++i)
                hijo->llaves.push_back(hermano->llaves[i]);
        
            if (!hijo->esHoja) {
                for (int i = 0; i < (int)hermano->hijos.size(); ++i)
                    hijo->hijos.push_back(hermano->hijos[i]);
            }
        
            llaves.erase(llaves.begin() + idx);
            hijos.erase(hijos.begin() + idx + 1);
        
            delete hermano;
        }
    

        bool prestarDeAnterior(int idx) {
            nodoB* hijo = hijos[idx];
            nodoB* hermano = hijos[idx - 1];
        
            hijo->llaves.insert(hijo->llaves.begin(), llaves[idx - 1]);
        
            if (!hijo->esHoja)
                hijo->hijos.insert(hijo->hijos.begin(), hermano->hijos.back());
        
            llaves[idx - 1] = hermano->llaves.back();
        
            hermano->llaves.pop_back();
            if (!hermano->esHoja) hermano->hijos.pop_back();
        
            return true;
        }

        bool prestarDeSiguiente(int idx) {
            nodoB* hijo = hijos[idx];
            nodoB* hermano = hijos[idx + 1];
        
            hijo->llaves.push_back(llaves[idx]);
        
            if (!hijo->esHoja)
                hijo->hijos.push_back(hermano->hijos.front());
        
            llaves[idx] = hermano->llaves.front();
        
            hermano->llaves.erase(hermano->llaves.begin());
            if (!hermano->esHoja) hermano->hijos.erase(hermano->hijos.begin());
        
            return true;
        }

        void llenar(int idx) {
            if (idx != 0 && (int)hijos[idx - 1]->llaves.size() >= grado) {
                prestarDeAnterior(idx);
            }
            else if (idx != (int)llaves.size() && (int)hijos[idx + 1]->llaves.size() >= grado) {
                prestarDeSiguiente(idx);
            }
            else {
                if (idx != (int)llaves.size()) fusionar(idx);
                else fusionar(idx - 1);
            }
        }        

        void eliminarDeHoja(int idx) {
            llaves.erase(llaves.begin() + idx);
        }   

        Libro* obtenerPredecesor(int idx) {
            nodoB* cur = hijos[idx];
            while (!cur->esHoja) cur = cur->hijos.back();
            return cur->llaves.back(); 
        }

        Libro* obtenerSucesor(int idx) {
            nodoB* cur = hijos[idx + 1];
            while (!cur->esHoja) cur = cur->hijos.front();
            return cur->llaves.front();
        }

        void eliminarDeNoHoja(int idx) {
            Libro* objetivo = llaves[idx];
            string key = objetivo->getGenero();
        
            if ((int)hijos[idx]->llaves.size() >= grado) {
                Libro* predObj = obtenerPredecesor(idx);
                llaves[idx] = predObj;
                hijos[idx]->eliminar(predObj->getGenero());
            }
            else if ((int)hijos[idx + 1]->llaves.size() >= grado) {
                Libro* succObj = obtenerSucesor(idx);
                llaves[idx] = succObj;
                hijos[idx + 1]->eliminar(succObj->getGenero());
            }
            else {
                fusionar(idx);
                hijos[idx]->eliminar(key);
            }
        }

        void eliminar(const string &genero, Libro* libroEspecifico = nullptr) {
            int index = encontrarLlave(genero);
                if (index != -1) {
                    while (index < (int)llaves.size() && llaves[index]->getGenero() == genero) {
                        if (libroEspecifico == nullptr || llaves[index]->getISBN() == libroEspecifico->getISBN()) {
                            if (esHoja)
                                eliminarDeHoja(index);
                            else
                                eliminarDeNoHoja(index);
                            return;
                        }
                        ++index;
                    }
                } else {
                    if (esHoja) {
                        cout << "La clave (género '" << genero << "') no existe en el árbol.\n";
                        return;
                    }
                    int indiceHijo = 0;
                    while (indiceHijo < (int)llaves.size() && llaves[indiceHijo]->getGenero() < genero)
                        ++indiceHijo;

                    if ((int)hijos[indiceHijo]->llaves.size() < grado)
                        llenar(indiceHijo);

                    hijos[indiceHijo]->eliminar(genero, libroEspecifico);
                }

        }
        
        
    };
#endif 
