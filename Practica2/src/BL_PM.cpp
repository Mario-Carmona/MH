/**
 * @file BL_PM.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>


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
    ListInt SoluIni;
    while(SoluIni.size() < numElemSelec) {
        int nuevo = rand() % (distancias.size()+1);
        SoluIni.push_back(nuevo);
        SoluIni.sort();
        SoluIni.unique();
    }
    
    VecInt noSeleccionados;
    auto it = SoluIni.cbegin();
    while(it != SoluIni.cend()) {
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

    double fitness = busquedaLocal_PM(&SoluIni, &noSeleccionados, &distancias, &iteraciones);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
