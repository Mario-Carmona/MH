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


/**
 * @brief Función que calcula la diversidad de una solución obtenida
 *        mediante el algoritmo Greedy.
 * 
 * @param Solucion Solución inicial
 * @param noSeleccionados Lista de elementos no seleccionados para la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @param numElemSelec Número de elementos que debe tener la solucións
 * @return double Diversidad de la solución final
 */
double Greedy(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int numElemSelec) {

    // Se elige el elemento no seleccionado que tenga una mayor distancia acumulada
    VecIntIt elegido = calcularDistAcu(noSeleccionados, distancias);

    Solucion->push_back(*elegido);
    noSeleccionados->erase(elegido);

    while(Solucion->size() < numElemSelec) {
        // Se elige el elemento que tenga la máxima distancia de entre las
        // distancias mínimas entre los elementos de la solución y los
        // elementos no seleccionados
        elegido = elegirSig(Solucion, noSeleccionados, distancias);

        Solucion->push_back(*elegido);
        noSeleccionados->erase(elegido);
    }

    return funcion_obj(Solucion, distancias);
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

    ListInt SoluIni;
    VecInt noSeleccionados;

    for(int i = 0; i < (int)distancias.size()+1; ++i) {
        noSeleccionados.push_back(i);
    }

    start_timers();

    double fitness = Greedy(&SoluIni, &noSeleccionados, &distancias, numElemSelec);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
