#include "GestorBiblioteca.h"
#include <iostream>
#include <limits>
#include <ratio>
#include <stdexcept>
#include <fstream>
#include <string>
#include <chrono>
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
            cargarLibroManual();
            menuFinAccion();
            break;
        case 2:
            leerArchivo();
            menuFinAccion();
            break;
        case 3:
            buscarLibro();
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
            compararTiempos();
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


bool GestorBiblioteca::validarISBN(std::string isbn) {
    if (isbn.front() == '-' || isbn.back() == '-') {
        return false;
    }

    int digitos = 0;
    for (char c : isbn) {
        if (std::isdigit(c)) {
            digitos++;
        } else if (c != '-') {
            return false;
        }
    }

    return digitos == 13;
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
    if(!libro) return;

    Libro* existente = listaLibros.buscarPorIsbn(libro->getISBN());
    if (existente) {
        existente->setNumCopias(existente->getNumCopias() + 1);
        delete libro; // Elimininando ya que si existe
        return;
    }

    listaLibros.agregarLibro(libro);
    arbolISBN.insertarPorIsbn(libro->getISBNNum(), libro);
    arbolTitulo.insertarPorTitulo(libro->getTitulo(), libro);
    arbolGenero.insertar(libro);
    arbolAnio.insertar(libro);
}


