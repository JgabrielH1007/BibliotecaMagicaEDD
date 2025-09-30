#include "ArbolB+.h"
#include "NodoB+.h"
#include "Libro.h"
#include "ListaLibro.h"
#include <cstddef>
#include <fstream>
#include <climits>
#include <vector>
#include <iostream>
#include <unordered_map>
using namespace std;

ArbolBPlus::ArbolBPlus(int t) {
    if (t < 2) t = 2;
    this->t = t;
    raiz = new NodoBPlus(true);
}

NodoBPlus* ArbolBPlus::buscarHoja(int anio) const {
    NodoBPlus* actual = raiz;
    while (actual && !actual->esHoja) {
        int i = 0;
        while (i < (int)actual->claves.size() && anio >= actual->claves[i]) i++;
        actual = actual->hijos[i];
    }
    return actual;
}

void ArbolBPlus::insertar(Libro* libro) {
    if (!libro) return;
    int anio = libro->getAnio();
    if (raiz->esHoja) {
        insertarEnHoja(raiz, libro);
        if ((int)raiz->claves.size() > 2*t - 1) {
            dividirHoja(raiz);
        }
    } else {
        NodoBPlus* hoja = buscarHoja(anio);
        insertarEnHoja(hoja, libro);
        if ((int)hoja->claves.size() > 2*t - 1) {
            dividirHoja(hoja);
        }
    }

    if (!raiz->esHoja && raiz->claves.empty() && !raiz->hijos.empty()) {
        raiz = raiz->hijos[0];
    }
}

void ArbolBPlus::insertarEnHoja(NodoBPlus* hoja, Libro* libro) {
    int anio = libro->getAnio();
    int i = 0;
    while (i < (int)hoja->claves.size() && hoja->claves[i] < anio) i++;

    if (i < (int)hoja->claves.size() && hoja->claves[i] == anio) {
        hoja->registros[i].push_back(libro);
    } else {
        hoja->claves.insert(hoja->claves.begin() + i, anio);
        hoja->registros.insert(hoja->registros.begin() + i, vector<Libro*>{libro});
    }
}

void ArbolBPlus::dividirHoja(NodoBPlus* hoja) {
    NodoBPlus* nueva = new NodoBPlus(true);
    int total = (int)hoja->claves.size();
    int mitad = total / 2;

    for (int i = mitad; i < total; ++i) {
        nueva->claves.push_back(std::move(hoja->claves[i]));
        nueva->registros.push_back(std::move(hoja->registros[i]));
    }
    hoja->claves.resize(mitad);
    hoja->registros.resize(mitad);

    nueva->siguiente = hoja->siguiente;
    hoja->siguiente = nueva;

    if (hoja == raiz) {
        NodoBPlus* nuevaRaiz = new NodoBPlus(false);
        nuevaRaiz->claves.push_back(nueva->claves.front());
        nuevaRaiz->hijos.push_back(hoja);
        nuevaRaiz->hijos.push_back(nueva);
        raiz = nuevaRaiz;
        return;
    }

    NodoBPlus* padre = nullptr;
    int pos = -1;
    encontrarPadreYPos(raiz, hoja, padre, pos);
    if (!padre) {
        // fallback: crear nueva raíz (situación excepcional)
        NodoBPlus* nuevaRaiz = new NodoBPlus(false);
        nuevaRaiz->claves.push_back(nueva->claves.front());
        nuevaRaiz->hijos.push_back(hoja);
        nuevaRaiz->hijos.push_back(nueva);
        raiz = nuevaRaiz;
        return;
    }

    int insertPos = 0;
    while (insertPos < (int)padre->claves.size() && padre->claves[insertPos] < nueva->claves.front()) insertPos++;

    padre->claves.insert(padre->claves.begin() + insertPos, nueva->claves.front());
    padre->hijos.insert(padre->hijos.begin() + insertPos + 1, nueva);

    if ((int)padre->claves.size() > 2*t - 1) {
        dividirHijo(padre, insertPos); 
    }
}

