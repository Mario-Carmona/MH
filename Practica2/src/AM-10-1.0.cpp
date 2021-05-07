/**
 * @file AM-(10,1.0).cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-05-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>

using namespace std;


double AM_10_1_0(list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion) {
    int iteraciones = 0;
    int generacion = 0;

    list<Individuo> poblacionActual = poblacionIni;

    Individuo mejorPadre;

    while(iteraciones < 100000) {
        // Mecanismo de selección
        mejorPadre = poblacionActual.front();
        operador_seleccion(&poblacionActual);

        // Operador de cruce
        operador_cruce_uniforme(&poblacionActual, probabilidadCruce, numGenesFactible, distancias);

        // Operador de mutación
        operadorMutacion(&poblacionActual, probabilidadMutacion);

        // Mecanismo de reemplazo
        int incre_iter = calcularFitness(&poblacionActual, distancias);
        operador_reemplazo(&poblacionActual, &mejorPadre);

        iteraciones += incre_iter;
        ++generacion;

        if(generacion == 10) {
            int intensidad_BL = 400;

            for(auto it = poblacionActual.begin(); it != poblacionActual.end(); ++it) {
                int iter_max = iteraciones + intensidad_BL;
                if(iter_max > 100000) {
                    iter_max = 100000;
                }

                ListInt seleccionados;
                VecInt noSeleccionados;
                convertirSolucion(it, &seleccionados, &noSeleccionados);
                busquedaLocal_PM(&seleccionados, &noSeleccionados, distancias, &iteraciones, iter_max);
                recuperarSolucion(it, &seleccionados);

                if(iteraciones >= 100000) {
                    break;
                }
            }

            generacion = 0;
        }
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

    double fitness = AM_10_1_0(poblacionIni, &distancias, numElemSelec, probabilidadCruce, probabilidadMutacion);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}