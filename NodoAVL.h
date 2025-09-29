#ifndef NODOAVL_H
#define NODOAVL_H
#include "Libro.h"
#include <iostream>
using namespace std;
class NodoAVL {
        private:
            Libro* libro;
            NodoAVL* izquierdo;
            NodoAVL* derecho;
            int altura;
            int factorEquilibrio;
        public:

        Libro* getLibro() {
            return libro;
        }
        void setLibro(Libro* libro) {
            this->libro = libro;
        }

        NodoAVL* getIzquierdo() {
            return izquierdo;
        }

        void setIzquierdo(NodoAVL* izquierdo) {
            this->izquierdo = izquierdo;
        }

        NodoAVL* getDerecho() {
            return derecho;
        }

        void setDerecho(NodoAVL* derecho) {
            this->derecho = derecho;
        }

        int getAltura() {
            return altura;
        }

        void setAltura(int altura) {
            this->altura = altura;
        }

        int getFactorEquilibrio() {
            return factorEquilibrio;
        }

        void setFactorEquilibrio(int factorEquilibrio) {
            this->factorEquilibrio = factorEquilibrio;
        }
};

#endif