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
#include "ArbolAVL.h"
using namespace std;

void GestorBiblioteca::bienvenidaInicial(){
    cout<<"--------------------------"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"|    BIENVENIDO A LA     |"<<endl;
    cout<<"|   BIBLIOTECA MAGICA    |"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"--------------------------"<<endl;
    menuBiblioteca();
}

void GestorBiblioteca::menuBiblioteca(){
    int opcion = 0;
    cout<<" __________________________________________ "<<endl;
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
    opcion = validarOpcion(1,8);
    realizarAccion(opcion);
}

void GestorBiblioteca::menuFinAccion(){
    int opcion = 0;
    cout<<" __________________________________________ "<<endl;
    cout<<"|         ¿QUE DESEA REALIZAR AHORA?       |"<<endl;
    cout<<"|      1.   Volver a menu principal        |"<<endl;
    cout<<"|      2.           Salir                  |"<<endl;
    cout<<"|__________________________________________|"<<endl;
    opcion = validarOpcion(1, 2);
    if(opcion == 1){
        menuBiblioteca();
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
            menuFinAccion();
            break;
        case 3:
            cout<<"Realizando busqueda"<<endl;
            menuFinAccion();
            break;
        case 4:
            eliminarLibro();
            menuFinAccion();
            break;
        case 5:
            mostrarLibros();
            menuFinAccion();
            break;
        case 6:
            cout<<"Comparando busquedas"<<endl;
            menuFinAccion();
            break;
        case 7:
            generarGraphvizArboles();
            menuFinAccion();
            break;
        case 8:
            break;
    }
}

long long int GestorBiblioteca::convertirISBN(string isbn) {
    string isbnSinGuiones;
    for (char c : isbn) {
        if (c != '-') {
            isbnSinGuiones += c;
        }
    }
    return stoll(isbnSinGuiones); 
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
            return nullptr; 
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
        libro->setISBNNum(convertirISBN(isbn));
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
    if(!libro) {
        return;
    }
    listaLibros.agregarLibro(libro);
    arbolISBN.insertarPorIsbn(libro->getISBNNum(), libro);
    arbolTitulo.insertarPorTitulo(libro->getTitulo(), libro);
    arbolGenero.insertar(libro);
}

void GestorBiblioteca::buscarLibro(){
    int opcion;
    cout<<"---Hora de buscar un libro---"<<endl;
    cout<<" __________________________________________"<<endl;
    cout<<"|       1     Buscar por titulo            |"<<endl;
    cout<<"|       2.     Buscar por ISBN             |"<<endl;
    cout<<"|       3.    Buscar por genero            |"<<endl;
    cout<<"|       4. Buscar por rango de fecha       |"<<endl;
    cout<<"|__________________________________________|"<<endl;
        
    opcion = validarOpcion(1,2);

    if(opcion == 1){
        int opcion1;
            cout<<" __________________________________________"<<endl;
            cout<<"|        1   Busqueda secuencial           |"<<endl;
            cout<<"|        2.    Busqueda binaria            |"<<endl;
            cout<<"|__________________________________________|"<<endl;
        opcion1 = validarOpcion(1, 2);  
        if (opcion1 == 1) {
            string titulo;
            cout << "Ingrese el título del libro a buscar: " << endl;
            std::getline(cin >> std::ws, titulo); 
            listaLibros.buscarPorTitulo(titulo);
        } else if (opcion1 == 2) {
            string titulo;
            cout << "Ingrese el título del libro a buscar: " << endl;
            std::getline(cin >> std::ws, titulo);
            NodoAVL* nodoTitulo = arbolTitulo.buscarPorTitulo(titulo);
            if (nodoTitulo != nullptr) {
                cout << "Libro encontrado:" << endl;
                nodoTitulo->getLibro()->imprimirLibro();
            } else {
                cout << "No se encontró el libro con título: " << titulo << endl;
            }
        }
    }else if(opcion == 2){
        int opcion2;
            cout<<" __________________________________________"<<endl;
            cout<<"|        1   Busqueda secuencial           |"<<endl;
            cout<<"|        2.    Busqueda binaria            |"<<endl;
            cout<<"|__________________________________________|"<<endl;
        opcion2 = validarOpcion(1, 2);  
        if (opcion2 == 1) {
            string isbn;
            cout << "Ingrese el ISBN del libro a buscar: " << endl;
            std::getline(cin >> std::ws, isbn); 
            listaLibros.buscarPorIsbn(isbn);
        } else if (opcion2 == 2) {
            string isbn;
            cout << "Ingrese el ISBN del libro a buscar: " << endl;
            std::getline(cin >> std::ws, isbn);
            long long int isbnNum = convertirISBN(isbn);
            NodoAVL* nodoIsbn = arbolISBN.buscarPorIsbn(isbnNum);
            if (nodoIsbn != nullptr) {
                cout << "Libro encontrado:" << endl;
                nodoIsbn->getLibro()->imprimirLibro();
            } else {
                cout << "No se encontró el libro con ISBN: " << isbn << endl;
            }
        } else if(opcion == 3){
            buscarArbolB();
        }else{
            cout<<"Busqueda por rango de fecha no implementada aun"<<endl;
        }

    }
}

