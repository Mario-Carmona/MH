/**
 * @file AGG-posicion.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>

using namespace std;


double AGG_posicion(list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion) {

    int iteraciones = 0;

    // También usaré esta población como nueva población, extrayendo
    // antes el mejor individuo
    list<Individuo> poblacionActual = poblacionIni;

    Individuo mejorPadre;

    while(iteraciones < 100000) {
        // Mecanismo de selección, en este modelo se realiza la selección sobre
        // la propia población actual, dado que va a ser del mismo tamaño

        // Se guarda el mejor padre de la población actual, antes de realizar la selección
        mejorPadre = poblacionActual.front();
        operador_seleccion(&poblacionActual);

        // Operador de cruce
        operador_cruce_posicion(&poblacionActual, probabilidadCruce);

        // Operador de mutación
        operadorMutacion(&poblacionActual, probabilidadMutacion);

        // Cálculo del fitness de la población
        int incre_iter = calcularFitness(&poblacionActual, distancias);
        // Mecanismo de reemplazo
        operador_reemplazo(&poblacionActual, &mejorPadre);

        iteraciones += incre_iter;
    }

    return poblacionActual.front().fitness;
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

    int numCromosomas = 50;
    float probabilidadCruce = 0.7;
    float probabilidadMutacion = 0.1/(distancias.size()+1);

    list<Individuo> poblacionIni;
    for(int i = 0; i < numCromosomas; ++i) {
        poblacionIni.push_back(Individuo(distancias.size()+1, numElemSelec, &distancias));
    }

    poblacionIni.sort(compare_mayorFitness);

    start_timers();

    double fitness = AGG_posicion(poblacionIni, &distancias, numElemSelec, probabilidadCruce, probabilidadMutacion);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}