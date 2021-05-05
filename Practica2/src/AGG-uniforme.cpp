/**
 * @file AGG-uniforme.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-05-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>

using namespace std;


double AGG_uniforme(list<Individuo> poblacionIni, const MatDouble* distancias, int numGenesFactible, float probabilidadCruce, float probabilidadMutacion) {

    int iteraciones = 0;
    //int aux;

    list<Individuo> poblacionActual = poblacionIni;

    Individuo mejorPadre;

    /*
    for(Individuo i : poblacionActual) {
        int num = 0;
        for(int j = 0; j < i.genes.size(); ++j) {
            cout << i.genes[j];
            if(i.genes[j] == 1) {
                ++num;
            } 
            cout << " ";
        }
        cout << endl;
        cout << num;
        cout << endl;
    }
    cout << poblacionActual.front().fitness;
    cout << endl;
    cout << endl;
    cin >> aux;
    */

    while(iteraciones < 100000) {
        // Mecanismo de selección
        mejorPadre = poblacionActual.front();
        operador_seleccion(&poblacionActual);

        /*
        for(Individuo i : poblacionActual) {
            int num = 0;
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                if(i.genes[j] == 1) {
                    ++num;
                } 
                cout << " ";
            }
            cout << endl;
            cout << num;
            cout << endl;
        }
        cout << poblacionActual.front().fitness;
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        // Operador de cruce
        operador_cruce_uniforme(&poblacionActual, probabilidadCruce, numGenesFactible, distancias);

        /*
        for(Individuo i : poblacionActual) {
            int num = 0;
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                if(i.genes[j] == 1) {
                    ++num;
                } 
                cout << " ";
            }
            cout << endl;
            cout << num;
            cout << endl;
        }
        cout << poblacionActual.front().fitness;
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        // Operador de mutación
        operadorMutacion(&poblacionActual, probabilidadMutacion);

        /*
        for(Individuo i : poblacionActual) {
            int num = 0;
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                if(i.genes[j] == 1) {
                    ++num;
                } 
                cout << " ";
            }
            cout << endl;
            cout << num;
            cout << endl;
        }
        cout << poblacionActual.front().fitness;
        cout << endl;
        cout << endl;
        cin >> aux;
        */


        // Mecanismo de reemplazo
        int incre_iter = calcularFitness(&poblacionActual, distancias);

        /*
        for(Individuo i : poblacionActual) {
            int num = 0;
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                if(i.genes[j] == 1) {
                    ++num;
                } 
                cout << " ";
            }
            cout << endl;
            cout << num;
            cout << endl;
        }
        cout << poblacionActual.front().fitness;
        cout << endl;
        cout << endl;
        cin >> aux;
        */


        operador_reemplazo(&poblacionActual, &mejorPadre);

        /*
        for(Individuo i : poblacionActual) {
            int num = 0;
            for(int j = 0; j < i.genes.size(); ++j) {
                cout << i.genes[j];
                if(i.genes[j] == 1) {
                    ++num;
                } 
                cout << " ";
            }
            cout << endl;
            cout << num;
            cout << endl;
        }
        cout << poblacionActual.front().fitness;
        cout << endl;
        cout << endl;
        cin >> aux;
        */

        
        iteraciones += incre_iter;

        /*
        cout << poblacionActual.front().fitness;
        cout << "----------------------------------------------------";
        cout << endl;
        */
        //cin >> aux;

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

    /*
    int aux;
    for(Individuo i : poblacionIni) {
        int num = 0;
        for(int j = 0; j < i.genes.size(); ++j) {
            cout << i.genes[j];
            if(i.genes[j] == 1) {
                ++num;
            } 
            cout << " ";
        }
        cout << endl;
        cout << num;
        cout << endl;
    }
    cout << endl;
    cout << endl;
    cin >> aux;
    */

    start_timers();

    double fitness = AGG_uniforme(poblacionIni, &distancias, numElemSelec, probabilidadCruce, probabilidadMutacion);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}