#include "ArbolB.h"
#include <iostream>
#include <vector>
#include "NodoB.h"
#include "Libro.h"
#include <fstream>
using namespace std;

ArbolB::ArbolB(int t) {
    this->raiz = nullptr;
    this->t = t;
}

void ArbolB::dividirHijo(NodoB* padre, int indice, NodoB* hijo) {
    NodoB* nuevoHijo = new NodoB(hijo->esHoja);

    Libro* medio = hijo->claves[t - 1];

    for (int j = 0; j < t - 1; j++) {
        nuevoHijo->claves.push_back(hijo->claves[j + t]);
    }

    if (!hijo->esHoja) {
        for (int j = 0; j < t; j++) {
            nuevoHijo->hijos.push_back(hijo->hijos[j + t]);
        }
    }

    hijo->claves.resize(t - 1);
    if (!hijo->esHoja) {
        hijo->hijos.resize(t);
    }

    padre->hijos.insert(padre->hijos.begin() + indice + 1, nuevoHijo);
    padre->claves.insert(padre->claves.begin() + indice, medio);
}

void ArbolB::insertarNoLleno(NodoB* nodo, Libro* libro) {
    int i = nodo->claves.size() - 1;
    if (nodo->esHoja) {
        nodo->claves.push_back(nullptr);
        while (i >= 0 && nodo->claves[i]->getGenero() > libro->getGenero()) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = libro;
    } else {
        while (i >= 0 && nodo->claves[i]->getGenero() > libro->getGenero()) {
            i--;
        }
        i++;
        if (nodo->hijos[i]->claves.size() == 2 * t - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (nodo->claves[i]->getGenero() < libro->getGenero()) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], libro);
    }
}

void ArbolB::insertar(Libro* libro) {
    if (raiz == nullptr) {
        raiz = new NodoB(true);
        raiz->claves.push_back(libro);
    } else {
        if (raiz->claves.size() == 2 * t - 1) {
            NodoB* nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos.push_back(raiz);
            dividirHijo(nuevaRaiz, 0, raiz);
            int i = 0;
            if (nuevaRaiz->claves[0]->getGenero() < libro->getGenero()) {
                i++;
            }
            insertarNoLleno(nuevaRaiz->hijos[i], libro);
            raiz = nuevaRaiz;
        } else {
            insertarNoLleno(raiz, libro);
        }
    }
}

void ArbolB::buscarPorGenero(NodoB* nodo, const std::string& genero, ListaLibro& lista, Libro*& primerEncontrado) {
    if (nodo == nullptr) return;

    int n = static_cast<int>(nodo->claves.size());

    for (int i = 0; i < n; ++i) {
        // Recorre hijo izquierdo
        if (i < static_cast<int>(nodo->hijos.size())) {
            buscarPorGenero(nodo->hijos[i], genero, lista, primerEncontrado);
        }

        // Comprueba la clave i
        if (nodo->claves[i] && nodo->claves[i]->getGenero() == genero) {
            // Agrega a la lista (ajusta el nombre del método si tu ListaLibro usa otro)
            lista.agregarLibro(nodo->claves[i]);

            // Guarda el primer encontrado si aún no hay uno
            if (primerEncontrado == nullptr) {
                primerEncontrado = nodo->claves[i];
            }
        }
    }

    // Último hijo
    if (n < static_cast<int>(nodo->hijos.size())) {
        buscarPorGenero(nodo->hijos[n], genero, lista, primerEncontrado);
    }
}

Libro* ArbolB::buscar(std::string genero, ListaLibro& lista) {

    NodoB* actual = raiz;
    Libro* primer = nullptr;

    while (actual != nullptr) {
        int i = 0;
        while (i < static_cast<int>(actual->claves.size()) && genero > actual->claves[i]->getGenero()) {
            i++;
        }
        if (i < static_cast<int>(actual->claves.size()) && genero == actual->claves[i]->getGenero()) {
            primer = actual->claves[i];
            break; // dejamos de buscar iterativamente; seguiremos con la recolección completa abajo
        }
        if (actual->esHoja) {
            break;
        } else {
            // si i está fuera de rango por seguridad, ajustamos:
            if (i < static_cast<int>(actual->hijos.size()))
                actual = actual->hijos[i];
            else
                break;
        }
    }

    buscarPorGenero(raiz, genero, lista, primer);

    return primer;
}