void GestorBiblioteca::buscarLibro() {
    cout << "---Hora de buscar un libro---" << endl;
    cout << " __________________________________________" << endl;
    cout << "|       1     Buscar por titulo            |" << endl;
    cout << "|       2.    Buscar por ISBN              |" << endl;
    cout << "|       3.    Buscar por genero            |" << endl;
    cout << "|       4.    Buscar por rango de fecha    |" << endl;
    cout << "|__________________________________________|" << endl;

    int opcion = validarOpcion(1, 4);

    if (opcion == 1 || opcion == 2) {
        string tipoBusqueda = (opcion == 1) ? "titulo" : "ISBN";
        cout << " __________________________________________" << endl;
        cout << "|        1   Busqueda secuencial           |" << endl;
        cout << "|        2.   Busqueda binaria             |" << endl;
        cout << "|__________________________________________|" << endl;
        int opcionMetodo = validarOpcion(1, 2);

        cout << "Ingrese el " << tipoBusqueda << " del libro a buscar: " << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        string valorBusqueda;
        std::getline(cin >> std::ws, valorBusqueda);

        auto inicio = std::chrono::high_resolution_clock::now();

        if (opcion == 1) { // Título
            if (opcionMetodo == 1) {
                Libro* busqueda = listaLibros.buscarPorTitulo(valorBusqueda);
                auto fin = std::chrono::high_resolution_clock::now();
                duracionBusquedaSTitulo = static_cast<int>(
                    std::chrono::duration<double, std::nano>(fin - inicio).count()
                );
                if (busqueda) {
                    cout << "Libro encontrado:" << endl;
                    busqueda->imprimirLibro();
                } else {
                    cout << "No se encontró el libro con título: " << valorBusqueda << endl;
                }

            } else {
                NodoAVL* nodo = arbolTitulo.buscarPorTitulo(valorBusqueda);
                auto fin = std::chrono::high_resolution_clock::now();
                duracionBusquedaBTitulo = static_cast<int>(
                    std::chrono::duration<double, std::nano>(fin - inicio).count()
                );
                if (nodo) {
                    cout << "Libro encontrado:" << endl;
                    nodo->getLibro()->imprimirLibro();
                } else {
                    cout << "No se encontró el libro con título: " << valorBusqueda << endl;
                }
            }
        } else { // ISBN
            if (opcionMetodo == 1) {
                Libro* busqueda = listaLibros.buscarPorIsbn(valorBusqueda);
                auto fin = std::chrono::high_resolution_clock::now();
                duracionBusquedaSISBN = static_cast<int>(
                    std::chrono::duration<double, std::nano>(fin - inicio).count()
                );
                if (busqueda) {
                    cout << "Libro encontrado:" << endl;
                    busqueda->imprimirLibro();
                } else {
                    cout << "No se encontró el libro con ISBN: " << valorBusqueda << endl;
                }
            } else {
                long long int isbnNum = convertirISBN(valorBusqueda);
                NodoAVL* nodo = arbolISBN.buscarPorIsbn(isbnNum);
                auto fin = std::chrono::high_resolution_clock::now();
                duracionBusquedaBISBN = static_cast<int>(
                    std::chrono::duration<double, std::nano>(fin - inicio).count()
                );
                if (nodo) {
                    cout << "Libro encontrado:" << endl;
                    nodo->getLibro()->imprimirLibro();
                } else {
                    cout << "No se encontró el libro con ISBN: " << valorBusqueda << endl;
                }
            }
        }

    } else if (opcion == 3) {
        buscarArbolB();
    } else {
        buscarPorRangoAnio();
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
    cout<<"|     5.  Mostrar arbolB+ de libros        |"<<endl;
    cout<<"|__________________________________________|"<<endl;

    opcion = validarOpcion(1,5);
    if(opcion == 1){
        arbolTitulo.inorden();
    }else if(opcion == 2){
        arbolISBN.inorden();
    }else if(opcion == 3){
        listaLibros.imprimirLista();
    }else if(opcion == 4){
        arbolGenero.inOrden();
    }else {
        arbolAnio.mostrar();
    }
}


void GestorBiblioteca::eliminarLibro() {
    cout << "---Hora de eliminar un libro---" << endl;
    string titulo;
    cout << "Ingrese el título del libro a eliminar: " << endl;
    std::getline(cin >> std::ws, titulo);

    NodoAVL* nodoTitulo = arbolTitulo.buscarPorTitulo(titulo);
    if (!nodoTitulo) {
        cout << "No se encontró el libro con título: " << titulo << endl;
        return;
    }
    if( nodoTitulo->getLibro()->getNumCopias() > 1){
        nodoTitulo->getLibro()->setNumCopias(nodoTitulo->getLibro()->getNumCopias()-1);
        cout << "Se ha reducido el número de copias del libro \"" << titulo << "\". Copias restantes: " << nodoTitulo->getLibro()->getNumCopias() << endl;
        return;
    }
    Libro* libro = nodoTitulo->getLibro();

    arbolISBN.eliminarIsbn(libro->getISBNNum());
    arbolTitulo.eliminarTitulo(titulo);     
    arbolGenero.eliminar(libro->getGenero(), libro);
    arbolAnio.eliminarPorTitulo(titulo);   
    listaLibros.eliminarLibro(titulo);
    cout << "Libro con título \"" << titulo << "\" eliminado correctamente." << endl;
}

void GestorBiblioteca::generarGraphvizArboles(){
    cout << "---Generando archivos Graphviz de los árboles AVL---" << endl;
    arbolISBN.generarGraphviz("arbolISBN.dot", "arbolISBN.png");
    arbolTitulo.generarGraphviz("arbolTitulo.dot", "arbolTitulo.png");
    arbolGenero.generarGraphviz("arbolGenero.dot", "arbolGenero.png");
    arbolAnio.generarGraphviz("arbolAnio.dot", "arbolAnio.png");
    cout << "Archivos Graphviz generados correctamente." << endl;}

void GestorBiblioteca::buscarArbolB(){
    cout << "---Hora de buscar un libro por genero en el arbol B---" << endl;
    string genero;
    cout << "Ingrese el genero del libro a buscar: " << endl;
    std::getline(cin >> std::ws, genero); 

    ListaLibro encontrados;
    arbolGenero.buscar(genero, encontrados);
    if (encontrados.estaVacia()) {
        cout << "No se encontraron libros con el género: " << genero << endl;
        return;
    }
    cout << "Libros encontrados con el género \"" << genero << "\":" << endl;
    encontrados.imprimirLista();
}

void GestorBiblioteca::buscarPorRangoAnio(){
    cout << "---Hora de buscar libros por rango de año en el arbol B+---" << endl;
    int anioInicial, anioFinal;
    cout << "Ingrese el año inicial: " << endl;
    cin >> anioInicial; 
    cout << "Ingrese el año final: " << endl;
    cin >> anioFinal;

    ListaLibro encontrados;
    Libro* primerEncontrado = arbolAnio.buscarPorRangoAnio(anioInicial, anioFinal, encontrados);
    if (primerEncontrado == nullptr) {
        cout << "No se encontraron libros del año: " << anioInicial<<" al año final: "<<anioFinal << endl;
        return;
    }
    cout << "Libros encontrados del año " << anioInicial << " al año " << anioFinal << ":" << endl;
    encontrados.imprimirLista();
}

void GestorBiblioteca::compararTiempos(){
    cout << "---Comparando tiempos de busqueda---" << endl;
    cout << "Tiempo de busqueda secuencial por titulo: " << duracionBusquedaSTitulo << " ms" << endl;
    cout << "Tiempo de busqueda binaria por titulo: " << duracionBusquedaBTitulo << " ms" << endl;
    cout << "Tiempo de busqueda secuencial por ISBN: " << duracionBusquedaSISBN << " ms" << endl;
    cout << "Tiempo de busqueda binaria por ISBN: " << duracionBusquedaBISBN << " ms" << endl;
}

void GestorBiblioteca::cargarLibroManual(){
    cout << "---Hora de cargar un libro manualmente---" << endl;
    string titulo, isbn, genero, autor;
    int anio;
    do{
        cout << "Ingrese el título del libro: ";
        std::getline(cin >> std::ws, titulo);
        cout << "Ingrese el ISBN del libro (formato 13 digitos): ";
        std::getline(cin >> std::ws, isbn);
        cout << "Ingrese el género del libro: ";
        std::getline(cin >> std::ws, genero);
        cout << "Ingrese el año de publicación del libro: ";
        cin >> anio;
        if(cin.fail()){
            cin.clear(); 
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            anio = -1;
        }
        cout << "Ingrese el autor del libro: ";
        std::getline(cin >> std::ws, autor);
    }while(!validarISBN(isbn) || titulo.empty() || genero.empty() || autor.empty() || anio <= 0);

    Libro* nuevoLibro = new Libro();
    nuevoLibro->setTitulo(titulo);
    nuevoLibro->setISBN(isbn);
    nuevoLibro->setISBNNum(convertirISBN(isbn));
    nuevoLibro->setGenero(genero);
    nuevoLibro->setAnio(anio);
    nuevoLibro->setAutor(autor);
    nuevoLibro->setNumCopias(1); 
    cargarLibros(nuevoLibro);
    cout << "Libro cargado exitosamente." << endl;
}