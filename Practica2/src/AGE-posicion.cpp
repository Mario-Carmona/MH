/**
 * @file AGE-posicion.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

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
    int tamNuevaPoblacion = 2;
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
        for(int i = 0; i < tamNuevaPoblacion; ++i) {
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
        int numParejasCruce = probabilidadCruce * (nuevaPoblacion.size() / 2);

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
        int numIndiMutacion = probabilidadMutacion * nuevaPoblacion.size();
        int numGenes = nuevaPoblacion.front().genes.size();
        int numGenesMutacion = probabilidadMutacion * numGenes;

        VecInt posGenes;
        for(int i = 0; i < numGenesMutacion; ++i) {
            posGenes.push_back(rand() % numGenes);
        }

        for(int i = 0; i < numIndiMutacion; ++i) {
            nuevaPoblacion.push_back(operadorMutacion(&nuevaPoblacion.front(), &posGenes));
            nuevaPoblacion.pop_front();
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

        // Mecanismo de reemplazo
        calcularFitness(&nuevaPoblacion, distancias);
        
        for(int i = 0; i < tamNuevaPoblacion; ++i) {
            nuevaPoblacion.push_back(poblacionActual.back());
            poblacionActual.pop_back();
        }

        nuevaPoblacion.sort(compare_mayorFitness);

        for(int i = 0; i < tamNuevaPoblacion; ++i) {
            poblacionActual.push_back(nuevaPoblacion.front());
            nuevaPoblacion.pop_front();
        }

        poblacionActual.sort(compare_mayorFitness);

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

        iteraciones += nuevaPoblacion.size();
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
    float probabilidadCruce = 1.0;
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