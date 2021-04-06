/**
 * @file Greedy.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <cmath>
#include <funciones_aux.h>
#include <timer.h>


using namespace std;


SetIntIt elegirSig(SetInt* Sel, SetInt* noSeleccionados, const MatDouble* distancias) {
    VecDouble distMin(Sel->size(), MAXFLOAT);
    vector<SetIntIt> distMinIt(Sel->size());
    int contador_i = 0;

    for(auto i = Sel->begin(); i != Sel->end(); ++i, ++contador_i) {
        for(auto j = noSeleccionados->begin(); j != noSeleccionados->end(); ++j) {
            double dist;
            if(*i < *j) {
                dist = (*distancias)[*i][*j-*i-1];
            }
            else {
                dist = (*distancias)[*j][*i-*j-1];
            }

            if(distMin[contador_i] > dist){
                distMin[contador_i] = dist;
                distMinIt[contador_i] = j;
            }
        }
    }

    double max_dist = -1;
    SetIntIt it = noSeleccionados->end();
    contador_i = 0;

    for(auto i = Sel->begin(); i != Sel->end(); ++i, ++contador_i){
        if(max_dist < distMin[contador_i]) {
            max_dist = distMin[contador_i];
            it = distMinIt[contador_i];
        }
    }

    return it;
}

SetIntIt calcularDistAcu(SetInt* noSeleccionados, const MatDouble* distancias) {
    VecDouble distAcu(noSeleccionados->size(), 0.0);
    int contador_i = 0;
    
    for(auto i = noSeleccionados->begin(); i != --noSeleccionados->end(); ++i, ++contador_i) {
        int contador_j = contador_i+1;
        auto j = i;
        ++j;
        for(; j != noSeleccionados->end(); ++j, ++contador_j) {
            double dist;
            if(*i < *j) {
                dist += (*distancias)[*i][*j-*i-1];
            }
            else {
                dist += (*distancias)[*j][*i-*j-1];
            }

            distAcu[contador_i] += dist;
            distAcu[contador_j] += dist;
        }
    }

    double max_dist = 0.0;
    SetIntIt it = noSeleccionados->end();
    contador_i = 0;

    for(auto i = noSeleccionados->begin(); i != noSeleccionados->end(); ++i, ++contador_i){
        if(max_dist < distAcu[contador_i]) {
            max_dist = distAcu[contador_i];
            it = i;
        }
    } 

    return it;
}


double Greedy(SetInt *Sel, SetInt *noSeleccionados, const MatDouble *distancias, int numElemSelec) {

    VecDouble distAcu;

    SetIntIt elegido = calcularDistAcu(noSeleccionados, distancias);
    
    Sel->insert(Sel->end(), *elegido);
    noSeleccionados->erase(elegido);

    while(Sel->size() < numElemSelec) {
        elegido = elegirSig(Sel, noSeleccionados, distancias);

        Sel->insert(Sel->end(), *elegido);
        noSeleccionados->erase(elegido);
    }

    return funcion_obj(Sel, distancias);
}


int main(int argc, char* argv[]) {

    // Semilla
    int seed = atoi(argv[1]);

    char* fichero = argv[2];

    int numElemSelec;

    MatDouble distancias;

    leerArchivo(fichero, numElemSelec, distancias);

    SetInt Sel, noSeleccionados;

    for(int i = 0; i < (int)distancias.size()+1; ++i) {
        noSeleccionados.insert(noSeleccionados.end(), i);
    }

    start_timers();

    double coste_final = Greedy(&Sel, &noSeleccionados, &distancias, numElemSelec);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Coste final: " << coste_final << endl;

    return 0;
}
