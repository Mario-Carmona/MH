/**
 * @file main.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <string>
#include <armadillo>
#include <random.h>

using namespace std;
using namespace arma;


/**
 * @brief 
 * 
 * @param Sel 
 * @param i 
 * @param j 
 */
void Int(uvec* Sel, int i, int j, const field<rowvec>* distancias, vec *contri) {

    int pos_i = 0;
    int contri_j = 0;

    for(int aux = 0; aux < Sel->size(); aux++) {
        if((*Sel)[aux] == i) {
            pos_i = aux;
        }
        else{
            contri[aux] -= ((*Sel)[aux] > i) ? (*distancias)[i][(*Sel)[aux]-i] 
                                             : (*distancias)[(*Sel)[aux]][i-(*Sel)[aux]];
            int dist = ((*Sel)[aux] > j) ? (*distancias)[j][(*Sel)[aux]-j] 
                                             : (*distancias)[(*Sel)[aux]][j-(*Sel)[aux]];
            contri[aux] += dist;
            contri_j += dist;                      
        }
    }

    Sel->replace(i, j);

    contri[pos_i] = contri_j;
}

int funcion_obj(const uvec* Sel, const field<rowvec>* distancias, vec *contri) {
    int total = 0;

    for(int i = 0; i < Sel->size()-1; i++) {
        for(int j = i+1; j < Sel->size(); j++) {
            int dist = ((*Sel)[i] < (*Sel)[j]) ? (*distancias)[(*Sel)[i]][(*Sel)[j]-(*Sel)[i]] 
                                               : (*distancias)[(*Sel)[j]][(*Sel)[i]-(*Sel)[j]];
            total += dist;
            contri[i] += dist;
            contri[j] += dist;
        }
    }

    return total;
}

bool funcion_obj_facto(const uvec* Sel, const field<rowvec>* distancias, int i, int j, int* coste_actual) {

    int nuevo_coste = (*coste_actual);

    for(int aux : (*Sel)) {
        if(aux != i){
            nuevo_coste -= (aux > i) ? (*distancias)[i][aux-i] 
                                     : (*distancias)[aux][i-aux];
            nuevo_coste += (aux > j) ? (*distancias)[j][aux-j] 
                                     : (*distancias)[aux][j-aux];
        }
    }

    if(nuevo_coste > (*coste_actual)){
        (*coste_actual) = nuevo_coste;
        return true;
    }
    else{
        return false;
    }
}


void busquedaLocal(uvec* Sel, const field<rowvec>* distancias) {

    bool terminado = false;
    int iteraciones = 0;
    vec contri = zeros(Sel->size());
    int minContri;

    vec noSeleccionados = regspace(0, distancias->size());
    for(int i : (*Sel)) {
        noSeleccionados.shed_rows((*Sel));
    }

    int coste_actual = funcion_obj(Sel, distancias, &contri);

    while(iteraciones != 100000 && !terminado) {
        
        minContri = contri.index_min();

        shuffle(noSeleccionados);

        bool mejorado = false;
        terminado = true;
        int j;

        for(j = 0 ; j < noSeleccionados.size() && !mejorado; j++) {
            
            mejorado = funcion_obj_facto(Sel, distancias, minContri, j, &coste_actual);

            iteraciones++;
        }

        if(mejorado) {
            Int(Sel, minContri, j, distancias, &contri);
            iteraciones++;

            terminado = false;
        }
    }
}

/**
 * @brief 
 * 
 * @param nombre 
 * @param numElemSelec 
 * @return field<rowvec> 
 */
field<rowvec> leerArchivo(const char* nombre, int &numElemSelec) {
    string cadena;
    ifstream archivo(nombre);
    if(archivo.is_open()) {
        getline(archivo, cadena, ' ');
        int numElecTotal = stoi(cadena);

        getline(archivo, cadena);
        numElemSelec = stoi(cadena);

        // Creación de la matriz triangular superior
        field<rowvec> distancias(numElecTotal-1);
        for(int i = 0; i < distancias.size(); i++) {
            distancias[i] = rowvec(distancias.size()-i);
        }

        int fil,col;
        double dist;

        while(!archivo.eof()) {
            getline(archivo, cadena, ' ');
            fil = stoi(cadena);
            getline(archivo, cadena, ' ');
            col = stoi(cadena);
            getline(archivo, cadena);
            dist = stod(cadena);

            distancias[fil][col-fil] = dist;
        }

        return distancias;
    }
    else {
        cerr << "Error: Apertura del archivo" << endl;
    }
}


int main(int argc, char* argv[]) {

    // Semilla
    int seed = atoi(argv[1]);

    char* fichero = argv[2];

    int numElemSelec;

    field<rowvec> distancias = leerArchivo(fichero, numElemSelec);

    arma_rng::set_seed(seed);
    Set_random(seed);

    uvec Sel = randperm(distancias.size()+1, numElemSelec);


}
