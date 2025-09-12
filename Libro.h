#ifndef LIBRO_H
#define LIBRO_H
#include <iostream>
#include <string>
using namespace std;

class Libro{

    private:
        string titulo;
        string isbn;
        string genero;
        int anio;
        string autor;
    public:
        void setTitulo(string titulo){
            this->titulo = titulo;
        }
        string getTitulo(){
            return titulo;
        }
        void setISBN(string isbn){
            this->isbn = isbn;
        }
        string getISBN(){
            return isbn;
        }
        void setGenero(string genero){
            this->genero = genero;
        }
        string getGenero(){
            return genero;
        }
        void setAnio(int anio){
            this->anio = anio;
        }
        int getAnio(){
            return anio;
        }
        void setAutor(string autor){
            this->autor = autor;
        }
        string getAutor(){
            return autor;
        }
        

};





#endif