int GestorBiblioteca::validarOpcion(int minimo, int maximo) {
    int opcion;
    bool valido = false;
    do {
        try {
            cout << "Ingresar numero de la busqueda que desea realizar: ";
            cin >> opcion;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Entrada no valida, intente de nuevo.");
            }
            if (opcion < minimo || opcion > maximo) {
                throw out_of_range("Entrada fuera de rango, intente de nuevo...");
            }
            valido = true; 
        } catch (const invalid_argument& e) {
            cout << "ERROR: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cout << "ERROR: " << e.what() << endl;
        }
    } while (!valido);

    return opcion;
}

void GestorBiblioteca::mostrarLibros(){
    int opcion;
    cout<<"---Hora de mostrar los libros---"<<endl;
    cout<<" __________________________________________"<<endl;
    cout<<"|     1. Mostrar arbolAVL por titulo       |"<<endl;
    cout<<"|     2.  Mostrar arbolAVL por isbn        |"<<endl;
    cout<<"|     3.   Mostrar lista de libros         |"<<endl;
    cout<<"|     4.  Mostrar arbolB de libros         |"<<endl;
    cout<<"|__________________________________________|"<<endl;

    opcion = validarOpcion(1,4);
    if(opcion == 1){
        arbolTitulo.inorden();
    }else if(opcion == 2){
        arbolISBN.inorden();
    }else if(opcion == 3){
        listaLibros.imprimirLista();
    }else{
        arbolGenero.mostrarlo();
    }
}


void GestorBiblioteca::eliminarLibro(){
    cout << "---Hora de eliminar un libro---" << endl;
    string titulo;
    cout << "Ingrese el título del libro a eliminar: " << endl;
    std::getline(cin >> std::ws, titulo); 

    NodoAVL* nodoTitulo = arbolTitulo.buscarPorTitulo(titulo);
    if (nodoTitulo == nullptr) {
        cout << "No se encontró el libro con título: " << titulo << endl;
        return;
    }
    arbolISBN.eliminar(titulo);
    arbolTitulo.eliminar(titulo);
    listaLibros.eliminarLibro(titulo);
    cout << "Libro con título \"" << titulo << "\" eliminado correctamente." << endl;
}

void GestorBiblioteca::generarGraphvizArboles(){
    cout << "---Generando archivos Graphviz de los árboles AVL---" << endl;
    arbolISBN.generarGraphviz("arbolISBN.dot", "arbolISBN.png");
    arbolTitulo.generarGraphviz("arbolTitulo.dot", "arbolTitulo.png");
    arbolGenero.generarGraphviz("arbolGenero.dot", "arbolGenero.png");
    cout << "Archivos generados: arbolISBN.dot y arbolTitulo.dot" << endl;
}

void GestorBiblioteca::buscarArbolB(){
    cout << "---Hora de buscar un libro por genero en el arbol B---" << endl;
    string genero;
    cout << "Ingrese el genero del libro a buscar: " << endl;
    std::getline(cin >> std::ws, genero); 

    ListaLibro encontrados;
    Libro* primerEncontrado = arbolGenero.buscar(genero, encontrados);
    if (primerEncontrado == nullptr) {
        cout << "No se encontraron libros con el género: " << genero << endl;
        return;
    }
    cout << "Libros encontrados con el género \"" << genero << "\":" << endl;
    encontrados.imprimirLista();
}