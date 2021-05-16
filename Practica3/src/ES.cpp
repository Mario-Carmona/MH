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

    double fitness = ES(&SoluIni, &noSeleccionados, &distancias, &iteraciones);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
