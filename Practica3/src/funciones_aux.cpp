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

    // Se ordenan los elementos de menor a mayor contribución
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
            // Tener en cuenta la distancia calculada en la contribución de los
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

            // Sumar la distancia de cada elemento en el vector
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

    // Generar los genes del individuo
    int numGenes = 0;
    while(numGenes != genesActivos) {
        int nuevaPos = rand() % tam;
        if(this->genes[nuevaPos] == 0) {
            this->genes[nuevaPos] = 1;
            ++numGenes;
        }
    }

    // Obtener el fitness del individuo
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
            // Elegir el padre del que coger el gen diferente
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

    // Obtener las posiciones a cambiar porque son
    // genes distintos en ambos padres
    for(int i = 0; i < hijo.genes.size(); ++i) {
        if(this->genes[i] != otro->genes[i]) {
            posACambiar.push_back(i);
            valores.push_back(this->genes[i]);
        }
    }

    while(!valores.empty()) {
        // Asignar un gen aleatorio de los obtenidos
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

    // Si sobran genes activos, se eliminan
    if(diferencia > 0) {
        eliminarGenesActivos(diferencia, distancias);
    }
    else if(diferencia < 0) { // Si faltan genes activos, se añaden
        aniadirGenesActivos(abs(diferencia), distancias);
    }
}

void Individuo::eliminarGenesActivos(int numElementos, const MatDouble* distancias) {
    MapDouble contribuciones;
    VecInt seleccionados;

    // Inicializar las contribuciones y obtener los elemenos seleccionados
    for(int i = 0; i < genes.size(); ++i) {
        if(genes[i] == 1) {
            seleccionados.push_back(i);
            contribuciones[i] = 0.0;
        }
    }

    // Calcular las contribuciones
    for(int i = 0; i < seleccionados.size()-1; ++i) {
        for(int j = i+1; j < seleccionados.size(); ++j) {
            int fila = seleccionados[i];
            int columna = seleccionados[j];
            contribuciones[fila] += (*distancias)[fila][columna-fila-1];
            contribuciones[columna] += (*distancias)[fila][columna-fila-1];
        }
    }

    // Se pasan los elementos y sus contribuciones a una lista
    list<pair<int,double>> listaSeleccionados;
    for(int i = 0; i < seleccionados.size(); ++i) {
        listaSeleccionados.push_back(pair<int,double>(seleccionados[i],contribuciones[seleccionados[i]]));
    }

    // Se ordena la lista por mayor contribución
    listaSeleccionados.sort(compare_mayorContri);

    // Se eliminan tantos elementos como se indique
    for(int i = 0; i < numElementos; ++i) {
        genes[listaSeleccionados.front().first] = 0;
        listaSeleccionados.pop_front();
    }
}

void Individuo::aniadirGenesActivos(int numElementos, const MatDouble* distancias) {
    MapDouble contribuciones;
    VecInt noSeleccionados;
    VecInt seleccionados;

    // Inicializar las contribuciones y obtener los elemenos seleccionados y no seleccionados
    for(int i = 0; i < genes.size(); ++i) {
        if(genes[i] == 0) {
            noSeleccionados.push_back(i);
            contribuciones[i] = 0.0;
        }
        else {
            seleccionados.push_back(i);
        }
    }

    // Calcular las contribuciones
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

    // Se pasan los elementos y sus contribuciones a una lista
    list<pair<int,double>> listaNoSeleccionados;
    for(int i = 0; i < noSeleccionados.size(); ++i) {
        listaNoSeleccionados.push_back(pair<int,double>(noSeleccionados[i],contribuciones[noSeleccionados[i]]));
    }

    // Se ordena la lista por mayor contribución
    listaNoSeleccionados.sort(compare_mayorContri);

    // Se añaden tantos elementos como se indique
    for(int i = 0; i < numElementos; ++i) {
        genes[listaNoSeleccionados.front().first] = 1;
        listaNoSeleccionados.pop_front();
    }
}

