/**
 * @file funciones_aux.h
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __FUNCIONES_AUX_H
#define __FUNCIONES_AUX_H

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <cmath>
#include <iterator>
#include <map>
#include <utility>

typedef std::vector<int> VecInt;
typedef std::vector<int>::iterator VecIntIt;
typedef std::map<int,double> MapDouble;
typedef std::map<int,double>::iterator MapDoubleIt;
typedef std::set<int> SetInt;
typedef std::set<int>::iterator SetIntIt;
typedef std::vector<std::vector<double>> MatDouble;
typedef std::list<int> ListInt;
typedef std::list<int>::iterator ListIntIt;


/******************************************************************/
// Búsqueda Local

/**
 * @brief Algoritmo de búsqueda local del mejor
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Mejor fitness obtenido durante la ejecución
 */
double busquedaLocal_M(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias);

/**
 * @brief Algoritmo de búsqueda local del primero mejor
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Mejor fitness obtenido durante la ejecución
 */
double busquedaLocal_PM(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iter, int iter_max = 100000);

/**
 * @brief Función que compara dos pares de elementos
 * 
 * @param first Par formando por el elemento y su contribución
 * @param second Par formando por el elemento y su contribución
 * @return true first va antes que second
 * @return false second va antes que first
 */
bool compare_menorContri(const std::pair<int,double>& first, const std::pair<int,double>& second);

/**
 * @brief Función que baraja un vector de enteros
 * 
 * @param vector Vector de enteros a barajar
 */
void shuffle(VecInt* vector);

/**
 * @brief Función que intercambia un elemento de la solución por otro
 *        elemento no seleccionado anteriormente
 * 
 * @param Solucion Lista de pares que forman la solución y sus contribuciones
 * @param elemASustituir Iterador que apunta al entero a sustituir de la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param elemAIncluir Iterador que apunta al entero a incluir en la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 */
void Int(std::list<std::pair<int,double>>* Solucion, std::list<std::pair<int,double>>::const_iterator elemASustituir, VecInt* noSeleccionados,
         VecIntIt elemAIncluir, const MatDouble* distancias);

/**
 * @brief Función que intercambia un elemento de la solución por otro
 *        elemento no seleccionado anteriormente
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param elemASustituir Iterador que apunta al entero a sustituir de la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param elemAIncluir Iterador que apunta al entero a incluir en la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 */
void Int(ListInt* Solucion, ListIntIt elemASustituir, VecInt* noSeleccionados,
         VecIntIt elemAIncluir, const MatDouble* distancias);

/**
 * @brief Función que calcula el coste de un solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Diversidad de la solución
 */
double funcion_obj(const ListInt* Solucion, const MatDouble* distancias);

/**
 * @brief Función que calcula el coste de un solución y sus contribuciones
 * 
 * @param Solucion Lista de pares que forman la solución y sus contribuciones
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Diversidad de la solución
 */
double funcion_obj(std::list<std::pair<int,double>>* Solucion, const MatDouble* distancias);

/**
 * @brief Función que calcula el coste de una solución, de manera más eficiente
 *        que la original. Para ello sólo se restan las distancias del elemento sustituido
 *        al resto de elementos de la solución, y se suman las distancias del elemento
 *        incluido al resto de elementos de la solución. Esta función borra el coste
 *        actual de la solución
 * 
 * @param Solucion Lista de pares que forman la solución y sus contribuciones
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param elemSustituido Iterador que apunta al entero sustituido de la solución
 * @param elemIncluido Iterador que apunta al entero incluido en la solución
 * @param coste_actual Coste de la solución
 * @return std::pair<bool,VecIntIt> Pareja que indica con el booleano su ha mejorado o no
 *                                  la solución y si mejora el elemento que se ha incluido
 *                                  para conseguir la mejora
 */
std::pair<bool,VecIntIt> funcion_obj_facto(const std::list<std::pair<int,double>>* Solucion, const MatDouble* distancias,
                       int elemSustituido, VecIntIt elemIncluido, double* coste_actual);

/**
 * @brief Función que calcula el coste de una solución, de manera más eficiente
 *        que la original. Para ello sólo restan las distancias del elemento sustituido
 *        al resto de elementos de la solución, y se suman las distancias del elemento
 *        incluido al resto de elementos de la solución. Esta función no borra el coste
 *        actual de la solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param elemSustituido Iterador que apunta al entero sustituido de la solución
 * @param elemIncluido Iterador que apunta al entero incluido en la solución
 * @param coste_actual Coste de la solución antes de sustituir el elemento
 * @param new_coste Coste de la solución después de sustituir el elemento
 * @return std::pair<bool,VecIntIt> Pareja que indica con el booleano su ha mejorado o no
 *                                  la solución y si mejora el elemento que se ha incluido
 *                                  para conseguir la mejora
 */
std::pair<bool,VecIntIt> funcion_obj_facto(const ListInt* Solucion, const MatDouble* distancias,
                       ListIntIt elemSustituido, VecIntIt elemIncluido, double coste_actual, double* new_coste);

