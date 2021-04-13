.PHONY: clean

# Carpetas
BIN = bin
OBJ = obj
INC = include
SRC = src
TABLAS = tablas

# Extensión de ficheros fuente
CPP = cpp
C = c

# Compilador
CXX= g++

# Ficheros fuente auxiliares
SOURCES_AUX = funciones_aux timer

# Ficheros objeto auxiliares
OBJS_AUX = $(addsuffix .o, $(addprefix $(OBJ)/, $(SOURCES_AUX)))

# Ficheros ejecutables
TARGET = BL_PM BL_M Greedy

# Opciones de compilación
CPPFLAGS_DEBUG = -Wall -fmax-errors=2 -O0 -g -I$(INC)
CPPFLAGS = -O2 -I$(INC)



all: ejecutables debug 

ejecutables: directorios $(addprefix $(BIN)/, $(TARGET))

debug: directorios $(addprefix $(BIN)/, $(addsuffix _debug, $(TARGET)))


## Creación de directorios ##
directorios: ./$(BIN) ./$(OBJ) ./$(TABLAS)

./$(BIN):
	~Creando $@...
	@mkdir ./$(BIN)

./$(OBJ):
	~Creando $@...
	@mkdir ./$(OBJ)

./$(TABLAS):
	~Creando $@...
	@mkdir ./$(TABLAS)


## Compilación ##

# Creación de ejecutables
$(BIN)/%: $(OBJ)/%.o $(OBJS_AUX)
	@echo ~Creando $@...
	@$(CXX) $^ $(CPPFLAGS) -o $@


# Creación de ejecutables para depurar
$(BIN)/%_debug: $(OBJ)/%.o $(OBJS_AUX)
	@echo ~Creando $@...
	@$(CXX) $^ $(CPPFLAGS_DEBUG) -o $@
	

# Creación de los ficheros objeto
$(OBJ)/%.o: $(SRC)/%.$(CPP)
	@echo ~Creando $@...
	@$(CXX) -c $< $(CPPFLAGS) -o $@ 

$(OBJ)/%.o: $(SRC)/%.$(C)
	@echo ~Creando $@...
	@$(CXX) -c $< $(CPPFLAGS) -o $@ 


## Ejecución del script de tiempos ##
tiempos: ejecutables
	@chmod +x ./script_tiempos.sh
	@./script_tiempos.sh


## Borrar tablas ##
clean_tablas:
	@rm -rf $(TABLAS)/*.csv


## Borrar ##
clean:
	@rm -rf $(OBJ)/*.o $(BIN)/*