void ArbolB::eliminar(string genero) {
    if (raiz == nullptr) {
        cout << "El árbol está vacío." << endl;
        return;
    }
    eliminar(raiz, genero);
    if (raiz->claves.empty()) {
        NodoB* viejaRaiz = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete viejaRaiz;
    }
}
void ArbolB::eliminar(NodoB* nodo, string genero) {
    int idx = nodo->encontrarIndice(genero);
    if (idx < nodo->claves.size() && nodo->claves[idx]->getGenero() == genero) {
        if (nodo->esHoja) {
            nodo->claves.erase(nodo->claves.begin() + idx);
        } else {
            NodoB* pred = nodo->hijos[idx];
            if (pred->claves.size() >= t) {
                while (!pred->esHoja) {
                    pred = pred->hijos.back();
                }
                Libro* libroPred = pred->claves.back();
                nodo->claves[idx] = libroPred;
                eliminar(pred, libroPred->getGenero());
            } else {
                NodoB* succ = nodo->hijos[idx + 1];
                if (succ->claves.size() >= t) {
                    while (!succ->esHoja) {
                        succ = succ->hijos.front();
                    }
                    Libro* libroSucc = succ->claves.front();
                    nodo->claves[idx] = libroSucc;
                    eliminar(succ, libroSucc->getGenero());
                } else {
                    pred->claves.push_back(nodo->claves[idx]);
                    for (int i = 0; i < succ->claves.size(); i++) {
                        pred->claves.push_back(succ->claves[i]);
                    }
                    if (!pred->esHoja) {
                        for (int i = 0; i < succ->hijos.size(); i++) {
                            pred->hijos.push_back(succ->hijos[i]);
                        }
                    }
                    nodo->claves.erase(nodo->claves.begin() + idx);
                    nodo->hijos.erase(nodo->hijos.begin() + idx + 1);
                    delete succ;
                    eliminar(pred, genero);
                }
            }
        }
    } else {
        if (nodo->esHoja) {
            cout << "El género " << genero << " no se encontró en el árbol." << endl;
            return;
        }
        bool debePrestar = (idx == nodo->claves.size()) ? true : false;
        if (nodo->hijos[idx]->claves.size() < t) {
            NodoB* hijoIzq = (idx != 0) ? nodo->hijos[idx - 1] : nullptr;
            NodoB* hijoDer = (idx != nodo->claves.size()) ? nodo->hijos[idx + 1] : nullptr;
            if (hijoIzq != nullptr && hijoIzq->claves.size() >= t) {
                nodo->hijos[idx]->claves.insert(nodo->hijos[idx]->claves.begin(), nodo->claves[idx - 1]);
                nodo->claves[idx - 1] = hijoIzq->claves.back();
                hijoIzq->claves.pop_back();
                if (!hijoIzq->esHoja) {
                    nodo->hijos[idx]->hijos.insert(nodo->hijos[idx]->hijos.begin(), hijoIzq->hijos.back());
                    hijoIzq->hijos.pop_back();
                }
            } else if (hijoDer != nullptr && hijoDer->claves.size() >= t) {
                nodo->hijos[idx]->claves.push_back(nodo->claves[idx]);
                nodo->claves[idx] = hijoDer->claves.front();
                hijoDer->claves.erase(hijoDer->claves.begin());
                if (!hijoDer->esHoja) { 
                    nodo->hijos[idx]->hijos.push_back(hijoDer->hijos.front());
                    hijoDer->hijos.erase(hijoDer->hijos.begin());
                }
            } else {
                if (hijoIzq != nullptr) {
                    hijoIzq->claves.push_back(nodo->claves[idx - 1]);
                    for (int i = 0; i < nodo->hijos[idx]->claves.size(); i++) {
                        hijoIzq->claves.push_back(nodo->hijos[idx]->claves[i]);
                    }
                    if (!hijoIzq->esHoja) {
                        for (int i = 0; i < nodo->hijos[idx]->hijos.size(); i++) {
                            hijoIzq->hijos.push_back(nodo->hijos[idx]->hijos[i]);
                        }
                    }
                    nodo->claves.erase(nodo->claves.begin() + idx - 1);
                    nodo->hijos.erase(nodo->hijos.begin() + idx);
                    delete nodo->hijos[idx];
                    idx--;
                } else if (hijoDer != nullptr) {
                    nodo->hijos[idx]->claves.push_back(nodo->claves[idx]);
                    for (int i = 0; i < hijoDer->claves.size(); i++) {
                        nodo->hijos[idx]->claves.push_back(hijoDer->claves[i]);
                    }
                    if (!nodo->hijos[idx]->esHoja) {
                        for (int i = 0; i < hijoDer->hijos.size(); i++) {
                            nodo->hijos[idx]->hijos.push_back(hijoDer->hijos[i]);
                        }
                    }
                    nodo->claves.erase(nodo->claves.begin() + idx);
                    nodo->hijos.erase(nodo->hijos.begin() + idx + 1);
                    delete hijoDer;
                }
            }
        }
        if (debePrestar && idx > nodo->claves.size()) {
            idx--;
        }
        eliminar(nodo->hijos[idx], genero);
    }
}

void ArbolB::mostrar(NodoB* nodo, int nivel) {
    if (nodo != nullptr) {
        cout << "Nivel " << nivel << ": ";
        for (const auto& libro : nodo->claves) {
            cout << "[" << libro->getGenero() << "] ";
        }
        cout << endl;
        if (!nodo->esHoja) {
            for (const auto& hijo : nodo->hijos) {
                mostrar(hijo, nivel + 1);
            }
        }
    }
}
void ArbolB::mostrarlo() {
    mostrar(raiz, 0);
}

void ArbolB::generarGraphvizRecursivo(NodoB* nodo, ofstream& archivo, int& contador) {
    if (nodo == nullptr) return;

    int idNodo = contador++;
    archivo << "  node" << idNodo << " [label=\"";

    for (size_t i = 0; i < nodo->claves.size(); i++) {
        archivo << nodo->claves[i]->getTitulo() << "\\n" 
                << nodo->claves[i]->getGenero();

        if (i < nodo->claves.size() - 1) {
            archivo << " | ";
        }
    }

    archivo << "\"];\n";

    for (size_t i = 0; i < nodo->hijos.size(); i++) {
        int idHijo = contador;
        generarGraphvizRecursivo(nodo->hijos[i], archivo, contador);
        archivo << "  node" << idNodo << " -> node" << idHijo << ";\n";
    }
}


void ArbolB::generarGraphviz(const string& nombreArchivo, const string& nombreImagen) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para escribir." << endl;
        return;
    }

    archivo << "digraph G {\n";
    archivo << "  node [shape=record];\n";

    int contador = 0;
    generarGraphvizRecursivo(raiz, archivo, contador);

    archivo << "}\n";
    archivo.close();

    string comando = "dot -Tpng " + nombreArchivo + " -o " + nombreImagen;
    system(comando.c_str());
}