/******************************************************************/
/******************************************************************/
// Greedy

/**
 * @brief Algoritmo Greedy
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numElemSelec Número de elementos a seleccionar para la solución
 * @return double Mejor fitness obtenido durante la ejecución
 */
double Greedy(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int numElemSelec);

/**
 * @brief Función que elige el elemento más optimo para empezar a generar
 *        la solución
 * 
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return VecIntIt Elemento elegido de los no seleccionados
 */
VecIntIt calcularDistAcu(VecInt* noSeleccionados, const MatDouble* distancias);

/**
 * @brief Función que elige el elemento más óptimo para la solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return VecIntIt Elemento elegido de los no seleccionados
 */
VecIntIt elegirSig(ListInt* Solucion, VecInt* noSeleccionados, const MatDouble* distancias);

/******************************************************************/
/******************************************************************/
// Algoritmo genético

/**
 *  Clase que representa a un individuo o a un cromosoma
 */
struct Individuo {
    VecInt genes;
    double fitness;
    // Variable que indica si es necesario o no recalcular su fitness
    bool actualizado;

    // Constructores
    Individuo();
    Individuo(int tam, int genesActivos, const MatDouble* distancias);
    // Contructor de copia
    Individuo(const Individuo* otro);

    // Sobrecarga de operadores
    bool operator==(const Individuo& otro) const;

    /**
     * @brief Método para realizar un cruce uniforme entre dos soluciones
     * 
     * @param otro Segunda solución
     * @return Individuo Solución resultado del cruce
     */
    Individuo cruce_uniforme(const Individuo* otro);

    /**
     * @brief Método para realizar un cruce posición entre dos soluciones
     * 
     * @param otro Segunda solución
     * @return Individuo Solución resultado del cruce
     */
    Individuo cruce_posicion(const Individuo* otro);

    /**
     * @brief Método para convertir en factible una solución
     * 
     * @param numGenesFactible Número de elementos para que una solución sea factible
     * @param distancias Matriz de distancias entre cada par de elementos
     */
    void repair(int numGenesFactible, const MatDouble* distancias);

    /**
     * @brief Método para eliminar elementos seleccionados
     * 
     * @param numElementos Número de elementos a eliminar de la solución
     * @param distancias Matriz de distancias entre cada par de elementos
     */
    void eliminarGenesActivos(int numElementos, const MatDouble* distancias);
    
    /**
     * @brief Método para añadir elementos no seleccionados
     * 
     * @param numElementos Número de elementos a añadir a la solución
     * @param distancias Matriz de distancias entre cada par de elementos
     */
    void aniadirGenesActivos(int numElementos, const MatDouble* distancias);
}; 

/**
 * @brief Algoritmo genético generacional con cruce uniforme
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AGG_uniforme(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Algoritmo genético generacional con cruce posición
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AGG_posicion(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Algoritmo genético estacionario con cruce uniforme
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AGE_uniforme(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Algoritmo genético estacionario con cruce posición
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AGE_posicion(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Función para obtener el fitness de una solución con representación binaria
 * 
 * @param Solucion Vector de enteros que representa una solución en binario
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Fitness de la solución
 */
double funcion_obj_binaria(const VecInt* Solucion, const MatDouble* distancias);

/**
 * @brief Función para comparar dos soluciones según su fitness
 * 
 * @param first Primera solución
 * @param second Segunda solución
 * @return true La primera solución va antes que la segunda solución
 * @return false La segunda solución va antes que la primera solución
 */
bool compare_mayorFitness(const Individuo& first, const Individuo& second);

/**
 * @brief Función para comparar dos pares según su contribución
 * 
 * @param first Par que forma un elemento y su contribución
 * @param second Par que forma un elemento y su contribución
 * @return true El primer par va antes que el segundo par
 * @return false El segundo par va antes que el primer par
 */
bool compare_mayorContri(const std::pair<int,double>& first, const std::pair<int,double>& second);

/**
 * @brief Operador para realizar una selección en la población (Generacional)
 * 
 * @param poblacion Lista de individuos que forman la población
 */
void operador_seleccion(std::list<Individuo>* poblacion);

/**
 * @brief Operador para realizar una selección de tamaño variable en 
 *        la población (Estacionario)
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param nuevaPoblacion Lista de individuos que forman la solución resultado
 *                       de la selección
 * @param tamNuevaPoblacion Número de individuos que habrá en la nueva solución
 */
void operador_seleccion(const std::list<Individuo>* poblacion, std::list<Individuo>* nuevaPoblacion, int tamNuevaPoblacion);

/**
 * @brief Función que realiza un torneo de cierto tamaño
 * 
 * @param poblacion Lista de individuos
 * @param tam Número de individuos que se podrán elegir para el torneo
 * @param tamTorneo Número de individuos para realizan el torneo
 * @return Individuo Individuo que gana el torneo
 */
Individuo seleccionTorneo(const std::list<Individuo>* poblacion, int tam, int tamTorneo);

