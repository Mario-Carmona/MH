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

double generarNumAleatorio(double MIN, double MAX) {
    return MIN + (double)(rand()) / ((double)(RAND_MAX/(MAX - MIN)));
}

ListMatDouble::iterator RouletteWheel(ListMatDouble* fitness_ant_lion) {
    int numIndividuos = fitness_ant_lion->size();

    // Calcula el fitness acumulado total
    double fitnessTotal = 0.0;
    for(auto it = fitness_ant_lion->begin(); it != fitness_ant_lion->end(); ++it) {
        fitnessTotal += it->second;
    }

    // Calcular el peso de cada individuo
    VecDouble pesos;
    for(auto it = fitness_ant_lion->begin(); it != fitness_ant_lion->end(); ++it) {
        pesos.push_back(it->second / fitnessTotal);
    }

    // Calcular la probabilidades de cada individuo
    VecDouble probabilidades;
    probabilidades.push_back(pesos[0]);
    for(int i = 1; i < (numIndividuos-1); ++i) {
        probabilidades.push_back(probabilidades[i-1] + pesos[i]);
    }
    // Se añade de esta forma para evitar problemas de redondeo
    probabilidades.push_back(1.0);

    // Generar un número aleatorio entre [0,1]
    double prob = generarNumAleatorio(0, 1);

    auto elegido = fitness_ant_lion->begin();
    for(int i = 0; i < (numIndividuos-1); ++i, ++elegido) {
        if(probabilidades[i] < prob && prob < probabilidades[i+1]) {
            break;
        }
    }

    return elegido;
}

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



void actualizarCyD(double iteraciones, double maxIteraciones, double* c, double* d) {
    int w = obtenerW(iteraciones, maxIteraciones);
    double I = pow((maxIteraciones/10),w) * (iteraciones/maxIteraciones);

    *c /= I;
    *d /= I;
}

VecDouble generarMovimientoRandom(const VecDouble* ant_lion, double c, double d, double X, double min_X, double max_X) {
    VecDouble movimiento;
    
    int dim = ant_lion->size();
    for(int i = 0; i < dim; ++i) {
        double a = min_X;
        double b = max_X;
        double c_i = c + (*ant_lion)[i];
        double d_i = d + (*ant_lion)[i];

        double X_norma = (((X-a)*(d_i-c_i)) / (b-a)) + c_i ;

        movimiento.push_back(X_norma);
    }

    return movimiento;
}

void actualizarPosicion(VecDouble* ant, const VecDouble* R_A, const VecDouble* R_E, const double MIN, const double MAX) {
    int dim = ant->size();
    for(int i = 0; i < dim; ++i) {
        (*ant)[i] = ((*R_A)[i] + (*R_E)[i]) / 2;
        if((*ant)[i] > MIN) {
            (*ant)[i] = MIN;
        }
        if((*ant)[i] < MAX) {
            (*ant)[i] = MAX;
        }
    }
}

bool replaceAntLion(ListMatDouble* M_ant_lion, vector<ListMatDouble::iterator>* antLionElegidas, const MatDouble* M_ant, const VecDouble* Fitness_ant) {
    bool modificado = false;
    
    for(int i = 0; i < M_ant->size(); ++i) {
        auto elegida = (*antLionElegidas)[i];
        if((*Fitness_ant)[i] < elegida->second) {
            elegida->second = (*Fitness_ant)[i];
            elegida->first = (*M_ant)[i];
            modificado = true;
        }
    }

    return modificado;
}

void actualizarElite(const MatDouble* M_ant_lion, const VecDouble* Fitness_ant_lion, VecDouble* elite, double* fitnessElite) {
    int elegido;
    double fitnessElegido = (*fitnessElite);
    bool modificado = false;
    for(int i = 0; i < M_ant_lion->size(); ++i) {
        if((*Fitness_ant_lion)[i] < fitnessElegido) {
            fitnessElegido = (*Fitness_ant_lion)[i];
            elegido = i;
            modificado = true;
        }
    }

    if(modificado) {
        (*fitnessElite) = fitnessElegido;
        (*elite) = (*M_ant_lion)[elegido];
    }
}

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

bool compare_mayorFitness(const pair<vector<double>,double>& first, const pair<vector<double>,double>& second) {
    if(first.second >= second.second) {
        return true;
    }
    else {
        return false;
    }
}

void calcularX(double* X, double* min_X, double* max_X) {
    *X = *X + (2*((generarNumAleatorio(0,1) > 0.5) ? 1 : 0) - 1);

    if(*X > *max_X) {
        *max_X = *X;
    }
    if(*X < *min_X) {
        *min_X = *X;
    }
}