void ArbolBPlus::dividirHijo(NodoBPlus* padre, int indice) {
    if (!padre) {
        return;
    }

    if (indice < 0 || indice >= (int)padre->hijos.size()) return;
    NodoBPlus* hijo = padre->hijos[indice];
    if (!hijo) return;

    NodoBPlus* nuevo = new NodoBPlus(hijo->esHoja);
    int total = (int)hijo->claves.size();
    int mitad = total / 2;

    if (hijo->esHoja) {
        for (int i = mitad; i < total; ++i) {
            nuevo->claves.push_back(std::move(hijo->claves[i]));
            nuevo->registros.push_back(std::move(hijo->registros[i]));
        }
        hijo->claves.resize(mitad);
        hijo->registros.resize(mitad);

        nuevo->siguiente = hijo->siguiente;
        hijo->siguiente = nuevo;

        int clavePromotora = nuevo->claves.front();
        padre->hijos.insert(padre->hijos.begin() + indice + 1, nuevo);
        padre->claves.insert(padre->claves.begin() + indice, clavePromotora);
    } else {
        int mid = mitad;
        int claveProm = hijo->claves[mid];
        for (int i = mid + 1; i < total; ++i) {
            nuevo->claves.push_back(std::move(hijo->claves[i]));
        }
        for (int i = mid + 1; i < (int)hijo->hijos.size(); ++i) {
            nuevo->hijos.push_back(hijo->hijos[i]);
        }

        hijo->claves.resize(mid);
        hijo->hijos.resize(mid + 1);

        padre->claves.insert(padre->claves.begin() + indice, claveProm);
        padre->hijos.insert(padre->hijos.begin() + indice + 1, nuevo);
    }

    if ((int)padre->claves.size() > 2*t - 1) {
        if (padre == raiz) {
            int totalP = (int)padre->claves.size();
            int midP = totalP / 2;
            NodoBPlus* derecho = new NodoBPlus(false);
            for (int k = midP + 1; k < totalP; ++k) derecho->claves.push_back(std::move(padre->claves[k]));
            for (int k = midP + 1; k < (int)padre->hijos.size(); ++k) derecho->hijos.push_back(padre->hijos[k]);

            int claveProm = padre->claves[midP];

            padre->claves.resize(midP);
            padre->hijos.resize(midP + 1);

            NodoBPlus* nuevaRaiz = new NodoBPlus(false);
            nuevaRaiz->claves.push_back(claveProm);
            nuevaRaiz->hijos.push_back(padre);
            nuevaRaiz->hijos.push_back(derecho);
            raiz = nuevaRaiz;
        } else {
            NodoBPlus* pp = nullptr;
            int idx = -1;
            encontrarPadreYPos(raiz, padre, pp, idx);
            if (pp) dividirHijo(pp, idx);
            else {
                int totalP = (int)padre->claves.size();
                int midP = totalP / 2;
                NodoBPlus* derecho = new NodoBPlus(false);
                for (int k = midP + 1; k < totalP; ++k) derecho->claves.push_back(std::move(padre->claves[k]));
                for (int k = midP + 1; k < (int)padre->hijos.size(); ++k) derecho->hijos.push_back(padre->hijos[k]);

                int claveProm = padre->claves[midP];

                padre->claves.resize(midP);
                padre->hijos.resize(midP + 1);

                NodoBPlus* nuevaRaiz = new NodoBPlus(false);
                nuevaRaiz->claves.push_back(claveProm);
                nuevaRaiz->hijos.push_back(padre);
                nuevaRaiz->hijos.push_back(derecho);
                raiz = nuevaRaiz;
            }
        }
    }
}

void ArbolBPlus::encontrarPadreYPos(NodoBPlus* raiz, NodoBPlus* hijo, NodoBPlus*& pp, int& idx) {
    pp = nullptr;
    idx = -1;

    if (raiz == nullptr || raiz == hijo) {
        return; 
    }

    for (size_t i = 0; i < raiz->claves.size() + 1; i++) {
        if (raiz->hijos[i] == hijo) {
            pp = raiz;  
            idx = i;     
            return;
        }
    }

    for (size_t i = 0; i < raiz->claves.size() + 1; i++) {
        if (raiz->hijos[i] != nullptr) {
            encontrarPadreYPos(raiz->hijos[i], hijo, pp, idx);
            if (pp != nullptr) {
                return; 
            }
        }
    }
}