/**
 * @brief Operador para realizar el cruce uniforme
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param numGenesFactible Número de elementos que hacen factible una solución
 * @param distancias Matriz de distancias entre cada par de elementos
 */
void operador_cruce_uniforme(std::list<Individuo>* poblacion, float probabilidadCruce, int numGenesFactible, const MatDouble* distancias);

/**
 * @brief Operador para realizar el cruce posición
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 */
void operador_cruce_posicion(std::list<Individuo>* poblacion, float probabilidadCruce);

/**
 * @brief Operador para realizar la mutación de una población
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param probabilidadMutacion Probabilidad de que un gen mute
 */
void operadorMutacion(std::list<Individuo>* poblacion, float probabilidadMutacion);

/**
 * @brief Operador para realizar el reemplazo de la nueva solución como 
 *        solución actual (Generacional)
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param mejorPadre Individuo con mejor fitness de la solución actual
 */
void operador_reemplazo(std::list<Individuo>* poblacion, Individuo* mejorPadre);

/**
 * @brief Operador para realizar el reemplazo de la nueva solución como
 *        solución actual (Estacionario)
 * 
 * @param poblacion Lista de individuos que forman la solución
 * @param nuevaPoblacion Lista de individuos que intentan entrar en la población actual
 */
void operador_reemplazo(std::list<Individuo>* poblacion, const std::list<Individuo>* nuevaPoblacion);

/**
 * @brief Función para calcular el fitness de los individuos de la población
 *        que lo necesiten
 *  
 * @param poblacion Lista de individuos que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return int Número de evaluaciones de la función objetivo
 */
int calcularFitness(std::list<Individuo>* poblacion, const MatDouble* distancias);

/******************************************************************/
/******************************************************************/
// Algoritmo genético

/**
 * @brief Algoritmo memético (10, 1.0)
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AM_10_1_0(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Algoritmo memético (10, 0.1)
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AM_10_0_1(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Algoritmo memético (10, 0.1) de los mejores
 * 
 * @param poblacionIni Lista de individuos que forman la solución inicial
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numGenesFactible Número de elementos necesarios para que una solución sea factible
 * @param probabilidadCruce Probabilidad de que una pareja de individuos se cruce
 * @param probabilidadMutacion Probabilidad de que un gen mute
 * @return double Mejor fitness encontrado en la ejecución del algoritmo
 */
double AM_10_0_1_mej(std::list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion);

/**
 * @brief Función para convertir una solución con representación binaria en una
 *        solución con representación entera
 * 
 * @param solucion_binaria Vector de 0's y 1's que forman la solución binaria
 * @param seleccionados Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 */
void convertirSolucion(std::list<Individuo>::iterator solucion_binaria, ListInt* seleccionados, VecInt* noSeleccionados);

/**
 * @brief Función para convertir una solución con representación entera en una
 *        solución con representación binaria
 * 
 * @param solucion_binaria Vector de 0's y 1's que forman la solución binaria
 * @param seleccionados Lista de enteros que forman la solución
 */
void recuperarSolucion(std::list<Individuo>::iterator solucion_binaria, ListInt* seleccionados);

/******************************************************************/
/******************************************************************/
// Algoritmo de enfriamiento simulado

double funcion_obj(const VecInt* Solucion, const MatDouble* distancias);

double funcion_obj_facto(const VecInt* Solucion, const MatDouble* distancias,
                       int elemSustituido, int elemIncluido, double coste_actual);

void Int(VecInt* Solucion, int elemASustituir, VecInt* noSeleccionados,
         int elemAIncluir, const MatDouble* distancias);

double enfriarTemperatura(double temp, double temp_ini, double temp_final, int iteraciones);

void generarSolAleatoria(ListInt* Solucion, VecInt* noSeleccionados, int m, int n);

double ES(VecInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iter, int iter_max = 100000);

double funcion_obj_facto(const std::list<std::pair<int,double>>* Solucion, const MatDouble* distancias,
                       std::list<std::pair<int,double>>::iterator elemSustituido, int elemIncluido, double coste_actual);

void Int(std::list<std::pair<int,double>>* Solucion, std::list<std::pair<int,double>>::iterator elemASustituir, VecInt* noSeleccionados,
         int elemAIncluir, const MatDouble* distancias);

double ES_inte(VecInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iter, int iter_max = 100000);

/******************************************************************/
/******************************************************************/
// Algoritmo ILS

void operador_mutacion_ILS(ListInt* Solucion, VecInt* noSeleccionados,const MatDouble* distancias, int t);

void operador_mutacion_ILS(VecInt* Solucion, VecInt* noSeleccionados,const MatDouble* distancias, int t);

void generarSolAleatoria(VecInt* Solucion, VecInt* noSeleccionados, int m, int n);

/******************************************************************/

/**
 * @brief Función que obtiene los datos del problema a partir de un fichero plano
 * 
 * @param nombre Nombre del archivo que contiene los datos
 * @param numElemSelec Número de elementos que debe contener la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 */
void leerArchivo(const char* nombre, int &numElemSelec, MatDouble &distancias);

#endif