double funcion_obj_binaria(const VecInt* Solucion, const MatDouble* distancias) {
    double fitness = 0.0;

    // Obtener los elementos seleccionados
    VecInt pos;
    for(int i = 0; i < Solucion->size(); ++i) {
        if((*Solucion)[i] == 1) {
            pos.push_back(i);
        }
    }

    // Calcular el fitness
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
    // Añadir el mismo número de individuos resultado del torneo que el número 
    // original de individuos de la población
    for(int i = 0; i < tam; ++i) {
        Individuo seleccionado = seleccionTorneo(poblacion, tam, 2);
        poblacion->push_back(seleccionado);
    }

    // Eliminar los individuos antiguos
    for(int i = 0; i < tam; ++i) {
        poblacion->pop_front();
    }
}

void operador_seleccion(const std::list<Individuo>* poblacion, std::list<Individuo>* nuevaPoblacion, int tamNuevaPoblacion) {
    // Se seleccionan tanto individuos como tamaño va a tener la nueva población
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

    // Comprobar que individuo es el mejor
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
        // Se crean dos hijos que se añaden al final de la lista, y se 
        // eliminan sus dos padres
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
        // Se crean dos hijos que se añaden al final de la lista, y se 
        // eliminan sus dos padres
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

    // Se obtiene al hacer los cromosomas y los genes a mutar
    list<pair<int,int>> posiciones;
    for(int i = 0; i < numMutaciones; ++i) {
        int pos = rand() % total;
        int cromo = pos / poblacion->front().genes.size();
        int gen = pos % poblacion->front().genes.size();
        
        posiciones.push_back(pair<int,int>(cromo,gen));
    }

    // Se ordenan las posiciones a mutar de menor a mayor, para
    // que su recorrido sea más eficiente
    posiciones.sort(compare_posiciones);

    int i = 0;
    auto it = poblacion->begin();

    // Se realizan todas la mutaciones
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
    // Se añade el mejor padre a la nueva población
    poblacion->push_back(mejorPadre);

    // Se ordena la población de mayor a menor según el fitness de los individuos
    // Esto se hace para encontrar antes al mejor padre, ya que tendrá un fitness alto
    // en la mayoría de los casos
    poblacion->sort(compare_mayorFitness);
    
    int encontrado = 0;
    list<Individuo>::const_iterator posEncontrado;

    // Se busca cuantas veces a aparece el mejor padre en la población, se deja de buscar
    // cuando el fitness es menor del que tiene el mejor padre, ya que la población está
    // ordenada.
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

    // Si se encuentran dos, se elimina uno de ellos; porque eso
    // significa que el mejor padre se había mantenido en la nueva población
    if(encontrado == 2) {
        poblacion->erase(posEncontrado);
    }
    else if(encontrado == 1) {  // Si sólo hay uno se elimina el último elemento de la lista que no sea el mejor padre
        auto it = poblacion->cend();
        --it;
        for(; it != poblacion->cbegin(); --it) {
            if((*it) == (mejorPadre)) {
                poblacion->erase(it);
                break;
            }
        }
    }
}

void operador_reemplazo(std::list<Individuo>* poblacion, const std::list<Individuo>* nuevaPoblacion) {
    // Añado los individuos de la nueva población a la población actual
    for(auto it = nuevaPoblacion->begin(); it != nuevaPoblacion->end(); ++it) {
        poblacion->push_back(*it);
    }

    // Se ordena la población actual de mayor a menor según el fitness
    poblacion->sort(compare_mayorFitness);

    // Se eliminan tanto últimos elementos de la población como elementos de la nueva
    // población hayan sido añadidos
    for(int i = 0; i < nuevaPoblacion->size(); ++i) {
        poblacion->pop_back();
    }
}

