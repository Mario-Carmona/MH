/**
 * @file funciones_aux.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief 
 * @version 0.1
 * @date 2021-03-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <funciones_aux.h>

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
double busquedaLocal_PM(ListInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iteraciones, int iter_max) {

    bool terminado = false;

    // Inicialización del vector de contribuciones, que indica cuánto
    // contribuye cada elementos al total de la diversidad de una solución
    list<pair<int,double>> solucion_contri;
    for(auto i : *Solucion) {
        solucion_contri.push_back(pair<int,double>(i,0.0));
    }

    // Se obtiene la diversidad de la solución inicial, y sus contribuciones
    double coste_actual = funcion_obj(&solucion_contri, distancias);

    // Se calcula el elemento con menor contribución en la solución inicial
    solucion_contri.sort(compare_menorContri);

    while(*iteraciones < iter_max && !terminado) {
        
        pair<bool,VecIntIt> mejorado (false,noSeleccionados->begin());
        terminado = true;

        // Se baraja la lista de elementos no seleccionado, para hacer que
        // la elección del primer elemento que mejore la solución, sea de forma
        // aleatoria
        shuffle(noSeleccionados);

        // Obtener un elemento en la lista de no seleccionado que mejore la
        // solución actual
        auto minContriElem = solucion_contri.cbegin();
        for(; minContriElem != solucion_contri.cend(); ++minContriElem) {
            for(auto elemAIncluir = noSeleccionados->begin(); elemAIncluir != noSeleccionados->end() && !mejorado.first; ++elemAIncluir) {
                
                mejorado = funcion_obj_facto(&solucion_contri, distancias, minContriElem->first, elemAIncluir, &coste_actual);

                (*iteraciones) += 1;
                if(mejorado.first || *iteraciones >= iter_max) {
                    break;
                }
            }
            if(mejorado.first || *iteraciones >= iter_max) {
                break;
            }
        }

        // Si se encuentra ese elemento, 
        if(mejorado.first) {
            // Se intercambia por el elementos con menor contribución, y se calculan 
            // las contribuciones de la nueva solución actual
            Int(&solucion_contri, minContriElem, noSeleccionados, mejorado.second, distancias);

            solucion_contri.sort(compare_menorContri);

            // Se indica que no debe terminar el proceso, al haber mejora la
            // antigua solución actual
            terminado = false;
        }
    }

    Solucion->clear();
    for(auto i : solucion_contri) {
        Solucion->push_back(i.first);
    }

    return coste_actual;
}

bool compare_menorContri(const pair<int,double>& first, const pair<int,double>& second) {
    if(first.second <= second.second) {
        return true;
    }
    else {
        return false;
    }
}

void shuffle(VecInt* vector) {
    for(int i = vector->size()-1; i > 0; --i) {
        int random = rand() % (i+1);
        int a = (*vector)[random];
        int b = (*vector)[i];
        (*vector)[random] = b;
        (*vector)[i] = a;
    }
}

void Int(list<pair<int,double>>* Solucion, list<pair<int,double>>::const_iterator elemASustituir, VecInt* noSeleccionados,
         VecIntIt elemAIncluir, const MatDouble* distancias) {

    double contri_j = 0.0;

    for(auto i = Solucion->begin(); i != Solucion->end(); ++i) {
        if(i->first != elemASustituir->first) {
            // Restar las distancias del elemento a sustituir a las contribuciones
            if(i->first > elemASustituir->first) {
                i->second -= (*distancias)[elemASustituir->first][i->first-elemASustituir->first-1];
            }
            else {
                i->second -= (*distancias)[i->first][elemASustituir->first-i->first-1];
            }

            // Sumar las distancias del elemento a incluir a las contribuciones
            double dist;
            if(i->first > *elemAIncluir) {
                dist = (*distancias)[*elemAIncluir][i->first-*elemAIncluir-1];
            }
            else {
                dist = (*distancias)[i->first][*elemAIncluir-i->first-1];
            }

            i->second += dist;
            contri_j += dist;                      
        }
    }

    // Intercambiar los elementos
    int a = elemASustituir->first;
    int b = *elemAIncluir;

    noSeleccionados->erase(elemAIncluir);
    noSeleccionados->push_back(a);

    Solucion->erase(elemASustituir);
    Solucion->push_back(pair<int,double>(b,contri_j));
}

void Int(ListInt* Solucion, ListIntIt elemASustituir, VecInt* noSeleccionados,
         VecIntIt elemAIncluir, const MatDouble* distancias) {

    // Intercambiar los elementos
    int a = *elemASustituir;
    int b = *elemAIncluir;

    noSeleccionados->erase(elemAIncluir);
    noSeleccionados->push_back(a);

    Solucion->erase(elemASustituir);
    Solucion->push_back(b);
}

double funcion_obj(const ListInt* Solucion, const MatDouble* distancias) {
    double total = 0.0;

    for(auto i = Solucion->cbegin(); i != --Solucion->cend(); ++i) {
        auto j = i;
        ++j;
        for(; j != Solucion->cend(); ++j) {
            // Sumar la distancia entre los elementos que conforman la solución
            if(*i > *j) {
                total += (*distancias)[*j][*i-*j-1];
            }
            else {
                total += (*distancias)[*i][*j-*i-1];
            }
        }
    }

    return total;
}

double funcion_obj(list<pair<int,double>>* Solucion, const MatDouble* distancias) {
    double total = 0.0;

    for(auto i = Solucion->begin(); i != --Solucion->end(); ++i) {
        auto j = i;
        ++j;
        for(; j != Solucion->end(); ++j) {
            double dist;
            if(i->first > j->first) {
                dist = (*distancias)[j->first][i->first-j->first-1];
            }
            else {
                dist = (*distancias)[i->first][j->first-i->first-1];
            }

            // Sumar la distancia entre los elementos que conforman la solución
            total += dist;
            // Tener en cuenta la distancia calculada en la contribución en los
            // elementos utilizados para calcular la distancia 
            i->second += dist;
            j->second += dist;
        }
    }

    return total;
}

pair<bool,VecIntIt> funcion_obj_facto(const list<pair<int,double>>* Solucion, const MatDouble* distancias,
                       int elemSustituido, VecIntIt elemIncluido, double* coste_actual) {
    
    // Nuevo coste de la solución al intercambiar el elemento
    double nuevo_coste = (*coste_actual);

    for(auto i : (*Solucion)) {
        if(i.first != elemSustituido) {
            // Se restan las distancias al elemento a sustituir al coste de la solución
            if(i.first > elemSustituido) {
                nuevo_coste -= (*distancias)[elemSustituido][i.first-elemSustituido-1];
            }
            else {
                nuevo_coste -= (*distancias)[i.first][elemSustituido-i.first-1];
            }

            // Se suman las distancias al elemento a incluir al coste de la solución
            if(i.first > *elemIncluido) {
                nuevo_coste += (*distancias)[*elemIncluido][i.first-*elemIncluido-1];
            }
            else {
                nuevo_coste += (*distancias)[i.first][*elemIncluido-i.first-1];
            }
        }
    }

    // Si se mejora el coste actual de la solución, se actualiza el coste y se indica el 
    // elemento que posibilitó la mejora
    if(nuevo_coste > (*coste_actual)){
        (*coste_actual) = nuevo_coste;
        return pair<bool,VecIntIt> (true,elemIncluido);
    }
    else{
        return pair<bool,VecIntIt> (false,elemIncluido);
    }
}

pair<bool,VecIntIt> funcion_obj_facto(const ListInt* Solucion, const MatDouble* distancias,
                       ListIntIt elemSustituido, VecIntIt elemIncluido, double coste_actual, double* new_coste) {

    // Nuevo coste de la solución al intercambiar el elemento
    (*new_coste) = coste_actual;

    for(auto aux = Solucion->begin(); aux != Solucion->end(); ++aux) {
        if(aux != elemSustituido) {
            // Se restan las distancias al elemento a sustituir al coste de la solución
            if(*aux > *elemSustituido) {
                (*new_coste) -= (*distancias)[*elemSustituido][*aux-*elemSustituido-1];
            }
            else {
                (*new_coste) -= (*distancias)[*aux][*elemSustituido-*aux-1];
            }

            // Se suman las distancias al elemento a incluir al coste de la solución
            if(*aux > *elemIncluido) {
                (*new_coste) += (*distancias)[*elemIncluido][*aux-*elemIncluido-1];
            }
            else {
                (*new_coste) += (*distancias)[*aux][*elemIncluido-*aux-1];
            }
        }
    }

    // Si se mejora el coste actual de la solución, se indica el 
    // elemento que posibilitó la mejora y se devuelve por referencia
    // el nuevo coste
    if((*new_coste) > coste_actual){
        return pair<bool,VecIntIt> (true,elemIncluido);
    }
    else{
        return pair<bool,VecIntIt> (false,elemIncluido);
    }
}

bool compare_mayorDistAcu(const pair<VecIntIt,double>& first, const pair<VecIntIt,double>& second) {
    if(first.second >= second.second) {
        return true;
    }
    else {
        return false;
    }
}

bool compare_mayorDist(const pair<pair<ListIntIt,VecIntIt>,double>& first, const pair<pair<ListIntIt,VecIntIt>,double>& second) {
    if(first.second >= second.second) {
        return true;
    }
    else {
        return false;
    }
}

VecIntIt calcularDistAcu(VecInt* noSeleccionados, const MatDouble* distancias) {
    // Inicializar la matriz de distancia acumuladas de cada 
    // elemento no seleccionado
    list<pair<VecIntIt,double>> distAcu;
    for(auto i = noSeleccionados->begin(); i != noSeleccionados->end(); ++i) {
        distAcu.push_back(pair<VecIntIt,double>(i,0.0));
    }
    
    for(auto i = distAcu.begin(); i != --distAcu.end(); ++i) {
        auto j = i;
        ++j;
        for(; j != distAcu.end(); ++j) {
            double dist = (*distancias)[*i->first][*j->first-*i->first-1];

            // Sumar la distancia en la posición de cada elemento en el vector
            // de distancias acumuladas
            i->second += dist;
            j->second += dist;
        }
    }

    // Obtener el elemento que tiene una mayor distancia acumulada
    distAcu.sort(compare_mayorDistAcu);

    return distAcu.front().first;
}

VecIntIt elegirSig(ListInt* Solucion, VecInt* noSeleccionados, const MatDouble* distancias) {
    // Inicializar la matriz de distancia acumuladas de cada 
    // elemento no seleccionado
    list<pair<pair<ListIntIt,VecIntIt>,double>> distMin;
    for(auto i = Solucion->begin(); i != Solucion->end(); ++i) {
        distMin.push_back(pair<pair<ListIntIt,VecIntIt>,double>(pair<ListIntIt,VecIntIt>(i,nullptr),MAXFLOAT));
    }

    for(auto i = distMin.begin(); i != distMin.end(); ++i) {
        for(auto j = noSeleccionados->begin(); j != noSeleccionados->end(); ++j) {
            double dist;
            if(*i->first.first < *j) {
                dist = (*distancias)[*i->first.first][*j-*i->first.first-1];
            }
            else {
                dist = (*distancias)[*j][*i->first.first-*j-1];
            }

            // Obtención de la distancia mínima de un elemento de la solución con
            // cualquier elemento no seleccionado
            if(i->second > dist){
                i->second = dist;
                i->first.second = j;
            }
        }
    }

    // Obtención del elemento que tiene una mayor distancia mínima
    distMin.sort(compare_mayorDist);

    return distMin.front().first.second;
}

Individuo::Individuo() {
    fitness = 0.0;
    actualizado = false;
}

Individuo::Individuo(int tam, int genesActivos, const MatDouble* distancias) {
    this->genes = vector<int>(tam,0);

    int pos = tam-1;
    ListInt posiciones;

    int numGenes = 0;
    while(numGenes != genesActivos) {
        int nuevaPos = rand() % tam;
        if(this->genes[nuevaPos] == 0) {
            this->genes[nuevaPos] = 1;
            ++numGenes;
        }
    }

    this->fitness = funcion_obj_binaria(&genes, distancias);
    actualizado = true;
}

Individuo::Individuo(const Individuo* otro) {
    this->fitness = otro->fitness;
    this->genes = otro->genes;
    this->actualizado = otro->actualizado;
}

bool Individuo::operator==(const Individuo& otro) const {
    if(this->fitness == otro.fitness) {
        for(int i = 0; i < this->genes.size(); ++i) {
            if(this->genes[i] != otro.genes[i]) {
                return false;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

Individuo Individuo::cruce_uniforme(const Individuo* otro) {
    Individuo hijo(*this);
    hijo.actualizado = false;

    for(int i = 0; i < hijo.genes.size(); ++i) {
        if(this->genes[i] != otro->genes[i]) {
            int elegido = rand() % 2;
            if(elegido == 1) {
                hijo.genes[i] = otro->genes[i];
            }
        }
    }

    return hijo;
}

Individuo Individuo::cruce_posicion(const Individuo* otro) {
    Individuo hijo(*this);
    hijo.actualizado = false;

    ListInt posACambiar;
    VecInt valores;

    for(int i = 0; i < hijo.genes.size(); ++i) {
        if(this->genes[i] != otro->genes[i]) {
            posACambiar.push_back(i);
            valores.push_back(this->genes[i]);
        }
    }

    while(!valores.empty()) {
        int elegido = rand() % valores.size();
        hijo.genes[posACambiar.front()] = valores[elegido];

        posACambiar.pop_front();
        valores[elegido] = valores.back();
        valores.pop_back();
    }

    return hijo;
}

void Individuo::repair(int numGenesFactible, const MatDouble* distancias) {
    // Calcular el número de genes activos
    int numGenesActivos = 0;
    for(int i = 0; i < genes.size(); ++i) {
        if(genes[i] == 1) {
            ++numGenesActivos;
        }
    }

    int diferencia = numGenesActivos - numGenesFactible;

    if(diferencia > 0) {
        eliminarGenesActivos(diferencia, distancias);
    }
    else if(diferencia < 0) {
        aniadirGenesActivos(abs(diferencia), distancias);
    }
}

void Individuo::eliminarGenesActivos(int numElementos, const MatDouble* distancias) {
    MapDouble contribuciones;
    VecInt seleccionados;

    for(int i = 0; i < genes.size(); ++i) {
        if(genes[i] == 1) {
            seleccionados.push_back(i);
            contribuciones[i] = 0.0;
        }
    }

    for(int i = 0; i < seleccionados.size()-1; ++i) {
        for(int j = i+1; j < seleccionados.size(); ++j) {
            int fila = seleccionados[i];
            int columna = seleccionados[j];
            contribuciones[fila] += (*distancias)[fila][columna-fila-1];
            contribuciones[columna] += (*distancias)[fila][columna-fila-1];
        }
    }

    list<pair<int,double>> listaSeleccionados;
    for(int i = 0; i < seleccionados.size(); ++i) {
        listaSeleccionados.push_back(pair<int,double>(seleccionados[i],contribuciones[seleccionados[i]]));
    }

    listaSeleccionados.sort(compare_mayorContri);

    for(int i = 0; i < numElementos; ++i) {
        genes[listaSeleccionados.front().first] = 0;
        listaSeleccionados.pop_front();
    }
}

void Individuo::aniadirGenesActivos(int numElementos, const MatDouble* distancias) {
    MapDouble contribuciones;
    VecInt noSeleccionados;
    VecInt seleccionados;

    for(int i = 0; i < genes.size(); ++i) {
        if(genes[i] == 0) {
            noSeleccionados.push_back(i);
            contribuciones[i] = 0.0;
        }
        else {
            seleccionados.push_back(i);
        }
    }

    for(int i = 0; i < noSeleccionados.size(); ++i) {
        for(int j = 0; j < seleccionados.size(); ++j) {
            int fila = noSeleccionados[i];
            int columna = seleccionados[j];
            if(fila < columna) {
                contribuciones[fila] += (*distancias)[fila][columna-fila-1];
            }
            else {
                contribuciones[fila] += (*distancias)[columna][fila-columna-1];
            }
            
        }
    }

    list<pair<int,double>> listaNoSeleccionados;
    for(int i = 0; i < noSeleccionados.size(); ++i) {
        listaNoSeleccionados.push_back(pair<int,double>(noSeleccionados[i],contribuciones[noSeleccionados[i]]));
    }

    listaNoSeleccionados.sort(compare_mayorContri);

    for(int i = 0; i < numElementos; ++i) {
        genes[listaNoSeleccionados.front().first] = 1;
        listaNoSeleccionados.pop_front();
    }
}

double funcion_obj_binaria(const VecInt* Solucion, const MatDouble* distancias) {
    double fitness = 0.0;
    VecInt pos;
    for(int i = 0; i < Solucion->size(); ++i) {
        if((*Solucion)[i] == 1) {
            pos.push_back(i);
        }
    }

    for(int i = 0; i < pos.size()-1; ++i) {
        for(int j = i+1; j < pos.size(); ++j) {
            fitness += (*distancias)[pos[i]][pos[j]-pos[i]-1];
        }
    }

    return fitness;
}

bool compare_mayorFitness(const Individuo& first, const Individuo& second) {
    if(first.fitness > second.fitness) {
        return true;
    }
    else {
        return false;
    }
}

bool compare_actualizado(const Individuo& first, const Individuo& second) {
    if(!first.actualizado) {
        return true;
    }
    else {
        return false;
    }
}

bool compare_mayorContri(const pair<int,double>& first, const pair<int,double>& second) {
    if(first.second >= second.second) {
        return true;
    }
    else {
        return false;
    }
}

bool compare_posiciones(const pair<int,int>& first, const pair<int,int>& second) {
    if(first.first == second.first) {
        if(first.second <= second.second) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(first.first < second.first) {
        return true;
    }
    else {
        return false;
    }
}

void operador_seleccion(std::list<Individuo>* poblacion) {
    int tam = poblacion->size();
    for(int i = 0; i < tam; ++i) {
        Individuo seleccionado = seleccionTorneo(poblacion, tam, 2);
        poblacion->push_back(seleccionado);
    }
    for(int i = 0; i < tam; ++i) {
        poblacion->pop_front();
    }
}

void operador_seleccion(const std::list<Individuo>* poblacion, std::list<Individuo>* nuevaPoblacion, int tamNuevaPoblacion) {
    for(int i = 0; i < tamNuevaPoblacion; ++i) {
        Individuo seleccionado = seleccionTorneo(poblacion, poblacion->size(), 2);
        nuevaPoblacion->push_back(seleccionado);
    }
}

Individuo seleccionTorneo(const list<Individuo>* poblacion, int tam, int tamTorneo) {
    list<Individuo> seleccionados;

    // Elegir las posiciones a seleccionar
    set<int> elegidos;
    while(elegidos.size() != tamTorneo) {
        int elegido = rand() % tam;
        elegidos.insert(elegido);
    }

    list<Individuo>::const_iterator it = poblacion->cbegin();
    int i = 0;
    set<int>::const_iterator itEle = elegidos.cbegin();

    list<Individuo>::const_iterator mejorIndividuo;
    double mejorFitness = -1;

    while(itEle != elegidos.cend()) {
        if(i == *itEle) {
            if(mejorFitness < it->fitness) {
                mejorFitness = it->fitness;
                mejorIndividuo = it;
            }
            ++itEle;
        }
        
        ++it;
        ++i;
    }

    return (*mejorIndividuo);
}

void operador_cruce_uniforme(list<Individuo>* poblacion, float probabilidadCruce, int numGenesFactible, const MatDouble* distancias) {
    int numParejasCruce = probabilidadCruce * (poblacion->size() / 2);
    
    for(int i = 0; i < numParejasCruce; ++i) {
        Individuo padre1 = poblacion->front();
        poblacion->pop_front();
        Individuo padre2 = poblacion->front();
        poblacion->pop_front();
        
        Individuo hijo;
        hijo = padre1.cruce_uniforme(&padre2);
        hijo.repair(numGenesFactible, distancias);
        poblacion->push_back(hijo);
        hijo = padre1.cruce_uniforme(&padre2);
        hijo.repair(numGenesFactible, distancias);
        poblacion->push_back(hijo);
    }
}

void operador_cruce_posicion(std::list<Individuo>* poblacion, float probabilidadCruce) {
    int numParejasCruce = probabilidadCruce * (poblacion->size() / 2);

    for(int i = 0; i < numParejasCruce; ++i) {
        Individuo padre1 = poblacion->front();
        poblacion->pop_front();
        Individuo padre2 = poblacion->front();
        poblacion->pop_front();
        
        Individuo hijo;
        hijo = padre1.cruce_posicion(&padre2);
        poblacion->push_back(hijo);
        hijo = padre1.cruce_posicion(&padre2);
        poblacion->push_back(hijo);
    }
}

void operadorMutacion(list<Individuo>* poblacion, float probabilidadMutacion) {
    int total = poblacion->size() * poblacion->front().genes.size();
    int numMutaciones = probabilidadMutacion * total;

    list<pair<int,int>> posiciones;
    for(int i = 0; i < numMutaciones; ++i) {
        int pos = rand() % total;
        int cromo = pos / poblacion->front().genes.size();
        int gen = pos % poblacion->front().genes.size();
        
        posiciones.push_back(pair<int,int>(cromo,gen));
    }

    posiciones.sort(compare_posiciones);

    int i = 0;
    auto it = poblacion->begin();

    while(!posiciones.empty()) {
        if(posiciones.front().first == i) {
            if(it->genes[posiciones.front().second] == 0) {
                it->genes[posiciones.front().second] = 1;
                bool encontrado = false;
                while(!encontrado) {
                    int pos = rand() % it->genes.size();
                    if(it->genes[pos] == 1 && pos != posiciones.front().second) {
                        encontrado = true;
                        it->genes[pos] = 0;
                    }
                }
            }
            else {
                it->genes[posiciones.front().second] = 0;
                bool encontrado = false;
                while(!encontrado) {
                    int pos = rand() % it->genes.size();
                    if(it->genes[pos] == 0) {
                        encontrado = true;
                        it->genes[pos] = 1;
                    }
                }
            }

            posiciones.pop_front();
            it->actualizado = false;
        }
        else {
            ++i;
            ++it;
        }
    }
}

void operador_reemplazo(std::list<Individuo>* poblacion, Individuo* mejorPadre) {
    poblacion->push_back(mejorPadre);

    poblacion->sort(compare_mayorFitness);
    
    int encontrado = 0;
    list<Individuo>::const_iterator posEncontrado;

    for(auto it = poblacion->cbegin(); it != poblacion->cend(); ++it) {
        if(encontrado == 2) {
            break;
        }

        if(it->fitness < mejorPadre->fitness) {
            break;
        }
        else if(it->fitness == mejorPadre->fitness) {
            if((*mejorPadre) == (*it)) {
                ++encontrado;
                posEncontrado = it;
            }
        }
    }

    if(encontrado == 2) {
        poblacion->erase(posEncontrado);
    }
    else if(encontrado == 1) {
        poblacion->pop_back();
    }
}

void operador_reemplazo(std::list<Individuo>* poblacion, const std::list<Individuo>* nuevaPoblacion) {
    for(auto it = nuevaPoblacion->begin(); it != nuevaPoblacion->end(); ++it) {
        poblacion->push_back(*it);
    }

    poblacion->sort(compare_mayorFitness);

    for(int i = 0; i < nuevaPoblacion->size(); ++i) {
        poblacion->pop_back();
    }
}

int calcularFitness(list<Individuo>* poblacion, const MatDouble* distancias) {
    poblacion->sort(compare_actualizado);
    int evaluaciones = 0;

    for(auto it = poblacion->begin(); it != poblacion->end(); ++it) {
        if(!it->actualizado) {
            it->fitness = funcion_obj_binaria(&it->genes, distancias);
            it->actualizado = true;
            ++evaluaciones;
        }
        else {
            break;
        }
    }
    
    return evaluaciones;
}

void convertirSolucion(list<Individuo>::iterator solucion_binaria, ListInt* seleccionados, VecInt* noSeleccionados) {
    for(int i = 0; i < solucion_binaria->genes.size(); ++i) {
        if(solucion_binaria->genes[i] == 0) {
            noSeleccionados->push_back(i);
        }
        else {
            seleccionados->push_back(i);
        }
    }
}

void recuperarSolucion(list<Individuo>::iterator solucion_binaria, ListInt* seleccionados) {
    auto it = seleccionados->cbegin();
    for(int i = 0; i < solucion_binaria->genes.size(); ++i) {
        if(*it == i) {
            solucion_binaria->genes[i] = 1;
            ++it;
        }   
        else {
            solucion_binaria->genes[i] = 0;
        }
    }
}

void leerArchivo(const char* nombre, int &numElemSelec, MatDouble &distancias) {
    ifstream archivo(nombre);
    if(archivo.is_open()) {
        int numElecTotal;

        // Se obtiene tanto el número de elemento total, como el número
        // de elementos de ls solución
        archivo >> numElecTotal;
        archivo >> numElemSelec;

        // Creación de la matriz triangular superior
        distancias.resize(numElecTotal-1);
        for(int i = 0; i < numElecTotal; ++i) {
            distancias[i].resize(numElecTotal-1-i);
        }

        int fil,col;
        double dist;

        // Obtención de las distancias entre los pares de elementos
        while(!archivo.eof()) {
            archivo >> fil;
            archivo >> col;
            archivo >> dist;

            distancias[fil][col-fil-1] = dist;
        }
    }
    else {
        cerr << "Error: Apertura del archivo" << endl;
    }
}
