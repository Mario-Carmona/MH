.PHONY: clean

BIN = bin
OBJ = obj
INC = include
SRC = src
DOC = doc

CPP = cpp

SOURCES = $(wildcard $(SRC)/*.$(CPP))
OBJS = $(addprefix $(OBJ)/, $(notdir $(SOURCES:.$(CPP)=.o)))

TARGET = BL_PM
      
CPPFLAGS = -Wall -fmax-errors=2 -g -I$(INC)

CXX= g++

all: directorios $(BIN)/$(TARGET) 
	./$(BIN)/$(TARGET) 0 ./data/MDG-a_1_n500_m50.txt

## Creación de directorios ##

directorios: ./$(BIN) ./$(OBJ)

./$(BIN):
	~Creando $@...
	@mkdir ./$(BIN)

./$(OBJ):
	~Creando $@...
	@mkdir ./$(OBJ)

./$(DOC):
	~Creando $@...
	@mkdir ./$(DOC)

## Compilación ##

$(BIN)/$(TARGET): $(OBJS)
	@echo ~Creando $@...
	@$(CXX) $(OBJS) $(CPPFLAGS) -o $@

$(OBJ)/%.o: $(SRC)/%.$(CPP)
	@echo ~Creando $@...
	@$(CXX) -c $< $(CPPFLAGS) -o $@ 

## Documentación ##

documentacion: clean_docu ./$(DOC)
	#doxywizard
	@doxygen Doxyfile
	@ln -s $(DOC)/html/index.html Practica3\ -\ Doxygen.html

## Crear comprimido ##

zip: clean_all
	@echo ~Creando .zip
	@rm -rf ./$(DOC)
	@zip -r Practica3.zip ./*

## Borrar ##

clean_docu:
	@rm -rf Practica3\ -\ Doxygen.html
	@rm -rf $(DOC)/*

clean:
	@rm -rf $(OBJ)/*.o $(BIN)/*

clean_all: clean clean_docu
