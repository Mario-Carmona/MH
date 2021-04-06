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

typedef std::vector<int> VecInt;
typedef std::vector<int>::iterator VecIntIt;
typedef std::vector<double> VecDouble;
typedef std::vector<double>::iterator VecDoubleIt;
typedef std::set<int> SetInt;
typedef std::set<int>::iterator SetIntIt;
typedef std::vector<std::vector<double>> MatDouble;

void shuffle(std::vector<int>* vector);

void Int(SetInt* Sel, SetIntIt* i, VecDoubleIt* pos_i, VecInt* noSeleccionados,
         VecIntIt j, const MatDouble* distancias, VecDouble* contri);

double funcion_obj(const SetInt* Sel, const MatDouble* distancias);

double funcion_obj(const SetInt* Sel, const MatDouble* distancias, VecDouble *contri);

bool funcion_obj_facto(const SetInt* Sel, const MatDouble* distancias,
                       SetIntIt i, VecIntIt j, double* coste_actual);

bool funcion_obj_facto(const SetInt* Sel, const MatDouble* distancias,
                       SetIntIt i, VecIntIt j, double coste_actual, double* new_coste);

void leerArchivo(const char* nombre, int &numElemSelec, MatDouble &distancias);

#endif
