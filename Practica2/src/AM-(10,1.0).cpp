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




            generacion = 0;
        }
    }

    return poblacionActual.front().fitness;
} 