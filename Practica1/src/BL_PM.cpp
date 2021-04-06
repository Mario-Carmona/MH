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


double busquedaLocal_PM(SetInt *Sel, VecInt *noSeleccionados, const MatDouble *distancias) {

    bool terminado = false;
    int iteraciones = 0;
    VecDouble contri(Sel->size());
    VecDoubleIt minContri = contri.begin();
    SetIntIt minContriIt = Sel->begin();
    SetIntIt it = ++Sel->begin();

    double coste_actual = funcion_obj(Sel, distancias, &contri);

    for(auto i = ++contri.begin(); i != contri.end(); ++i, ++it) {
        if(*i < *minContri) {
            minContri = i;
            minContriIt = it;
        }
    }

    while(iteraciones != 100000 && !terminado) {
        
        bool mejorado = false;
        terminado = true;
        VecIntIt j;

        shuffle(noSeleccionados);

        for(j = noSeleccionados->begin(); j != noSeleccionados->end() && !mejorado; ++j) {
            
            mejorado = funcion_obj_facto(Sel, distancias, minContriIt, j, &coste_actual);
        }

        if(mejorado) {
            // Se va para atras porque al comprobar la condicción del for se le hace un ++
            --j;
            Int(Sel, &minContriIt, &minContri, noSeleccionados, j, distancias, &contri);

            terminado = false;
        }

        iteraciones++;
    }

    return coste_actual;
}


int main(int argc, char* argv[]) {

    // Semilla
    int seed = atoi(argv[1]);

    char* fichero = argv[2];

    int numElemSelec;

    MatDouble distancias;

    leerArchivo(fichero, numElemSelec, distancias);

    srand(seed);

    SetInt Sel;
    while((int)Sel.size() < numElemSelec) {
        int aux = rand() % (distancias.size()+1);
        Sel.insert(Sel.cend(), aux);
    }
    
    VecInt noSeleccionados;
    for(int i = 0; i < (int)distancias.size()+1; ++i) {
        if(Sel.find(i) == Sel.end()) {
            noSeleccionados.push_back(i);
        }
    }

    start_timers();

    double coste_final = busquedaLocal_PM(&Sel, &noSeleccionados, &distancias);

    auto tiempo_ejec = elapsed_time();

    cout << "Tiempo de ejecución: " << tiempo_ejec << endl;
    cout << "Coste final: " << coste_final << endl;

    return 0;
}
