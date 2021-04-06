/**
 * @file funciones_aux.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>

using namespace std;

void shuffle(vector<int>* vector) {
    for(int i = vector->size()-1; i > 0; --i) {
        int random = rand() % (i+1);
        int a = (*vector)[random];
        int b = (*vector)[i];
        (*vector)[random] = b;
        (*vector)[i] = a;
    }
}

void Int(SetInt* Sel, SetIntIt* i, VecDoubleIt* pos_i, VecInt* noSeleccionados,
         VecIntIt j, const MatDouble* distancias, VecDouble* contri) {

    double contri_j = 0;
    SetIntIt auxIt = Sel->begin();

    for(int aux = 0; aux < (int)Sel->size(); ++aux, ++auxIt) {
        if(auxIt != *i) {
            if(*auxIt > **i) {
                (*contri)[aux] -= (*distancias)[**i][*auxIt-**i-1];
            }
            else {
                (*contri)[aux] -= (*distancias)[*auxIt][**i-*auxIt-1];
            }

            double dist;
            if(*auxIt > *j) {
                dist = (*distancias)[*j][*auxIt-*j-1];
            }
            else {
                dist = (*distancias)[*auxIt][*j-*auxIt-1];
            }

            (*contri)[aux] += dist;
            contri_j += dist;                      
        }
    }

    int a = **i;
    int b = *j;

    noSeleccionados->erase(j);
    noSeleccionados->push_back(a);

    Sel->erase(*i);
    Sel->insert(Sel->end(), b);

    contri->erase(*pos_i);
    contri->insert(contri->end(), contri_j);

    *i = Sel->begin();
    *pos_i = contri->begin();

    auto it = ++Sel->begin();

    for(auto aux = ++contri->begin(); aux != contri->end(); ++aux, ++it) {
        if(*aux < **pos_i) {
            *pos_i = aux;
            *i = it;
        }
    }
}

double funcion_obj(const SetInt* Sel, const MatDouble* distancias) {
    double total = 0.0;

    for(auto i = Sel->cbegin(); i != --Sel->cend(); ++i) {
        auto j = i;
        ++j;
        for(; j != Sel->cend(); ++j) {
            if(*i < *j) {
                total += (*distancias)[*i][*j-*i-1];
            }
            else {
                total += (*distancias)[*j][*i-*j-1];
            }
        }
    }

    return total;
}

double funcion_obj(const SetInt* Sel, const MatDouble* distancias, VecDouble *contri) {
    double total = 0.0;

    int contador_i = 0;

    for(auto i = Sel->cbegin(); i != --Sel->cend(); ++i, ++contador_i) {
        int contador_j = contador_i+1;
        auto j = i;
        ++j;
        for(; j != Sel->cend(); ++j, ++contador_j) {
            double dist;
            if(*i < *j) {
                dist = (*distancias)[*i][*j-*i-1];
            }
            else {
                dist = (*distancias)[*j][*i-*j-1];
            }

            total += dist;
            (*contri)[contador_i] += dist;
            (*contri)[contador_j] += dist;
        }
    }

    return total;
}

bool funcion_obj_facto(const SetInt* Sel, const MatDouble* distancias,
                       SetIntIt i, VecIntIt j, double* coste_actual) {
    
    double nuevo_coste = (*coste_actual);

    for(auto aux = Sel->begin(); aux != Sel->end(); ++aux) {
        if(aux != i) {
            if(*aux > *i) {
                nuevo_coste -= (*distancias)[*i][*aux-*i-1];
            }
            else {
                nuevo_coste -= (*distancias)[*aux][*i-*aux-1];
            }

            if(*aux > *j) {
                nuevo_coste += (*distancias)[*j][*aux-*j-1];
            }
            else {
                nuevo_coste += (*distancias)[*aux][*j-*aux-1];
            }
        }
    }

    if(nuevo_coste > (*coste_actual)){
        (*coste_actual) = nuevo_coste;
        return true;
    }
    else{
        return false;
    }
}

bool funcion_obj_facto(const SetInt* Sel, const MatDouble* distancias,
                       SetIntIt i, VecIntIt j, double coste_actual, double* new_coste) {

    (*new_coste) = coste_actual;

    for(auto aux = Sel->begin(); aux != Sel->end(); ++aux) {
        if(aux != i) {
            if(*aux > *i) {
                (*new_coste) -= (*distancias)[*i][*aux-*i-1];
            }
            else {
                (*new_coste) -= (*distancias)[*aux][*i-*aux-1];
            }

            if(*aux > *j) {
                (*new_coste) += (*distancias)[*j][*aux-*j-1];
            }
            else {
                (*new_coste) += (*distancias)[*aux][*j-*aux-1];
            }
        }
    }

    if((*new_coste) > coste_actual){
        return true;
    }
    else{
        return false;
    }
}

void leerArchivo(const char* nombre, int &numElemSelec, MatDouble &distancias) {
    ifstream archivo(nombre);
    if(archivo.is_open()) {
        int numElecTotal;
        archivo >> numElecTotal;

        archivo >> numElemSelec;

        // Creación de la matriz triangular superior
        distancias.resize(numElecTotal-1);
        for(int i = 0; i < numElecTotal; ++i) {
            distancias[i].resize(numElecTotal-1-i);
        }

        int fil,col;
        double dist;

        while(!archivo.eof()) {
            archivo >> fil;
            archivo >> col;
            archivo >> dist;

            distancias[fil][col-fil-1] = dist;
        }
    }
    else {
        cerr << "Error: Apertura del archivo" << endl;
    }
}
