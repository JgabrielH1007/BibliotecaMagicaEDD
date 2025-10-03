#include "ArbolB.h"
#include <cstddef>
#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
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

void ArbolB::eliminar(const string genero, Libro* libroEspecifico) {
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

static std::string escapeLabel(const std::string &s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == '\"') out.push_back('\''); 
        else if (c == '\n' || c == '\r') out.push_back(' ');
        else out.push_back(c);
    }
    return out;
}

void ArbolB::generarGraphviz(const std::string &nombreArchivoDot, const std::string &nombreArchivoImagen) {
    if (!raiz) {
        std::cerr << "Árbol vacío: no se genera DOT.\n";
        return;
    }

    std::vector<nodoB*> nodos;
    nodos.reserve(1024);
    std::queue<nodoB*> q;
    std::unordered_map<nodoB*, bool> seen;
    q.push(raiz);
    seen[raiz] = true;

    while (!q.empty()) {
        nodoB* cur = q.front(); q.pop();
        if (!cur) continue;
        nodos.push_back(cur);
        for (nodoB* h : cur->hijos) {
            if (h && !seen[h]) {
                seen[h] = true;
                q.push(h);
            }
        }
    }

    size_t nodoCount = nodos.size();
    std::cerr << "Total nodos detectados: " << nodoCount << "\n";

    std::unordered_map<nodoB*, std::string> idMap;
    idMap.reserve(nodoCount * 2);
    for (size_t i = 0; i < nodos.size(); ++i) {
        std::ostringstream oss;
        oss << "n" << (i + 1);
        idMap[nodos[i]] = oss.str();
    }

    std::ofstream archivo(nombreArchivoDot);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo DOT: " << nombreArchivoDot << "\n";
        return;
    }

    archivo << "digraph G {\n";
    archivo << "    node [shape=record,fontsize=10];\n";
    archivo << "    rankdir=TB;\n";

    for (nodoB* n : nodos) {
        std::ostringstream label;
        label << "{";
        for (size_t i = 0; i < n->llaves.size(); ++i) {
            std::string genero = n->llaves[i] ? n->llaves[i]->getGenero() : std::string("");
            label << escapeLabel(genero);
            if (i + 1 < n->llaves.size()) label << " | ";
        }
        label << "}";
        archivo << "    " << idMap[n] << " [label=\"" << label.str() << "\"];\n";
    }

    for (nodoB* n : nodos) {
        for (nodoB* hijo : n->hijos) {
            if (hijo) {
                archivo << "    " << idMap[n] << " -> " << idMap[hijo] << ";\n";
            }
        }
    }

    archivo << "}\n";
    archivo.close();

    std::string engine = (nodoCount > 1000) ? "sfdp" : "dot";
    std::string cmd = engine + " -Tsvg \"" + nombreArchivoDot + "\" -o \"" + nombreArchivoImagen + "\"";

    int res = std::system(cmd.c_str());
    if (res != 0) {
        std::cerr << "Graphviz retornó código " << res << ". Asegúrate que '" << engine << "' esté instalado y en PATH.\n";
    }
}