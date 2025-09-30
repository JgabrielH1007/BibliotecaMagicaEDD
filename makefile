# Nombre del compilador
CXX = g++ 

# Opciones de compilación
CXXFLAGS = -Wall -std=c++11

# Archivos fuente y objetos
SRC = main.cpp GestorBiblioteca.cpp ListaLibro.cpp ArbolAVL.cpp ArbolB.cpp ArbolB+.cpp
OBJ = $(SRC:.cpp=.o)

# Nombre del ejecutable
TARGET = biblioteca

# Regla principal
all: $(TARGET)

# Cómo construir el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar cada .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(TARGET)
