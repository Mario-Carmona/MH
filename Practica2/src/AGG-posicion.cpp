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
    //int aux;

    list<Individuo> poblacionActual = poblacionIni;

    /*
    for(Individuo i : poblacionActual) {
        for(int j = 0; j < i.genes.size(); ++j) {
            cout << i.genes[j];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
    cin >> aux;
    */

    while(iteraciones < 100000) {
        // Mecanismo de selección
        list<Individuo> nuevaPoblacion;
        while(nuevaPoblacion.size() != poblacionActual.size()) {
            Individuo seleccionado = seleccionTorneo(&poblacionActual, 2);
            nuevaPoblacion.push_back(seleccionado);
        }

        /*
        for(Individuo i : nuevaPoblacion) {
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        // Operador de cruce
        int numParejasCruce = probabilidadCruce * (poblacionIni.size() / 2);

        for(int i = 0; i < numParejasCruce; ++i) {
            Individuo padre1 = nuevaPoblacion.front();
            nuevaPoblacion.pop_front();
            Individuo padre2 = nuevaPoblacion.front();
            nuevaPoblacion.pop_front();
            
            Individuo hijo;
            hijo = cruce_posicion(&padre1, &padre2);
            nuevaPoblacion.push_back(hijo);
            hijo = cruce_posicion(&padre1, &padre2);
            nuevaPoblacion.push_back(hijo);
        }

        /*
        for(Individuo i : nuevaPoblacion) {
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        // Operador de mutación
        operadorMutacion(&nuevaPoblacion, probabilidadMutacion);

        /*
        for(Individuo i : nuevaPoblacion) {
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        // Mecanismo de reemplazo
        int incre_iter = calcularFitness(&nuevaPoblacion, distancias);

        int maximoFitnessActual = poblacionActual.front().fitness;
        nuevaPoblacion.sort(compare_mayorFitness);
        
        bool encontrado = false;

        for(Individuo i : nuevaPoblacion) {
            if(i.fitness < maximoFitnessActual) {
                break;
            }
            else if(i.fitness == maximoFitnessActual) {
                if(poblacionActual.front() == i) {
                    encontrado = true;
                    break;
                }
            }
        }

        if(!encontrado) {
            nuevaPoblacion.pop_back();
            nuevaPoblacion.push_back(poblacionActual.front());
        }

        poblacionActual = nuevaPoblacion;

        /*
        for(Individuo i : nuevaPoblacion) {
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                cout << " ";
            }
            cout << endl;
        }
        cout << endl;
        cout << endl;
        cin >> aux;
        */

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
    float probabilidadMutacion = 0.1;

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