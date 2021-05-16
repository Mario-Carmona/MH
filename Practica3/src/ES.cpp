/**
 * @file ES.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-05-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>
#include <cmath>


using namespace std;

double ES(VecInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias) {
    // Valores de los parámetros
    double fi = 0.3;
    double mu = 0.3;
    double temp_final = 0.001;

    double coste_actual = funcion_obj(Solucion, distancias);
    double mejorSolucion = coste_actual;

    // Cálculo de la temperatura inicial
    double temp_ini = (mu*coste_actual) / (-log(fi));
    double temp = temp_ini;
    
    int evaluaciones = 0;
    int iteraciones = 0;
    int numExitos = 1;
    int numVecGenerados;

    int m = Solucion->size();
    int numMaxVecGenerados = 10*m;
    int numMaxExitos = m;

    while(evaluaciones != 100000 && numExitos > 0) {
        numExitos = 0;
        numVecGenerados = 0;

        while(numExitos != numMaxExitos && numVecGenerados != numMaxVecGenerados) {
            // Generar vecino
            int elemASustituir = rand() % Solucion->size();
            int elemAIncluir = rand() % noSeleccionados->size();

            // Se comprueba si se sustituye la solución acutal
            // por el nuevo vecino
            double coste_vecino = funcion_obj_facto(Solucion, distancias, (*Solucion)[elemASustituir], (*noSeleccionados)[elemAIncluir], coste_actual);
            ++evaluaciones;
            ++numVecGenerados;

            double diferencia = coste_actual - coste_vecino;
            if(diferencia < 0) {
                Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                coste_actual = coste_vecino;

                if(coste_actual > mejorSolucion) {
                    mejorSolucion = coste_actual;
                }

                ++numExitos;
            }
            else {
                double probabilidad = rand() / (RAND_MAX + 1.);
                int k = 1;
                if(probabilidad <= exp( (-diferencia) / (k*temp) )) {
                    Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                    coste_actual = coste_vecino;

                    ++numExitos;
                }
            }

            if(evaluaciones == 100000) {
                break;
            }
        }
        ++iteraciones;

        // Enfriamiento
        temp = enfriarTemperatura(temp, temp_ini, temp_final, iteraciones);
    }

    return mejorSolucion;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        cout << "Nº de argumentos incorrecto" << endl;
        return -1;
    }

    // Semilla
    int seed = atoi(argv[1]);

    // Fichero con los datos de entrada
    char* fichero = argv[2];

    int numElemSelec;
    MatDouble distancias;

    // Obtenemos los datos de entrada
    leerArchivo(fichero, numElemSelec, distancias);

    srand(seed);

    // Creación de la solución inicial de forma aleatoria
    ListInt SoluIniList;
    while(SoluIniList.size() < numElemSelec) {
        int nuevo = rand() % (distancias.size()+1);
        SoluIniList.push_back(nuevo);
        SoluIniList.sort();
        SoluIniList.unique();
    }

    VecInt SoluIni;
    for(auto i : SoluIniList) {
        SoluIni.push_back(i);
    }
    
    VecInt noSeleccionados;
    auto it = SoluIniList.cbegin();
    while(it != SoluIniList.cend()) {
        for(int i = 0; i < (int)distancias.size()+1; ++i) {
            if(*it == i) {
                ++it;
            }
            else {
                noSeleccionados.push_back(i);
            }
        }
    }

    start_timers();

    int iteraciones = 0;

    double fitness = ES(&SoluIni, &noSeleccionados, &distancias);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