void ArbolBPlus::mostrar() {
    if (!raiz) {
        cout << "Arbol vacío\n";
        return;
    }
    vector<NodoBPlus*> nivel;
    nivel.push_back(raiz);
    int depth = 0;
    while (!nivel.empty()) {
        cout << "Nivel " << depth << ": ";
        vector<NodoBPlus*> siguiente;
        for (auto n : nivel) {
            cout << "[";
            for (int k = 0; k < (int)n->claves.size(); ++k) {
                cout << n->claves[k];
                if (k+1 < (int)n->claves.size()) cout << ",";
            }
            cout << "] ";
            if (!n->esHoja) {
                for (auto h : n->hijos) if (h) siguiente.push_back(h);
            }
        }
        cout << "\n";
        nivel = std::move(siguiente);
        depth++;
    }
}


void ArbolBPlus::generarGraphvizRecursivo(NodoBPlus* nodo, ofstream& archivo, int& contador, unordered_map<NodoBPlus*, int>& idMap) {
    if (!nodo) return;
    int id = contador++;
    idMap[nodo] = id;

    if (nodo->esHoja) {
        archivo << "  node" << id << " [label=\"";
        for (size_t i = 0; i < nodo->claves.size(); ++i) {
            archivo << nodo->claves[i] << " (" << nodo->registros[i].size() << ")";
            if (i + 1 < nodo->claves.size()) archivo << " | ";
        }
        archivo << "\", shape=record];\n";
    } else {
        archivo << "  node" << id << " [label=\"";
        for (size_t i = 0; i < nodo->claves.size(); ++i) {
            archivo << nodo->claves[i];
            if (i + 1 < nodo->claves.size()) archivo << " | ";
        }
        archivo << "\", shape=record];\n";
    }

    if (!nodo->esHoja) {
        for (size_t i = 0; i < nodo->hijos.size(); ++i) {
            if (nodo->hijos[i]) {
                generarGraphvizRecursivo(nodo->hijos[i], archivo, contador, idMap);
                int idH = idMap[nodo->hijos[i]];
                archivo << "  node" << id << " -> node" << idH << ";\n";
            }
        }
    } else {
        if (nodo->siguiente) {
            if (idMap.find(nodo->siguiente) == idMap.end()) {
                generarGraphvizRecursivo(nodo->siguiente, archivo, contador, idMap);
            }
            int idNext = idMap[nodo->siguiente];
            archivo << "  node" << id << " -> node" << idNext << " [style=dashed, constraint=false];\n";
        }
    }
}

void ArbolBPlus::generarGraphviz(const string& nombreArchivo, const string& nombreImagen) {
    ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo para escribir.\n";
        return;
    }
    archivo << "digraph G {\n  node [shape=record];\n";
    int contador = 0;
    unordered_map<NodoBPlus*, int> idMap;
    generarGraphvizRecursivo(raiz, archivo, contador, idMap);
    archivo << "}\n";
    archivo.close();
    string comando = "dot -Tpng " + nombreArchivo + " -o " + nombreImagen;
    system(comando.c_str());
}

void ArbolBPlus::clavesPromotorasRec(NodoBPlus* nodo) {
    if (!nodo || nodo->esHoja) return;
    for (auto h : nodo->hijos) if (h) clavesPromotorasRec(h);
    for (size_t i = 0; i < nodo->claves.size(); ++i) {
        if (i + 1 < nodo->hijos.size() && nodo->hijos[i + 1] && !nodo->hijos[i + 1]->claves.empty())
            nodo->claves[i] = nodo->hijos[i + 1]->claves.front();
    }
}


