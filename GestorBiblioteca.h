#ifndef GESTOR_BIBLIOTECA
#define GESTOR_BIBLIOTECA

class GestorBilbioteca{
    private:

    
    public: 
        void bienvenidaInicial();
        void menuJuego();
        void realizarAccion(int& opcion);
        void menuFinAccion();
        void cargarLibros();
        bool leerArchivo();

};

#endif 