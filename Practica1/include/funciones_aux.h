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


/******************************************************************/
// Búsqueda Local

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
 * @param Solucion Lista de enteros que forman la solución
 * @param elemASustituir Iterador que apunta al entero a sustituir de la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param elemAIncluir Iterador que apunta al entero a incluir en la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param contri Contribuciones de cada elemento a la distancia total de la solución
 */
void Int(SetInt* Solucion, SetIntIt elemASustituir, VecInt* noSeleccionados,
         VecIntIt elemAIncluir, const MatDouble* distancias, MapDouble* contri);

/**
 * @brief Función que calcula el coste de un solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Diversidad de la solución
 */
double funcion_obj(const SetInt* Solucion, const MatDouble* distancias);

/**
 * @brief Función que calcula el coste de un solución y sus contribuciones
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param contri Contribuciones de cada elemento a la distancia total de la solución
 * @return double Diversidad de la solución
 */
double funcion_obj(const SetInt* Solucion, const MatDouble* distancias, MapDouble *contri);

/**
 * @brief Función que calcula el coste de una solución, de manera más eficiente
 *        que la original. Para ello sólo restan las distancias del elemento sustituido
 *        al resto de elementos de la solución, y se suman las distancias del elemento
 *        incluido al resto de elementos de la solución. Esta función borra el coste
 *        actual de la solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param elemSustituido Iterador que apunta al entero sustituido de la solución
 * @param elemIncluido Iterador que apunta al entero incluido en la solución
 * @param coste_actual Coste de la solución
 * @return std::pair<bool,VecIntIt> Pareja que indica con el booleano su ha mejorado o no
 *                                  la solución y si mejora el elemento que se ha incluido
 *                                  para conseguir la mejora
 */
std::pair<bool,VecIntIt> funcion_obj_facto(const SetInt* Solucion, const MatDouble* distancias,
                       SetIntIt elemSustituido, VecIntIt elemIncluido, double* coste_actual);

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
std::pair<bool,VecIntIt> funcion_obj_facto(const SetInt* Solucion, const MatDouble* distancias,
                       SetIntIt elemSustituido, VecIntIt elemIncluido, double coste_actual, double* new_coste);

/******************************************************************/
/******************************************************************/
// Greedy

/**
 * @brief Función que elige el elemento más optimo para empezar a generar
 *        la solución
 * 
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return SetIntIt 
 */
SetIntIt calcularDistAcu(SetInt* noSeleccionados, const MatDouble* distancias);

/**
 * @brief Función que elige el elemento más óptimo para la solución
 * 
 * @param Solucion Lista de enteros que forman la solución
 * @param noSeleccionados Lista de enteros que no forman parte de la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return SetIntIt 
 */
SetIntIt elegirSig(SetInt* Solucion, SetInt* noSeleccionados, const MatDouble* distancias);

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