void ArbolBPlus::rebalancearInterno(NodoBPlus* nodo) {
    if (!nodo || nodo == raiz) return;
    if ((int)nodo->claves.size() >= (t - 1)) return;

    NodoBPlus* padre = nullptr;
    int pos = -1;
    encontrarPadreYPos(raiz, nodo, padre, pos);
    if (!padre) return; 
    if (pos - 1 >= 0) {
        NodoBPlus* izq = padre->hijos[pos - 1];
        if ((int)izq->claves.size() >= t) {
            int claveDesdePadre = padre->claves[pos - 1];
            int claveIzqLast = izq->claves.back();
            if (!izq->hijos.empty()) {
                NodoBPlus* moverHijo = izq->hijos.back();
                izq->hijos.pop_back();
                nodo->hijos.insert(nodo->hijos.begin(), moverHijo);
            }
            nodo->claves.insert(nodo->claves.begin(), claveDesdePadre);
            padre->claves[pos - 1] = claveIzqLast;
            izq->claves.pop_back();
            return;
        }
    }

    if (pos + 1 < (int)padre->hijos.size()) {
        NodoBPlus* der = padre->hijos[pos + 1];
        if ((int)der->claves.size() >= t) {
            int claveDesdePadre = padre->claves[pos];
            int claveDerFirst = der->claves.front();
            if (!der->hijos.empty()) {
                NodoBPlus* mover = der->hijos.front();
                der->hijos.erase(der->hijos.begin());
                nodo->hijos.push_back(mover);
            }
            nodo->claves.push_back(claveDesdePadre);
            padre->claves[pos] = claveDerFirst;
            der->claves.erase(der->claves.begin());
            return;
        }
    }

    if (pos - 1 >= 0) {
        NodoBPlus* izq = padre->hijos[pos - 1];
        izq->claves.push_back(padre->claves[pos - 1]);
        for (auto &k : nodo->claves) izq->claves.push_back(k);
        for (auto &h : nodo->hijos) izq->hijos.push_back(h);

        padre->hijos.erase(padre->hijos.begin() + pos);
        padre->claves.erase(padre->claves.begin() + (pos - 1));
        delete nodo;

        if (padre != raiz && (int)padre->claves.size() < (t - 1)) {
            rebalancearInterno(padre);
        } else if (padre == raiz && padre->claves.empty()) {
            NodoBPlus* unico = padre->hijos.empty() ? nullptr : padre->hijos.front();
            delete padre;
            raiz = unico;
        }
        return;
    } else if (pos + 1 < (int)padre->hijos.size()) {
        NodoBPlus* der = padre->hijos[pos + 1];
        nodo->claves.push_back(padre->claves[pos]);
        for (auto &k : der->claves) nodo->claves.push_back(k);
        for (auto &h : der->hijos) nodo->hijos.push_back(h);

        padre->hijos.erase(padre->hijos.begin() + (pos + 1));
        padre->claves.erase(padre->claves.begin() + pos);
        delete der;

        if (padre != raiz && (int)padre->claves.size() < (t - 1)) {
            rebalancearInterno(padre);
        } else if (padre == raiz && padre->claves.empty()) {
            NodoBPlus* unico = padre->hijos.empty() ? nullptr : padre->hijos.front();
            delete padre;
            raiz = unico;
        }
        return;
    }
}

void ArbolBPlus::rebalancearHoja(NodoBPlus* hoja) {
    if (!hoja || hoja == raiz) return;
    if ((int)hoja->claves.size() >= (t - 1)) return;

    NodoBPlus* padre = nullptr;
    int pos = -1;
    encontrarPadreYPos(raiz, hoja, padre, pos);
    if (!padre) return;

    if (pos - 1 >= 0) {
        NodoBPlus* izq = padre->hijos[pos - 1];
        if ((int)izq->claves.size() >= t) {
            int k = izq->claves.back();
            vector<Libro*> regs = izq->registros.back();
            izq->claves.pop_back();
            izq->registros.pop_back();

            hoja->claves.insert(hoja->claves.begin(), k);
            hoja->registros.insert(hoja->registros.begin(), regs);

            if (pos - 1 >= 0 && pos - 1 < (int)padre->claves.size())
                padre->claves[pos - 1] = hoja->claves.front();
            return;
        }
    }

    if (pos + 1 < (int)padre->hijos.size()) {
        NodoBPlus* der = padre->hijos[pos + 1];
        if ((int)der->claves.size() >= t) {
            int k = der->claves.front();
            vector<Libro*> regs = der->registros.front();
            der->claves.erase(der->claves.begin());
            der->registros.erase(der->registros.begin());

            hoja->claves.push_back(k);
            hoja->registros.push_back(regs);

            if (pos < (int)padre->claves.size()) {
                if (!der->claves.empty())
                    padre->claves[pos] = der->claves.front();
                else
                    padre->claves[pos] = hoja->claves.back();
            }
            return;
        }
    }

    if (pos - 1 >= 0) {
        NodoBPlus* izq = padre->hijos[pos - 1];
        for (size_t i = 0; i < hoja->claves.size(); ++i) {
            izq->claves.push_back(hoja->claves[i]);
            izq->registros.push_back(hoja->registros[i]);
        }
        izq->siguiente = hoja->siguiente;

        padre->hijos.erase(padre->hijos.begin() + pos);
        if (pos - 1 >= 0 && pos - 1 < (int)padre->claves.size())
            padre->claves.erase(padre->claves.begin() + (pos - 1));

        delete hoja;

        if (padre != raiz && (int)padre->claves.size() < (t - 1)) {
            rebalancearInterno(padre);
        } else if (padre == raiz && padre->claves.empty()) {
            NodoBPlus* unico = padre->hijos.empty() ? nullptr : padre->hijos.front();
            delete padre;
            raiz = unico;
        }
        return;
    } else if (pos + 1 < (int)padre->hijos.size()) {
        NodoBPlus* der = padre->hijos[pos + 1];
        for (size_t i = 0; i < der->claves.size(); ++i) {
            hoja->claves.push_back(der->claves[i]);
            hoja->registros.push_back(der->registros[i]);
        }
        hoja->siguiente = der->siguiente;

        padre->hijos.erase(padre->hijos.begin() + (pos + 1));
        if (pos < (int)padre->claves.size())
            padre->claves.erase(padre->claves.begin() + pos);

        delete der;

        if (padre != raiz && (int)padre->claves.size() < (t - 1)) {
            rebalancearInterno(padre);
        } else if (padre == raiz && padre->claves.empty()) {
            NodoBPlus* unico = padre->hijos.empty() ? nullptr : padre->hijos.front();
            delete padre;
            raiz = unico;
        }
        return;
    }
}