void generarFuncionConvergencia(double c, double d) {
    ofstream archivo;
    archivo.open("func_convergencia.csv", ios::app);
    archivo << max(-100.0,c) << "," << min(100.0,d) << ",\n";
    archivo.close();
}

void ant_lion_optimizer(VecDouble* sol, double* fitness, const double MIN, const double MAX) {
    int tamPoblacion = obtenerTamanioPoblacion(sol->size());

    int dim = sol->size();

    int evalucionesTotal = 10000 * dim;
    int evaluaciones = 0;

    int iteraciones = 1;
    int maxIteraciones = (double)evalucionesTotal/(double)tamPoblacion;
    
    

    // Inicialización de la población
    MatDouble M_ant;
    ListMatDouble M_ant_lion;
    for(int i = 0; i < tamPoblacion; ++i) {
        M_ant.push_back(VecDouble());
        M_ant_lion.push_back(pair<VecDouble,double>(VecDouble(), 0.0));
        auto it = M_ant_lion.end();
        --it;
        for(int j = 0; j < dim; ++j) {
            M_ant[i].push_back( generarNumAleatorio(MIN, MAX) );
            it->first.push_back( generarNumAleatorio(MIN, MAX) );
        }
    }

    VecDouble Fitness_ant;
    auto it = M_ant_lion.begin();
    for(int i = 0; i < M_ant.size(); ++i, ++it) {
        Fitness_ant.push_back(cec17_fitness(&M_ant[i][0]));
        ++evaluaciones;
        it->second = cec17_fitness(&it->first[0]);
        ++evaluaciones;
    }

    M_ant_lion.sort(compare_mayorFitness);

    // Hormigas león elegidas
    vector<ListMatDouble::iterator> antLionElegidas(M_ant.size());

    double X = 0.0;
    double min_X = X;
    double max_X = X;
    generarFuncionConvergencia(MIN, MAX);

    while(evaluaciones < evalucionesTotal) {
        // Actualizar X
        calcularX(&X, &min_X, &max_X);

        // Actualizar c y d
        double c = MIN;
        double d = MAX;
        actualizarCyD((double)iteraciones, (double)maxIteraciones, &c, &d);
        generarFuncionConvergencia(c, d);

        // Genera movimiento aleatorio respecto del elite
        VecDouble R_E = generarMovimientoRandom(&M_ant_lion.front().first, c, d, X, min_X, max_X);

        // Movimiento de cada hormiga
        for(int i = 0; i < M_ant.size(); ++i) {
            // Seleccionar una hormiga león
            antLionElegidas[i] = RouletteWheel(&M_ant_lion);

            // Genera movimiento aleatorio
            VecDouble R_A = generarMovimientoRandom(&antLionElegidas[i]->first, c, d, X, min_X, max_X);

            // Actualizar posición de la hormiga
            actualizarPosicion(&M_ant[i], &R_A, &R_E, MIN, MAX);
        }

        // Calcular fitness de todas las hormigas
        for(int i = 0; i < M_ant.size(); ++i) {
            Fitness_ant[i] = cec17_fitness(&M_ant[i][0]);
            ++evaluaciones;

            if(evaluaciones >= evalucionesTotal) {
                break;
            }
        }

        // Comprobar si la hormiga león se come a la hormiga
        bool modificado = replaceAntLion(&M_ant_lion, &antLionElegidas, &M_ant, &Fitness_ant);

        // Si se modifican las hormigas león se comprueba si se actualiza el elite
        if(modificado) {
            M_ant_lion.sort(compare_mayorFitness);
            cout << M_ant_lion.front().second << endl;
        }

        ++iteraciones;
    }

    (*sol) = M_ant_lion.front().first;
    (*fitness) = M_ant_lion.front().second;
}

int main() {
    int seed = 1;
    srand(seed);

    int dim = 10;
    int numRepeticiones = 10;

    for (int funcid = 1; funcid <= 30; funcid++) {
        for(int i = 0; i < numRepeticiones; ++i) {
            vector<double> sol(dim);
            double fitness;

            cec17_init("ant_lion", funcid, dim);

            double MIN = -100.0;
            double MAX = 100.0;

            ant_lion_optimizer(&sol, &fitness, MIN, MAX);

            cout <<"Fitness[F" <<funcid <<"]: " << scientific <<cec17_error(fitness) <<endl;
        }
    }
}
