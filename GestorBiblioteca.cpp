#include "GestorBiblioteca.h"
#include <cstddef>
#include <cstdio>
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <fstream>
#include <string>
#include "Libro.h"

using namespace std;

void GestorBiblioteca::bienvenidaInicial(){
    cout<<"--------------------------"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"|    BIENVENIDO A LA     |"<<endl;
    cout<<"|   BIBLIOTECA MAGICA    |"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"--------------------------"<<endl;
    menuJuego();
}

void GestorBiblioteca::menuJuego(){
    int opcion = 0;

    do{
        cout<<"____________________________________________"<<endl;
        cout<<"|  ¿QUE DESEA REALIZAR EN LA BIBLIOTECA?   |"<<endl;
        cout<<"|      1.        Agregar libro             |"<<endl;
        cout<<"|      2.     Cargar archivo csv           |"<<endl;
        cout<<"|      3.      Realizar busqueda           |"<<endl;
        cout<<"|      4.       Eliminar libro             |"<<endl;
        cout<<"|      5.        Listar libros             |"<<endl;
        cout<<"|      6. Comparar tiempo de busquedas     |"<<endl;
        cout<<"|      7. Visulizar arboles utilizados     |"<<endl;
        cout<<"|      8.            Salir                 |"<<endl;
        cout<<"|__________________________________________|"<<endl;
        try{
            cout<<"Ingresar numero de opcion que desea realizar: ";
            cin>>opcion;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                throw invalid_argument("Entrada realizada no valida, intente de nuevo.");
            }
            if (opcion < 1 || opcion >8) {throw out_of_range("Entrada fuera de rango, intente de nuevo...");}
        }catch(const invalid_argument& e){cout<<"ERROR: "<<e.what()<<endl;}
        catch(const out_of_range& e){cout<<"ERROR: "<<e.what()<<endl;}
        
    }while(cin.fail()||opcion < 1 || opcion >8);
    realizarAccion(opcion);
}

void GestorBiblioteca::menuFinAccion(){
    int opcion = 0;

    do{
        cout<<"____________________________________________"<<endl;
        cout<<"|         ¿QUE DESEA REALIZAR AHORA?       |"<<endl;
        cout<<"|      1.   Volver a menu principal        |"<<endl;
        cout<<"|      2.           Salir                  |"<<endl;
        cout<<"|__________________________________________|"<<endl;
        try{
            cout<<"Ingresar numero de opcion que desea realizar: ";
            cin>>opcion;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                throw invalid_argument("Entrada realizada no valida, intente de nuevo.");
            }
            if (opcion < 1 || opcion >2) {throw out_of_range("Entrada fuera de rango, intente de nuevo...");}
        }catch(const invalid_argument& e){cout<<"ERROR: "<<e.what()<<endl;}
        catch(const out_of_range& e){cout<<"ERROR: "<<e.what()<<endl;}
        
    }while(cin.fail()||opcion < 1 || opcion >2);
    if(opcion == 1){
        menuJuego();
    }else{

    }
}

void GestorBiblioteca::realizarAccion(int& opcion){
    switch (opcion) {
        case 1: 
            cout<<"Agregando libro"<<endl;
            menuFinAccion();
            break;
        case 2:
            leerArchivo();
            listaLibros.imprimirLista();
            menuFinAccion();
            break;
        case 3:
            cout<<"Realizando busqueda"<<endl;
            menuFinAccion();
            break;
        case 4:
            cout<<"Eliminando libro"<<endl;
            menuFinAccion();
            break;
        case 5:
            cout<<"Listando libros"<<endl;
            menuFinAccion();
            break;
        case 6:
            cout<<"Comparando busquedas"<<endl;
            menuFinAccion();
            break;
        case 7:
            cout<<"Viendo arboles"<<endl;
            menuFinAccion();
            break;
        case 8:
            break;
    }
}

Libro* GestorBiblioteca::crearLibro(string linea) {
    size_t posicion = 0;
    string atributo;
    string campos[5];
    int indice = 0;

    auto quitarComillas = [](string &s) {
        if (!s.empty() && s.front() == '"' && s.back() == '"') {
            s = s.substr(1, s.size() - 2);
        }
    };

    while ((posicion = linea.find(',')) != string::npos && indice < 5) {
        atributo = linea.substr(0, posicion);
        linea.erase(0, posicion + 1);

        if (atributo.empty() || atributo.front() != '"' || atributo.back() != '"') {
            return nullptr; // formato inválido
        }

        quitarComillas(atributo);
        campos[indice++] = atributo;
    }

    if (linea.empty() || linea.front() != '"' || linea.back() != '"') {
        return nullptr;
    }
    quitarComillas(linea);
    if (indice < 5) {
        campos[indice++] = linea;
    }

    if (indice != 5) {
        return nullptr;
    }

    string titulo, isbn, genero, autor;
    int anio = 0;

    try {
        titulo = campos[0];
        isbn   = campos[1];
        genero = campos[2];
        anio   = stoi(campos[3]); 
        autor  = campos[4];
    } catch (...) {
        return nullptr; 
    }

    if (validarISBN(isbn)) {
        Libro* libro = new Libro();
        libro->setTitulo(titulo);
        libro->setISBN(isbn);
        libro->setGenero(genero);
        libro->setAnio(anio);
        libro->setAutor(autor);
        return libro;
    }

    return nullptr;
}


bool GestorBiblioteca::validarISBN(string isbn){
    return listaLibros.buscarPorIsbn(isbn) == nullptr;
}


void GestorBiblioteca::leerArchivo(){
    cout<<"---Hora de cargar el archivo---"<<endl;
    bool valido = false;
    string nombre;
    while (!valido) {
        cout<<"Ingresa el nombre del archivo (debe finalizar con 'csv.'): "<<endl;
        cin>> nombre;

        if (nombre.size()<4||nombre.substr(nombre.size()-4)!=".csv") {
            cout<<"ERROR: el archivo debe tener extension '.csv' "<<endl;
            continue;
        }
        ifstream archivo(nombre);
        if(archivo.fail()){
            cout<<"ERROR: archivo no valido o no encontrado"<<endl;

        }else{
            valido = true;
            cout<<"EXITO: Se cargo correctamente el archivo"<<endl;
            string linea;
            while (getline(archivo, linea)) {
                cargarLibros(crearLibro(linea));
            }
            archivo.close();
        }

    }
}

void GestorBiblioteca::cargarLibros(Libro* libro){
    listaLibros.agregarLibro(libro);
}