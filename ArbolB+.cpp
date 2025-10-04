#include "ArbolB+.h"
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

ArbolBPlus::ArbolBPlus(int orden_) : raiz(nullptr), orden(orden_) {
    if (orden < 3) orden = 3;
    maxKeys = orden - 1;
    minKeys = (orden + 1) / 2 - 1;
}


ArbolBPlus::Nodo* ArbolBPlus::encontrarHoja(Nodo* nodo, int key) const {
    if (!nodo) return nullptr;
    if (nodo->esHoja) return nodo;
    int i = 0;
    while (i < (int)nodo->keys.size() && key >= nodo->keys[i]) ++i;
    return encontrarHoja(nodo->children[i], key);
}

void ArbolBPlus::insertarEnHoja(Nodo* hoja, Libro* libro) {
    int key = libro->getAnio();
    auto it = lower_bound(hoja->keys.begin(), hoja->keys.end(), key);
    int idx = int(it - hoja->keys.begin());
    if (it != hoja->keys.end() && *it == key) {
        // mismo año -> añadir al vector correspondiente
        hoja->records[idx].push_back(libro);
    } else {
        hoja->keys.insert(it, key);
        hoja->records.insert(hoja->records.begin() + idx, std::vector<Libro*>{libro});
    }
}

pair<ArbolBPlus::Nodo*, int> ArbolBPlus::dividirHoja(Nodo* hoja) {
    Nodo* nuevo = new Nodo(true);
    int total = int(hoja->keys.size());
    int split = (total + 1) / 2;
    nuevo->keys.assign(hoja->keys.begin() + split, hoja->keys.end());
    nuevo->records.assign(hoja->records.begin() + split, hoja->records.end());
    hoja->keys.erase(hoja->keys.begin() + split, hoja->keys.end());
    hoja->records.erase(hoja->records.begin() + split, hoja->records.end());

    nuevo->next = hoja->next;
    hoja->next = nuevo;

    int promoted = nuevo->keys.front();
    return {nuevo, promoted};
}

pair<ArbolBPlus::Nodo*, int> ArbolBPlus::dividirInterno(Nodo* node) {
    Nodo* nuevo = new Nodo(false);
    int total = int(node->keys.size());
    int mid = total / 2;
    int promoted = node->keys[mid];

    // keys right of mid move to new node
    nuevo->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
    node->keys.erase(node->keys.begin() + mid, node->keys.end());

    // corresponding children
    nuevo->children.assign(node->children.begin() + mid + 1, node->children.end());
    node->children.erase(node->children.begin() + mid + 1, node->children.end());

    return {nuevo, promoted};
}

void ArbolBPlus::insertarRecursivo(Nodo* node, int key, Libro* libro, int &promoKey, Nodo* &promoNode, bool &splitHappened) {
    if (node->esHoja) {
        insertarEnHoja(node, libro);
        if ((int)node->keys.size() > maxKeys) {
            auto pr = dividirHoja(node);
            promoNode = pr.first;
            promoKey = pr.second;
            splitHappened = true;
        } else {
            splitHappened = false;
        }
        return;
    }

    // interno
    int i = 0;
    while (i < (int)node->keys.size() && key >= node->keys[i]) ++i;
    int childPromoKey = 0;
    Nodo* childPromoNode = nullptr;
    bool childSplit = false;
    insertarRecursivo(node->children[i], key, libro, childPromoKey, childPromoNode, childSplit);
    if (!childSplit) {
        splitHappened = false;
        return;
    }

    auto it = lower_bound(node->keys.begin(), node->keys.end(), childPromoKey);
    int pos = int(it - node->keys.begin());
    node->keys.insert(node->keys.begin() + pos, childPromoKey);
    node->children.insert(node->children.begin() + pos + 1, childPromoNode);

    if ((int)node->keys.size() > maxKeys) {
        auto pr = dividirInterno(node);
        promoNode = pr.first;
        promoKey = pr.second;
        splitHappened = true;
    } else {
        splitHappened = false;
    }
}

void ArbolBPlus::insertar(Libro* libro) {
    if (!libro) return;
    int key = libro->getAnio();
    if (!raiz) {
        raiz = new Nodo(true);
        raiz->keys.push_back(key);
        raiz->records.push_back(std::vector<Libro*>{libro});
        return;
    }
    int promoKey = 0;
    Nodo* promoNode = nullptr;
    bool split = false;
    insertarRecursivo(raiz, key, libro, promoKey, promoNode, split);
    if (split && promoNode) {
        Nodo* nuevaRaiz = new Nodo(false);
        nuevaRaiz->keys.push_back(promoKey);
        nuevaRaiz->children.push_back(raiz);
        nuevaRaiz->children.push_back(promoNode);
        raiz = nuevaRaiz;
    }
}