void ArbolBPlus::eliminarPorTitulo(const string& titulo) {
    if (!raiz) {
        cout << "Árbol vacío.\n";
        return;
    }

    NodoBPlus* hoja = raiz;
    while (!hoja->esHoja) {
        if (hoja->hijos.empty()) { cout << "Estructura corrupta.\n"; return; }
        hoja = hoja->hijos.front();
    }

    int eliminadosTotales = 0;
    std::vector<NodoBPlus*> hojasVisitadas; hojasVisitadas.reserve(128);

    for (NodoBPlus* actual = hoja; actual != nullptr; actual = actual->siguiente) {
        hojasVisitadas.push_back(actual);
        for (int idx = (int)actual->claves.size() - 1; idx >= 0; --idx) {
            vector<Libro*>& vec = actual->registros[idx];
            int antes = (int)vec.size();

            for (size_t j = 0; j < vec.size(); ) {
                if (vec[j] && vec[j]->getTitulo() == titulo) {

                    vec.erase(vec.begin() + j);
                } else {
                    j++;
                }
            }

            int despues = (int)vec.size();
            eliminadosTotales += (antes - despues);

            if (vec.empty()) {
                actual->claves.erase(actual->claves.begin() + idx);
                actual->registros.erase(actual->registros.begin() + idx);
            }
        }
    }

    if (eliminadosTotales == 0) {
        cout << "No se encontró ningún libro con el título \"" << titulo << "\".\n";
        return;
    }

    for (NodoBPlus* h : hojasVisitadas) {
        if (!h) continue;
        if (h == raiz && h->esHoja) continue;
        if ((int)h->claves.size() < (t - 1)) {
            rebalancearHoja(h);
        }
    }

    clavesPromotorasRec(raiz);

    if (raiz && !raiz->esHoja && raiz->claves.empty()) {
        NodoBPlus* nuevo = raiz->hijos.empty() ? nullptr : raiz->hijos.front();
        delete raiz;
        raiz = nuevo;
    }

    cout << "Eliminados " << eliminadosTotales << " libros con el título \"" << titulo << "\". Árbol re-balanceado.\n";
}

Libro* ArbolBPlus::buscarPorRangoAnio(int anioInicial, int anioFinal, ListaLibro& lista) const {
    if (anioInicial > anioFinal) return nullptr;
    NodoBPlus* hoja = buscarHoja(anioInicial);
    if (!hoja) return nullptr;

    Libro* primerEncontrado = nullptr;
    while (hoja) {
        for (size_t i = 0; i < hoja->claves.size(); ++i) {
            if (hoja->claves[i] >= anioInicial && hoja->claves[i] <= anioFinal) {
                for (Libro* libro : hoja->registros[i]) {
                    lista.agregarLibro(libro);
                    if (!primerEncontrado) {
                        primerEncontrado = libro;
                    }
                }
            } else if (hoja->claves[i] > anioFinal) {
                return primerEncontrado;
            }
        }
        hoja = hoja->siguiente;
    }
    return primerEncontrado;
}