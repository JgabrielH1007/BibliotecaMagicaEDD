#ifndef LIBRO_H
#define LIBRO_H
#include <iostream>
#include <string>
using namespace std;

class Libro{

    private:
        string titulo="";
        string isbn="";
        long long int isbnNum=0;
        string genero="";
        int anio = 0;
        string autor ="";
        int numCopias = 1;
    public:

        void setNumCopias(int numCopias){
            this->numCopias = numCopias;
        }
        int getNumCopias(){
            return numCopias;
        }
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

        long long int getISBNNum(){
            return isbnNum;
        }
        void setISBNNum(long long int isbnNum){
            this->isbnNum = isbnNum;
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

        void imprimirLibro(){
            cout << "Título: " << titulo<<"\n"
             << " | ISBN: " << isbn<<"\n"
             << " | Género: " << genero<<"\n"
             << " | Año: " << anio<<"\n"
             << " | Autor: " << autor<<"\n"
             << endl;
        }
        

};





#endif