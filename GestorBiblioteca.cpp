#include "GestorBiblioteca.h"
#include <ios>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

void GestorBilbioteca::bienvenidaInicial(){
    cout<<"--------------------------"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"|    BIENVENIDO A LA     |"<<endl;
    cout<<"|   BIBLIOTECA MAGICA    |"<<endl;
    cout<<"|                        |"<<endl;
    cout<<"--------------------------"<<endl;
    menuJuego();
}

void GestorBilbioteca::menuJuego(){
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

void GestorBilbioteca::menuFinAccion(){
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

void GestorBilbioteca::realizarAccion(int& opcion){
    switch (opcion) {
        case 1: 
            cout<<"Agregando libro"<<endl;
            menuFinAccion();
            break;
        case 2:
            cout<<"Cargando archivo"<<endl;
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
