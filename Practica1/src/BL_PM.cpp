/**
 * @file main.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <ctime>
#include <vector>
#include <set>
#include <list>
#include <iterator>


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

void Int(set<int>* Sel, set<int>::iterator* i, vector<double>::iterator* pos_i, vector<int>* noSeleccionados, vector<int>::iterator j, const vector<vector<double>>* distancias, vector<double> *contri) {

    double contri_j = 0;
    set<int>::iterator auxIt = Sel->begin();

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

double funcion_obj(const set<int>* Sel, const vector<vector<double>>* distancias, vector<double> *contri) {
    double total = 0;

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

bool funcion_obj_facto(const set<int>* Sel, const vector<vector<double>>* distancias, set<int>::iterator i, vector<int>::iterator j, double* coste_actual) {

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


double busquedaLocal_PM(set<int> *Sel, vector<int> *noSeleccionados, const vector<vector<double>> *distancias) {

    bool terminado = false;
    int iteraciones = 0;
    vector<double> contri(Sel->size());
    vector<double>::iterator minContri = contri.begin();
    set<int>::iterator minContriIt = Sel->begin();
    set<int>::iterator it = ++Sel->begin();

    double coste_actual = funcion_obj(Sel, distancias, &contri);

    for(auto i = ++contri.begin(); i != contri.end(); ++i, ++it) {
        if(*i < *minContri) {
            minContri = i;
            minContriIt = it;
        }
    }

    while(iteraciones != 100000 && !terminado) {
        
        bool mejorado = false;
        terminado = true;
        vector<int>::iterator j;

        shuffle(noSeleccionados);

        for(j = noSeleccionados->begin(); j != noSeleccionados->end() && !mejorado; ++j) {
            
            mejorado = funcion_obj_facto(Sel, distancias, minContriIt, j, &coste_actual);
        }

        if(mejorado) {
            // Se va para atras porque al comprobar la condicción del for se le hace un ++
            --j;
            Int(Sel, &minContriIt, &minContri, noSeleccionados, j, distancias, &contri);

            terminado = false;
        }

        iteraciones++;
    }

    return coste_actual;
}

void leerArchivo(const char* nombre, int &numElemSelec, 
                 vector<vector<double>> &distancias) {
                     
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


int main(int argc, char* argv[]) {

    // Semilla
    int seed = atoi(argv[1]);

    char* fichero = argv[2];

    int numElemSelec;

    vector<vector<double>> distancias;

    leerArchivo(fichero, numElemSelec, distancias);

    srand(seed);

    set<int> Sel;
    while((int)Sel.size() < numElemSelec) {
        int aux = rand() % (distancias.size()+1);
        Sel.insert(Sel.cend(), aux);
    }
    
    vector<int> noSeleccionados;
    for(int i = 0; i < (int)distancias.size()+1; ++i) {
        if(Sel.find(i) == Sel.end()) {
            noSeleccionados.push_back(i);
        }
    }

    auto inicio = clock();

    double coste_final = busquedaLocal_PM(&Sel, &noSeleccionados, &distancias);

    auto fin = clock();

    auto tiempo_ejec = (double(fin - inicio)/CLOCKS_PER_SEC);

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Coste final: " << coste_final << endl;
    cout << "Seleccionados → { ";
    for(auto i : Sel) {
        cout << i << ", ";
    }
    cout << "}" << endl;

    return 0;
}
