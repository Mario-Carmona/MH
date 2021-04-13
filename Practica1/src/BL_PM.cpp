/**
 * @file BL_PM.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>
#include <timer.h>


using namespace std;


/**
 * @brief Función que calcula la diversidad de una solución obtenida
 *        mediante el algoritmo de Búsqueda Local del Primero Mejor.
 * 
 * @param Solucion Solución inicial
 * @param noSeleccionados Lista de elementos no seleccionados para la solución
 * @param distancias Matriz de distancias entre cada par de elementos
 * @return double Diversidad de la solución final
 */
double busquedaLocal_PM(SetInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias) {

    bool terminado = false;
    int iteraciones = 0;

    // Inicialización del vector de contribuciones, que indica cuánto
    // contribuye cada elementos al total de la diversidad de una solución
    MapDouble contri;
    for(auto i : *Solucion) {
        contri[i] = 0.0;
    }

    // Se obtiene la diversidad de la solución inicial, y sus contribuciones
    double coste_actual = funcion_obj(Solucion, distancias, &contri);

    // Se calcula el elemento con menor contribución en la solución inicial
    MapDoubleIt minContri = contri.begin();
    for(auto i = ++contri.begin(); i != contri.end(); ++i) {
        if(*i < *minContri) {
            minContri = i;
        }
    }
    SetIntIt minContriElem = Solucion->find(minContri->first);

    while(iteraciones != 100000 && !terminado) {
        
        pair<bool,VecIntIt> mejorado (false,noSeleccionados->begin());
        terminado = true;
        VecIntIt elemAIncluir;

        // Se baraja la lista de elementos no seleccionado, para hacer que
        // la elección del primer elemento que mejore la solución, sea de forma
        // aleatoria
        shuffle(noSeleccionados);

        // Obtener un elemento en la lista de no seleccionado que mejore la
        // solución actual
        for(elemAIncluir = noSeleccionados->begin(); elemAIncluir != noSeleccionados->end() && !mejorado.first; ++elemAIncluir) {
            
            mejorado = funcion_obj_facto(Solucion, distancias, minContriElem, elemAIncluir, &coste_actual);
        }

        // Si se encuentra ese elemento, 
        if(mejorado.first) {
            // Se intercambia por el elementos con menor contribución, y se calculan 
            // las contribuciones de la nueva solución actual
            Int(Solucion, minContriElem, noSeleccionados, mejorado.second, distancias, &contri);

            // Se calcula el elemento con menor contribución en la solución actual
            minContri = contri.begin();
            for(auto i = ++contri.begin(); i != contri.end(); ++i) {
                if(*i < *minContri) {
                    minContri = i;
                }
            }
            minContriElem = Solucion->find(minContri->first);

            // Se indica que no debe terminar el proceso, al haber mejora la
            // antigua solución actual
            terminado = false;
        }

        iteraciones++;
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
    SetInt SoluIni;
    while((int)SoluIni.size() < numElemSelec) {
        int aux = rand() % (distancias.size()+1);
        SoluIni.insert(aux);
    }
    
    VecInt noSeleccionados;
    for(int i = 0; i < (int)distancias.size()+1; ++i) {
        if(SoluIni.find(i) == SoluIni.end()) {
            noSeleccionados.push_back(i);
        }
    }

    start_timers();

    double coste_final = busquedaLocal_PM(&SoluIni, &noSeleccionados, &distancias);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Coste final: " << coste_final << endl;

    return 0;
}
