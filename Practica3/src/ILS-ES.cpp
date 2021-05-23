/**
 * @file ILS-ES.cpp
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


using namespace std;

double ILS_ES(VecInt *Solucion, const MatDouble *distancias, int m) {
    int iteracionesMax = 10;
    int iteraciones = 0;
    double mejorFitness;

    int t = 0.1 * m;

    int iter_fin = 100000 / iteracionesMax;

    VecInt mejorSolucion;
    VecInt noSeleccionados;
    double fitnessActual;

    generarSolAleatoria(&mejorSolucion, &noSeleccionados, m, distancias->size()+1);

    int iter_ini = 0;
    mejorFitness = ES(&mejorSolucion, &noSeleccionados, distancias, &iter_ini, iter_fin);

    ++iteraciones;

    while(iteraciones != iteracionesMax) {
        VecInt nuevaSolucion = mejorSolucion;
        VecInt nuevoNoSeleccionados = noSeleccionados;

        operador_mutacion_ILS(&nuevaSolucion, &nuevoNoSeleccionados, distancias, t);

        iter_ini = 0;
        double nuevo_fitness = ES(&nuevaSolucion, &nuevoNoSeleccionados, distancias, &iter_ini, iter_fin);

        if(nuevo_fitness > mejorFitness) {
            mejorFitness = nuevo_fitness;
            mejorSolucion = nuevaSolucion;
            noSeleccionados = nuevoNoSeleccionados;
        }

        ++iteraciones;
    }

    Solucion->clear();
    for(auto i : mejorSolucion) {
        Solucion->push_back(i);
    }

    return mejorFitness;
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
    VecInt SoluIni;

    start_timers();

    double fitness = ILS_ES(&SoluIni, &distancias, numElemSelec);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