int calcularFitness(list<Individuo>* poblacion, const MatDouble* distancias) {
    // Se ordena la población de tal forma que los que necesitan
    // recalcular su fitness se encuentran al principio
    poblacion->sort(compare_actualizado);
    int evaluaciones = 0;

    // Se recalcular el fitness de los individuos no actualizados
    for(auto it = poblacion->begin(); it != poblacion->end(); ++it) {
        if(!it->actualizado) {
            it->fitness = funcion_obj_binaria(&it->genes, distancias);
            it->actualizado = true;
            ++evaluaciones;
        }
        else {  // Se deja de recalcular cuando nos encontramos con un individuo actualizado
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

double funcion_obj(const VecInt* Solucion, const MatDouble* distancias) {
    double total = 0.0;

    for(int i = 0; i < Solucion->size()-1; ++i) {
        for(int j = i+1; j < Solucion->size(); ++j) {
            // Sumar la distancia entre los elementos que conforman la solución
            if((*Solucion)[i] > (*Solucion)[j]) {
                total += (*distancias)[(*Solucion)[j]][(*Solucion)[i]-(*Solucion)[j]-1];
            }
            else {
                total += (*distancias)[(*Solucion)[i]][(*Solucion)[j]-(*Solucion)[i]-1];
            }
        }
    }

    return total;
}

double funcion_obj_facto(const VecInt* Solucion, const MatDouble* distancias,
                       int elemSustituido, int elemIncluido, double coste_actual) {
    
    // Nuevo coste de la solución al intercambiar el elemento
    double new_coste = coste_actual;

    for(int aux = 0; aux < Solucion->size(); ++aux) {
        if((*Solucion)[aux] != elemSustituido) {
            // Se restan las distancias al elemento a sustituir al coste de la solución
            if((*Solucion)[aux] > elemSustituido) {
                new_coste -= (*distancias)[elemSustituido][(*Solucion)[aux]-elemSustituido-1];
            }
            else {
                new_coste -= (*distancias)[(*Solucion)[aux]][elemSustituido-(*Solucion)[aux]-1];
            }

            // Se suman las distancias al elemento a incluir al coste de la solución
            if((*Solucion)[aux] > elemIncluido) {
                new_coste += (*distancias)[elemIncluido][(*Solucion)[aux]-elemIncluido-1];
            }
            else {
                new_coste += (*distancias)[(*Solucion)[aux]][elemIncluido-(*Solucion)[aux]-1];
            }
        }
    }

    return new_coste;
}    

void Int(VecInt* Solucion, int elemASustituir, VecInt* noSeleccionados,
         int elemAIncluir, const MatDouble* distancias) {

    // Intercambiar los elementos
    int a = (*Solucion)[elemASustituir];
    int b = (*noSeleccionados)[elemAIncluir];

    (*noSeleccionados)[elemAIncluir] = a;

    (*Solucion)[elemASustituir] = b;
}

double enfriarTemperatura(double temp, double temp_ini, double temp_final, int iteraciones) {
    if(temp_final >= temp_ini) {
        temp_final = temp_final / 100;
    }
    
    double beta = (temp_ini - temp_final) / (iteraciones*temp_ini*temp_final);

    return (temp / (1 + beta*temp));
}

void generarSolAleatoria(ListInt* Solucion, VecInt* noSeleccionados, int m, int n) {
    while(Solucion->size() < m) {
        int nuevo = rand() % n;
        Solucion->push_back(nuevo);
        Solucion->sort();
        Solucion->unique();
    }

    auto it = Solucion->cbegin();
    while(it != Solucion->cend()) {
        for(int i = 0; i < n; ++i) {
            if(*it == i) {
                ++it;
            }
            else {
                noSeleccionados->push_back(i);
            }
        }
    }
}

double ES(VecInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iter, int iter_max) {
    // Valores de los parámetros
    double fi = 0.3;
    double mu = 0.3;
    double temp_final = 0.001;

    double coste_actual = funcion_obj(Solucion, distancias);
    VecInt mejorSolucion = (*Solucion);
    double mejorFitness = coste_actual;

    // Cálculo de la temperatura inicial
    double temp_ini = (mu*coste_actual) / (-log(fi));
    double temp = temp_ini;
    
    int evaluaciones = (*iter);
    int iteraciones = 0;
    int numExitos = 1;
    int numVecGenerados;

    int m = Solucion->size();
    int numMaxVecGenerados = 10*m;
    int numMaxExitos = m;

    while(evaluaciones != iter_max && numExitos > 0) {
        numExitos = 0;
        numVecGenerados = 0;

        while(numExitos != numMaxExitos && numVecGenerados != numMaxVecGenerados) {
            // Generar vecino
            int elemASustituir = rand() % Solucion->size();
            int elemAIncluir = rand() % noSeleccionados->size();

            // Se comprueba si se sustituye la solución acutal
            // por el nuevo vecino
            double coste_vecino = funcion_obj_facto(Solucion, distancias, (*Solucion)[elemASustituir], (*noSeleccionados)[elemAIncluir], coste_actual);
            ++evaluaciones;
            ++numVecGenerados;

            double diferencia = coste_actual - coste_vecino;
            if(diferencia < 0) {
                Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                coste_actual = coste_vecino;

                if(coste_actual > mejorFitness) {
                    mejorFitness = coste_actual;
                    mejorSolucion = (*Solucion);
                }

                ++numExitos;
            }
            else {
                double probabilidad = rand() / (RAND_MAX + 1.);
                int k = 1;
                if(probabilidad <= exp( (-diferencia) / (k*temp) )) {
                    Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                    coste_actual = coste_vecino;

                    ++numExitos;
                }
            }

            if(evaluaciones == iter_max) {
                break;
            }
        }
        ++iteraciones;

        // Enfriamiento
        temp = enfriarTemperatura(temp, temp_ini, temp_final, iteraciones);
    }

    Solucion->clear();
    for(auto i : mejorSolucion) {
        Solucion->push_back(i);
    }

    (*iter) = evaluaciones;

    return mejorFitness;
}

double funcion_obj_facto(const list<pair<int,double>>* Solucion, const MatDouble* distancias,
                       list<pair<int,double>>::iterator elemSustituido, int elemIncluido, double coste_actual) {

    // Nuevo coste de la solución al intercambiar el elemento
    double new_coste = coste_actual;

    for(auto aux = Solucion->begin(); aux != Solucion->end(); ++aux) {
        if(aux->first != elemSustituido->first) {
            // Se restan las distancias al elemento a sustituir al coste de la solución
            if(aux->first > elemSustituido->first) {
                new_coste -= (*distancias)[elemSustituido->first][aux->first-elemSustituido->first-1];
            }
            else {
                new_coste -= (*distancias)[aux->first][elemSustituido->first-aux->first-1];
            }

            // Se suman las distancias al elemento a incluir al coste de la solución
            if(aux->first > elemIncluido) {
                new_coste += (*distancias)[elemIncluido][aux->first-elemIncluido-1];
            }
            else {
                new_coste += (*distancias)[aux->first][elemIncluido-aux->first-1];
            }
        }
    }

    return new_coste;
}    

void Int(std::list<std::pair<int,double>>* Solucion, std::list<std::pair<int,double>>::iterator elemASustituir, VecInt* noSeleccionados,
         int elemAIncluir, const MatDouble* distancias) {

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
            if(i->first > (*noSeleccionados)[elemAIncluir]) {
                dist = (*distancias)[(*noSeleccionados)[elemAIncluir]][i->first-(*noSeleccionados)[elemAIncluir]-1];
            }
            else {
                dist = (*distancias)[i->first][(*noSeleccionados)[elemAIncluir]-i->first-1];
            }

            i->second += dist;
            contri_j += dist;                      
        }
    }

    // Intercambiar los elementos
    int a = elemASustituir->first;
    int b = (*noSeleccionados)[elemAIncluir];

    (*noSeleccionados)[elemAIncluir] = a;

    Solucion->erase(elemASustituir);
    Solucion->push_back(pair<int,double>(b,contri_j));
}

double ES_inte(VecInt *Solucion, VecInt *noSeleccionados, const MatDouble *distancias, int* iter, int iter_max) {
    list<pair<int,double>> solucion_contri;
    for(auto i : (*Solucion)) {
        solucion_contri.push_back(pair<int,double>(i,0.0));
    }
    
    // Valores de los parámetros
    double fi = 0.3;
    double mu = 0.3;
    double temp_final = 0.001;

    double coste_actual = funcion_obj(&solucion_contri, distancias);
    list<pair<int,double>> mejorSolucion = solucion_contri;
    double mejorFitness = coste_actual;

    // Cálculo de la temperatura inicial
    double temp_ini = (mu*coste_actual) / (-log(fi));
    double temp = temp_ini;
    
    int evaluaciones = (*iter);
    int iteraciones = 0;
    int numExitos = 1;
    int numVecGenerados;

    int m = Solucion->size();
    int numMaxVecGenerados = 10*m;
    int numMaxExitos = m;

    solucion_contri.sort(compare_menorContri);

    while(evaluaciones != iter_max && numExitos > 0) {
        numExitos = 0;
        numVecGenerados = 0;

        while(numExitos != numMaxExitos && numVecGenerados != numMaxVecGenerados) {
            // Generar vecino
            auto elemASustituir = solucion_contri.begin();
            int elemAIncluir = rand() % noSeleccionados->size();

            // Se comprueba si se sustituye la solución acutal
            // por el nuevo vecino
            double coste_vecino = funcion_obj_facto(&solucion_contri, distancias, elemASustituir, (*noSeleccionados)[elemAIncluir], coste_actual);
            ++evaluaciones;
            ++numVecGenerados;

            double diferencia = coste_actual - coste_vecino;
            if(diferencia < 0) {
                Int(&solucion_contri, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                coste_actual = coste_vecino;

                solucion_contri.sort(compare_menorContri);

                if(coste_actual > mejorFitness) {
                    mejorFitness = coste_actual;
                    mejorSolucion = solucion_contri;
                }

                ++numExitos;
            }
            else {
                double probabilidad = rand() / (RAND_MAX + 1.);
                int k = 1;
                if(probabilidad <= exp( (-diferencia) / (k*temp) )) {
                    Int(&solucion_contri, elemASustituir, noSeleccionados, elemAIncluir, distancias);
                    coste_actual = coste_vecino;

                    solucion_contri.sort(compare_menorContri);

                    ++numExitos;
                }
            }

            if(evaluaciones == iter_max) {
                break;
            }
        }
        ++iteraciones;

        // Enfriamiento
        temp = enfriarTemperatura(temp, temp_ini, temp_final, iteraciones);
    }

    Solucion->clear();
    for(auto i : mejorSolucion) {
        Solucion->push_back(i.first);
    }

    (*iter) = evaluaciones;

    return mejorFitness;
}

void operador_mutacion_ILS(ListInt* Solucion, VecInt* noSeleccionados, const MatDouble* distancias, int t) {
    int numMutaciones = 0;
    while(numMutaciones != t) {
        ListIntIt elemASustituir = Solucion->begin();
        VecIntIt elemAIncluir = noSeleccionados->begin();

        int aux1 = rand() % Solucion->size();
        int aux2 = rand() % noSeleccionados->size();

        for(int i = 0; i < max(aux1, aux2); ++i) {
            if(i < aux1) {
                ++elemASustituir;
            }
            if(i < aux2) {
                ++elemAIncluir;
            }
        }

        Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);

        ++numMutaciones;
    }
}

void operador_mutacion_ILS(VecInt* Solucion, VecInt* noSeleccionados, const MatDouble* distancias, int t) {
    int numMutaciones = 0;
    while(numMutaciones != t) {
        int elemASustituir = rand() % Solucion->size();
        int elemAIncluir = rand() % noSeleccionados->size();

        Int(Solucion, elemASustituir, noSeleccionados, elemAIncluir, distancias);

        ++numMutaciones;
    }
}

void generarSolAleatoria(VecInt* Solucion, VecInt* noSeleccionados, int m, int n) {
    ListInt SolInter;
    while(SolInter.size() < m) {
        int nuevo = rand() % n;
        SolInter.push_back(nuevo);
        SolInter.sort();
        SolInter.unique();
    }

    for(auto i : SolInter) {
        Solucion->push_back(i);
    }

    auto it = Solucion->cbegin();
    while(it != Solucion->cend()) {
        for(int i = 0; i < n; ++i) {
            if(*it == i) {
                ++it;
            }
            else {
                noSeleccionados->push_back(i);
            }
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
