/**
 * @file BMB.cpp
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

double BMB(ListInt *Solucion, const MatDouble *distancias, int m) {
    int iteracionesMax = 10;
    int iteraciones = 0;
    ListInt mejorSolucion;
    double mejorFitness = 0;

    int iter_fin = 100000 / iteracionesMax;

    while(iteraciones != iteracionesMax) {

        ListInt solucionAleatoria;
        VecInt noSeleccionadosAleatoria;

        generarSolAleatoria(&solucionAleatoria, &noSeleccionadosAleatoria, m, distancias->size()+1);

        int iter_ini = 0;
        double nuevo_fitness = busquedaLocal_PM(&solucionAleatoria, &noSeleccionadosAleatoria, distancias, &iter_ini, iter_fin);

        if(nuevo_fitness > mejorFitness) {
            mejorFitness = nuevo_fitness;
            mejorSolucion = solucionAleatoria;
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
    ListInt SoluIni;

    start_timers();

    double fitness = BMB(&SoluIni, &distancias, numElemSelec);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
