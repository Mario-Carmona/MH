/**
 * @file ant_lion_optimizer.cpp
 * @author Mario Carmona Segovia (mcs2000carmona@correo.ugr.es)
 * @brief Implementación del algoritmo evolutivo
 * @version 0.1
 * @date 2021-06-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */


extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <random>
#include <fstream>

typedef std::list<int> ListInt;
typedef std::vector<int> VecInt;
typedef std::vector<double> VecDouble;
typedef std::vector<std::vector<double>> MatDouble;
typedef std::list<std::pair<std::vector<double>,double>> ListMatDouble;

using namespace std;


/**
 * @brief Metodo de selección de la ruleta
 * 
 * @param M_ant_lion Lista de ant lion
 * @param gen Generador de número aleatorios
 * @return ListMatDouble::const_iterator Puntero al ant lion seleccionado
 */
ListMatDouble::iterator RouletteWheel(ListMatDouble& M_ant_lion, mt19937& gen) {
    // Obtenemos el número de ant lion
    int numIndividuos = M_ant_lion.size();

    // Calcular el fitness acumulado total
    double fitnessTotal = 0.0;
    for(auto it = M_ant_lion.begin(); it != M_ant_lion.end(); ++it) {
        fitnessTotal += it->second;
    }

    // Calcular el peso de cada ant lion
    VecDouble pesos;
    for(auto it = M_ant_lion.begin(); it != M_ant_lion.end(); ++it) {
        pesos.push_back(it->second / fitnessTotal);
    }

    // Calcular las probabilidades de cada ant lion
    VecDouble probabilidades;
    probabilidades.push_back(pesos[0]);
    for(int i = 1; i < (numIndividuos-1); ++i) {
        probabilidades.push_back(probabilidades[i-1] + pesos[i]);
    }

    // Se añade la última probabilidad acumulada de esta forma para evitar problemas de redondeo
    probabilidades.push_back(1.0);

    // Generar un número aleatorio en el rango [0,1]
    uniform_real_distribution<> dis(0.0, 1.0);
    double prob = dis(gen);

    // Selección del ant lion
    auto elegido = M_ant_lion.begin();
    for(int i = 0; i < (numIndividuos-1); ++i, ++elegido) {
        if(probabilidades[i] < prob && prob < probabilidades[i+1]) {
            break;
        }
    }

    return elegido;
}

/**
 * @brief Función para obtener el valor de w
 * 
 * @param iteraciones Iteración actual
 * @param maxIteraciones Número máximo de iteraciones
 * @return int Valor de w
 */
int obtenerW(double iteraciones, double maxIteraciones) {
    int w = 1;

    if(iteraciones > 0.1*maxIteraciones) {
        ++w;
    }
    if(iteraciones > 0.5*maxIteraciones) {
        ++w;
    }
    if(iteraciones > 0.75*maxIteraciones) {
        ++w;
    }
    if(iteraciones > 0.9*maxIteraciones) {
        ++w;
    }
    if(iteraciones > 0.95*maxIteraciones) {
        ++w;
    }

    return w;
}

/**
 * @brief Función para obtener el tamaño de la población a usar en el algoritmo
 * 
 * @param dim Dimensión de cada individuo
 * @return int Tamaño de la población
 */
int obtenerTamanioPoblacion(int dim) {
    int tam = 30;

    switch (dim)
    {
    case 30:
        tam += 10;
        break;
    case 50:
        tam += 20;
        break;
    }

    return tam;
}

/**
 * @brief Función de comparación para las poblaciones
 * 
 * @param first Primer individuo
 * @param second Segundo individuo
 * @return true Si el primero tiene menor fitness que el segundo 
 * @return false Si el segundo tiene menor fitness que el primero
 */
bool compare_menorFitness(const pair<vector<double>,double>& first, const pair<vector<double>,double>& second) {
    if(first.second <= second.second) {
        return true;
    }
    else {
        return false;
    }
}

void generarFuncionConvergencia(double c, double d) {
    ofstream archivo;
    archivo.open("func_convergencia.csv", ios::app);
    archivo << max(-100.0,c) << "," << min(100.0,d) << ",\n";
    archivo.close();
}

double obtenerI(double iteraciones, double maxIteraciones) {
    double w = obtenerW(iteraciones, maxIteraciones);
    double I = pow(10, w) * (iteraciones/maxIteraciones);

    return I;
}

/**
 * @brief Función para actualizar la posición de las hormigas en cada iteración
 * 
 * @param M_ant Lista de hormigas
 * @param M_ant_lion Lista de ant lion
 * @param iteraciones Iteración actual
 * @param incremento Incremento ó decremento del rango del movimiento por iteración
 * @param lb Cota inferior del rango de movimiento
 * @param ub Cota superior del rango de movimiento
 * @param X Vector de movimientos aleatorios acumulados para cada variable
 * @param min_X Vector del menor valor de movimiento aleatorio acumulado para cada variables
 * @param max_X Vector del mayor valor de movimiento aleatorio acumulado para cada variables
 * @param X_e Vector de movimientos aleatorios acumulados del mejor ant lion para cada variable
 * @param min_X_e Vector del menor valor de movimiento aleatorio acumulado del mejor ant lion para cada variables
 * @param max_X_e Vector del mayor valor de movimiento aleatorio acumulado del mejor ant lion para cada variables
 * @param gen Generador de número aleatorios
 */