void ArbolBPlus::recolectarRangoDesdeHoja(Nodo* hoja, int anioInicial, int anioFinal, ListaLibro &resultados) const {
    Nodo* cur = hoja;
    while (cur) {
        for (size_t i = 0; i < cur->keys.size(); ++i) {
            int a = cur->keys[i];
            if (a < anioInicial) continue;
            if (a > anioFinal) return;
            for (Libro* l : cur->records[i]) {
                resultados.agregarLibro(l);
            }
        }
        cur = cur->next;
    }
}

void ArbolBPlus::buscar(int anioInicial, int anioFinal, ListaLibro &resultados) const {
    if (!raiz) return;
    Nodo* hoja = encontrarHoja(raiz, anioInicial);
    if (!hoja) return;
    // retroceder a la izquierda del subtree por si faltan registros menores
    Nodo* cur = hoja;
    while (!cur->esHoja) cur = cur->children.front();
    // avanzar hasta hallar >= anioInicial
    while (cur) {
        bool any = false;
        for (int k : cur->keys) if (k >= anioInicial) { any = true; break; }
        if (any) break;
        cur = cur->next;
    }
    if (!cur) return;
    recolectarRangoDesdeHoja(cur, anioInicial, anioFinal, resultados);
}

bool ArbolBPlus::eliminarPorTitulo(const string &titulo) {
    if (!raiz) return false;
    Nodo* cur = raiz;
    while (!cur->esHoja) cur = cur->children.front();
    while (cur) {
        for (size_t i = 0; i < cur->records.size(); ++i) {
            auto &vec = cur->records[i];
            for (auto it = vec.begin(); it != vec.end(); ++it) {
                if ((*it)->getTitulo() == titulo) {
                    vec.erase(it);
                    if (vec.empty()) {
                        cur->records.erase(cur->records.begin() + int(i));
                        cur->keys.erase(cur->keys.begin() + int(i));
                    }
                    return true;
                }
            }
        }
        cur = cur->next;
    }
    return false;
}

void ArbolBPlus::imprimirArbol() const {
    if (!raiz) {
        cout << "Arbol B+ vacío\n";
        return;
    }
    queue<Nodo*> q;
    q.push(raiz);
    int nivel = 0;
    while (!q.empty()) {
        int sz = int(q.size());
        cout << "Nivel " << nivel << ": ";
        for (int i = 0; i < sz; ++i) {
            Nodo* n = q.front(); q.pop();
            cout << "[";
            for (size_t k = 0; k < n->keys.size(); ++k) {
                cout << n->keys[k];
                if (k + 1 < n->keys.size()) cout << ",";
            }
            cout << "] ";
            if (!n->esHoja) {
                for (Nodo* ch : n->children) q.push(ch);
            }
        }
        cout << "\n";
        ++nivel;
    }
}

void ArbolBPlus::imprimirHojas() const {
    if (!raiz) {
        cout << "Arbol B+ vacío\n";
        return;
    }
    Nodo* cur = raiz;
    while (!cur->esHoja) cur = cur->children.front();
    cout << "Hojas: ";
    while (cur) {
        cout << "[";
        for (size_t i = 0; i < cur->keys.size(); ++i) {
            cout << cur->keys[i];
            if (i + 1 < cur->keys.size()) cout << ",";
        }
        cout << "] -> ";
        cur = cur->next;
    }
    cout << "NULL\n";
}

void ArbolBPlus::generarGraphviz(const string &nombreArchivoDot, const string &nombreArchivoImagen) {
    if (!raiz) {
        cerr << "Árbol vacío: no se genera DOT.\n";
        return;
    }

    ofstream archivo(nombreArchivoDot);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo DOT para escribir.\n";
        return;
    }

    archivo << "digraph G {\n";
    archivo << "  node [shape=record];\n";

    generarGraphvizRec(raiz, archivo);

    archivo << "}\n";
    archivo.close();

    string comando = "dot -Tsvg " + nombreArchivoDot + " -o " + nombreArchivoImagen;
    int resultado = system(comando.c_str());
    if (resultado != 0) {
        cerr << "Error al ejecutar Graphviz. Asegúrate de que 'dot' esté instalado y en el PATH.\n";
    }
}

void ArbolBPlus::generarGraphvizRec(Nodo* nodo, ofstream &archivo) {
    if (!nodo) return;

    archivo << "  node" << nodo << " [label=\"";

    for (size_t i = 0; i < nodo->keys.size(); ++i) {
        archivo << "<f" << i << "> | " << nodo->keys[i] << " | ";
    }
    archivo << "<f" << nodo->keys.size() << ">";
    archivo << "\"];\n";
    if (!nodo->esHoja) {
        for (size_t i = 0; i < nodo->children.size(); ++i) {
            Nodo* hijo = nodo->children[i];
            generarGraphvizRec(hijo, archivo);
            archivo << "  node" << nodo << ":f" << i << " -> node" << hijo << ";\n";
        }
    }
}
