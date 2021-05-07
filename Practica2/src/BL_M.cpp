/**
 * @file BL_M.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>


using namespace std;


/**
 * @brief Función que calcula la diversidad de una solución obtenida
 *        mediante el algoritmo de Búsqueda Local del Mejor.
 * 
 * @param Solucion Solución inicial 
 * @param noSeleccionados Lista de elementos no seleccionados para la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Diversidad de la solución final
 */
double busquedaLocal_M(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias) {

    bool terminado = false;
    int iteraciones = 0;
    
    // Se obtiene la diversidad de la solución inicial, y sus contribuciones
    double coste_actual = funcion_obj(Solucion, distancias);

    while(iteraciones != 100000 && !terminado) {
        
        pair<bool,VecIntIt> mejorado (false,noSeleccionados->begin());
        terminado = true;
        VecIntIt mejor_elemAIncluir;
        ListIntIt mejor_elemASustituir;
        double mejor_coste = 0.0;
        double new_coste;

        // Se obtiene el elemento de los no seleccionados que mejora en mayor
        // medida la solución actual al ser sustituido por el elemento que menos
        // contribuye a la solución actual
        for(auto elemAIncluir = noSeleccionados->begin(); elemAIncluir != noSeleccionados->end(); ++elemAIncluir) {
            for(auto elemASustituir = Solucion->begin(); elemASustituir != Solucion->end(); ++elemASustituir) {
                // Calcular el coste de la solución si se intercambia uno de sus elementos
                mejorado = funcion_obj_facto(Solucion, distancias, elemASustituir, elemAIncluir, coste_actual, &new_coste);

                // Se va guardando el mejor elemento que mejora la solución actual
                if(mejorado.first) {
                    if(new_coste > mejor_coste) {
                        mejor_elemAIncluir = elemAIncluir;
                        mejor_elemASustituir = elemASustituir;
                        mejor_coste = new_coste;
                    }
                }

                ++iteraciones;
                if(iteraciones >= 100000) {
                    break;
                }
            }
            if(iteraciones >= 100000) {
                break;
            }
        }

        if(mejor_coste != 0.0) {
            // Se actualiza el coste de la solución
            coste_actual = mejor_coste;

            // Se realiza el intercambio de elementos y se calculan sus contribuciones
            Int(Solucion, mejor_elemASustituir, noSeleccionados, mejor_elemAIncluir, distancias);

            // Se indica que no debe terminar el proceso, al haber mejora la
            // antigua solución actual
            terminado = false;
        }
    }

    return coste_actual;
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

    double fitness = busquedaLocal_M(&SoluIni, &noSeleccionados, &distancias);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Fitness: " << fitness << endl;

    return 0;
}