void actualizarAnt(ListMatDouble& M_ant, ListMatDouble& M_ant_lion, double iteraciones, double maxIteraciones, VecDouble& lb, VecDouble& ub, double minValue, double maxValue, vector<double>& X, vector<double>& min_X, vector<double>& max_X, vector<double>& X_e, vector<double>& min_X_e, vector<double>& max_X_e, vector<ListMatDouble::iterator>& antLionSeleccionados, mt19937& gen) {
    for(auto it = M_ant.begin(); it != M_ant.end(); ++it) {
        // Elegimos la ant lion que va a construir la trampa
        auto elegido = RouletteWheel(M_ant_lion, gen);
        antLionSeleccionados.push_back(elegido);

        // Modificamos las distintas variables de la hormiga seleccionada
        for(int j = 0; j < it->first.size(); ++j) {
            double I = obtenerI(iteraciones, maxIteraciones);
            lb[j] = max(minValue,lb[j] / I);
            ub[j] = min(maxValue,ub[j] / I);

            // Calculo del rango de movimiento en la iteración actual de la ant lion seleccionada
            double c_i = lb[j];
            double d_i = ub[j];

            // Calculo del rango de movimiento en la iteración actual de la mejor ant lion
            double c_e = lb[j];
            double d_e = ub[j];

            // Fijamos la distribución
            uniform_real_distribution<> dis(0.0, 1.0);

            double probabilidad = dis(gen);
            if(probabilidad < 0.5) {
                c_i = c_i + elegido->first[j];
                c_e = c_e + M_ant_lion.front().first[j];
            }
            else {
                c_i = -c_i + elegido->first[j];
                c_e = -c_e + M_ant_lion.front().first[j];
            }

            probabilidad = dis(gen);
            if(probabilidad >= 0.5) {
                d_i = d_i + elegido->first[j];
                d_e = d_e + M_ant_lion.front().first[j];
            }
            else {
                d_i = -d_i + elegido->first[j];
                d_e = -d_e + M_ant_lion.front().first[j];
            }

            // Actualizar X , min_X y max_X
            probabilidad = dis(gen);
            double valor = (probabilidad > 0.5) ? 1.0 : 0.0;
            X[j] = X[j] + (2.0*valor - 1.0);
            if(min_X[j] > X[j]) {
                min_X[j] = X[j];
            }
            if(max_X[j] < X[j]) {
                max_X[j] = X[j];
            }

            // Actualizar X_e , min_X_e y max_X_e
            probabilidad = dis(gen);
            valor = (probabilidad > 0.5) ? 1.0 : 0.0;
            X_e[j] = X_e[j] + (2.0*valor - 1.0);
            if(min_X_e[j] > X_e[j]) {
                min_X_e[j] = X_e[j];
            }
            if(max_X_e[j] < X_e[j]) {
                max_X_e[j] = X_e[j];
            }

            // Calculo del movimiento aleatorio respecto del ant lion seleccionado
            double X_random_walk = (((X[j]-min_X[j])*(d_i - c_i)) / (max_X[j] - min_X[j])) + c_i;
            
            // Calculo del movimiento aleatorio respecto del mejor ant lion
            double X_e_random_walk = (((X_e[j]-min_X_e[j])*(d_e - c_e)) / (max_X_e[j] - min_X_e[j])) + c_e;

            // Calculo del movimiento aleatorio con el elitismo y actualización del valor
            // de la variable de la hormiga seleccionada
            it->first[j] = (X_random_walk + X_e_random_walk) / 2.0;

            // Acotar el nuevo valor de la variable
            if(it->first[j] < minValue) {
                it->first[j] = minValue;
            }
            if(it->first[j] > maxValue) {
                it->first[j] = maxValue;
            }
        }
    }
}

/**
 * @brief Función para actualizar la posición de las ant lions en cada iteración
 * 
 * @param M_ant Lista de hormigas
 * @param M_ant_lion Lista de ant lion
 */
void actualizarAntLion(ListMatDouble& M_ant, vector<ListMatDouble::iterator>& antLionSeleccionados) {
    auto it_ant = M_ant.begin();
    int i = 0;
    for(; it_ant != M_ant.end(); ++it_ant, ++i) {
        if(it_ant->second < antLionSeleccionados[i]->second) {
            (*antLionSeleccionados[i]) = (*it_ant);
        }
    }
}

/**
 * @brief Algoritmo original the ant lion optmimizer
 * 
 * @param sol Mejor solución encontrada
 * @param fitness Fitness de la mejor solución encontrada
 * @param lb Vector de límites inferiores del valor de las variables
 * @param ub Vector de límites superiores del valor de las variables
 * @param gen Generador de números aleatorios
 */
void ant_lion_optimizer(VecDouble& sol, double& fitness, VecDouble& lb, VecDouble& ub, mt19937& gen) {
    // Fijar distribución para la generación de valores para la creación de los individuos
    std::uniform_real_distribution<> distri_limites(lb[0],ub[0]);
    
    // Obtener el tamaño de la población que depende de la dimensión de los individuos
    int tamPoblacion = obtenerTamanioPoblacion(sol.size());

    int dim = sol.size();

    // Número máximo de evaluaciones
    int evalucionesTotal = 10000 * dim;
    // Número de evaluaciones actual
    int evaluaciones = 0;

    // Número de iteraciones actual
    int iteraciones = 0;
    // Número máximo de iteraciones
    int maxIteraciones = (double)evalucionesTotal/(double)tamPoblacion;
    

    // Inicialización de las poblaciones
    ListMatDouble M_ant;
    ListMatDouble M_ant_lion;
    for(int i = 0; i < tamPoblacion; ++i) {
        M_ant.push_back(pair<VecDouble,double>(VecDouble(), 0.0));
        M_ant_lion.push_back(pair<VecDouble,double>(VecDouble(), 0.0));
        auto it_ant = M_ant.end();
        --it_ant;
        auto it_ant_lion = M_ant_lion.end();
        --it_ant_lion;
        for(int j = 0; j < dim; ++j) {
            it_ant->first.push_back(distri_limites(gen));
            it_ant_lion->first.push_back(distri_limites(gen));
        }
    }

    // Cálculo de los fitness de ambas poblaciones
    auto it_ant = M_ant.begin();
    auto it_ant_lion = M_ant_lion.begin();
    for(int i = 0; i < M_ant.size(); ++i, ++it_ant, ++it_ant_lion) {
        it_ant->second = cec17_fitness(&it_ant->first[0]);
        ++evaluaciones;
        it_ant_lion->second = cec17_fitness(&it_ant_lion->first[0]);
        ++evaluaciones;
    }

    // Se ordenan de menor a mayor fitness las ant lion
    M_ant_lion.sort(compare_menorFitness);

    // Obtener la mejor ant lion
    VecDouble elite = M_ant_lion.front().first;
    // Obtener el fitness de la mejor ant lion
    double fitnessElite = M_ant_lion.front().second;

    // Inicialización del vector de movimientos aleatorios acumulados de cada variable
    vector<double> X(dim,0.0);
    vector<double> min_X = X;
    vector<double> max_X = X;

    // Inicialización del vector de movimientos aleatorios acumulados de cada variable
    // respecto del mejor ant lion
    vector<double> X_e(dim,0.0);
    vector<double> min_X_e = X_e;
    vector<double> max_X_e = X_e;

    double minValue = lb[0];
    double maxValue = ub[0];

    while(evaluaciones < evalucionesTotal) {
        vector<ListMatDouble::iterator> antLionSeleccionados;

        // Actualización de la posición de las hormigas
        actualizarAnt(M_ant, M_ant_lion, iteraciones, maxIteraciones, lb, ub, minValue, maxValue, X, min_X, max_X, X_e, min_X_e, max_X_e, antLionSeleccionados, gen);

        // Calcular el fitness de todas las hormigas
        for(auto it = M_ant.begin(); it != M_ant.end(); ++it) {
            it->second = cec17_fitness(&it->first[0]);
            ++evaluaciones;

            if(evaluaciones >= evalucionesTotal) {
                break;
            }
        }

        // Actualización de la posición de las ant lion, para esta actualización
        // deben estar ambas poblaciones ordenas de menor a mayor fitness
        actualizarAntLion(M_ant, antLionSeleccionados);

        // Se ordenan de menor a mayor fitness las ant lion
        M_ant_lion.sort(compare_menorFitness);

        // Actualización del mejor ant lion y de su fitness
        if(M_ant_lion.front().second < fitnessElite) {
            elite = M_ant_lion.front().first;
            fitnessElite = M_ant_lion.front().second;
        }

        // Incremento de las iteraciones
        ++iteraciones;
    }

    // Devolución de la mejor solución encontrada
    sol = elite;
    fitness = fitnessElite;
}

int main() {
    int seed = 1;
    mt19937 gen(seed);

    int dim = 10;
    int numRepeticiones = 10;

    VecDouble lb(dim,-100.0);
    VecDouble ub(dim, 100.0);

    for (int funcid = 1; funcid <= 30; funcid++) {
        for(int i = 0; i < numRepeticiones; ++i) {
            vector<double> sol(dim);
            double fitness;

            cec17_init("ant_lion", funcid, dim);

            ant_lion_optimizer(sol, fitness, lb, ub, gen);

            cout <<"Fitness[F" <<funcid <<"]: " << scientific <<cec17_error(fitness) <<endl;
        }
    }
}
