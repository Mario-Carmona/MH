extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <vector>
#include <list>
#include <cmath>

typedef std::list<int> ListInt;
typedef std::vector<int> VecInt;
typedef std::vector<double> VecDouble;
typedef std::vector<std::vector<double>> MatDouble;

using namespace std;

double generarAleDouble() {
    const double MIN = -100;
    const double MAX = 100;

    return MIN + (double)(rand()) / ((double)(RAND_MAX/(MAX - MIN)));
}

int RouletteWheel(const VecDouble* Fitness_ant_lion) {
    int numParticipantes = 3;

    ListInt participantes;
    while(participantes.size() < numParticipantes) {
        participantes.push_back(rand() % Fitness_ant_lion->size());
        participantes.sort();
        participantes.unique();
    }

    int elegido;
    double fitnessElegido = -1;
    int i = 0;
    for(auto it = participantes.cbegin(); it != participantes.cend(); ++it, ++i) {
        if(fitnessElegido < (*Fitness_ant_lion)[*it]) {
            fitnessElegido = (*Fitness_ant_lion)[*it];
            elegido = i;
        }
    }

    return elegido;
}

int obtenerW(int iteraciones, int maxIteraciones) {
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

void actualizarCyD(int iteraciones, int maxIteraciones, double* c, double* d) {
    const double MAX = 100;
    const double MIN = -100;
    
    int w = obtenerW(iteraciones, maxIteraciones);
    double I = pow((int)(maxIteraciones/10) , w) * (iteraciones/maxIteraciones);

    *c = max(MIN, MIN / I);
    *d = min(MAX, MAX / I);
}

void generarMovimientoRandom(VecDouble* ant, const VecDouble* ant_lion, const VecDouble* elite, double c_t, double d_t) {
    const double a = -100;
    const double b = 100;
    
    for(int i = 0; i < ant_lion->size(); ++i) {
        double c_i, d_i;

        // Cálculo del movimiento respecto de la hormiga león elegida
        c_i = (*ant_lion)[i] + c_t;
        d_i = (*ant_lion)[i] + d_t;

        double R_A = (((*ant)[i] - a) * (b - c_i)) / (d_i - a) + b;

        // Cálculo del movimiento respecto de la mejor hormiga león
        c_i = (*elite)[i] + c_t;
        d_i = (*elite)[i] + d_t;

        double R_E = (((*ant)[i] - a) * (b - c_i)) / (d_i - a) + b;

        // Actualizar la posición de la hormiga de forma elitista
        (*ant)[i] = (R_A + R_E) / 2;
    }
}

bool replaceAntLion(MatDouble* M_ant_lion, VecDouble* Fitness_ant_lion, const VecInt* antLionElegidas, const MatDouble* M_ant, const VecDouble* Fitness_ant) {
    bool modificado = false;
    
    for(int i = 0; i < M_ant->size(); ++i) {
        int elegida = (*antLionElegidas)[i];
        if((*Fitness_ant)[i] < (*Fitness_ant_lion)[elegida]) {
            (*Fitness_ant_lion)[elegida] = (*Fitness_ant)[i];
            (*M_ant_lion)[elegida] = (*M_ant)[i];
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

void ant_lion_optimizer(VecDouble* sol, double* fitness) {
    int tamPoblacion = 10;

    int evalucionesTotal = 10000 * sol->size();
    int evaluacionesPorIter = 10;
    int maxIteraciones = evalucionesTotal/evaluacionesPorIter;
    
    int iteraciones = 0;

    // Inicialización de la población
    MatDouble M_ant;
    MatDouble M_ant_lion;
    for(int i = 0; i < tamPoblacion; ++i) {
        M_ant.push_back(vector<double>());
        M_ant_lion.push_back(vector<double>());
        for(int j = 0; j < sol->size(); ++j) {
            M_ant[i].push_back( generarAleDouble() );
            M_ant_lion[i].push_back( generarAleDouble() );
        }
    }

    VecDouble Fitness_ant;
    VecDouble Fitness_ant_lion;
    for(int i = 0; i < M_ant.size(); ++i) {
        Fitness_ant.push_back(cec17_fitness(&M_ant[i][0]));
        Fitness_ant_lion.push_back(cec17_fitness(&M_ant_lion[i][0]));
    }

    ++iteraciones;
    ++iteraciones;


    VecDouble elite;
    double fitnessElite = INFINITY;
    int elegido;

    for(int i = 0; i < M_ant_lion.size(); ++i) {
        if(Fitness_ant_lion[i] < fitnessElite) {
            fitnessElite = Fitness_ant_lion[i];
            elegido = i;
        }
    }
    elite = M_ant_lion[elegido];

    // Hormigas león elegidas
    VecInt antLionElegidas(M_ant.size());
    
    while(iteraciones < maxIteraciones) {
        // Movimiento de cada hormiga
        for(int i = 0; i < M_ant.size(); ++i) {
            // Seleccionar una hormiga león
            antLionElegidas[i] = RouletteWheel(&Fitness_ant_lion);

            // Actualizar c y d
            double c, d;
            actualizarCyD(iteraciones, maxIteraciones, &c, &d);

            // Genera movimiento aleatorio
            VecDouble desplazamiento;
            generarMovimientoRandom(&M_ant[i], &M_ant_lion[antLionElegidas[i]], &elite, c, d);
        }

        // Calcular fitness de todas las hormigas
        for(int i = 0; i < M_ant.size(); ++i) {
            Fitness_ant[i] = cec17_fitness(&M_ant[i][0]);
        }

        ++iteraciones;

        // Comprobar si la hormiga león se come a la hormiga
        bool modificado = replaceAntLion(&M_ant_lion, &Fitness_ant_lion, &antLionElegidas, &M_ant, &Fitness_ant);

        // Si se modifican las hormigar león se comprueba si se actualiza el elite
        if(modificado) {
            actualizarElite(&M_ant_lion, &Fitness_ant_lion, &elite, &fitnessElite);
        }
    }

    (*sol) = elite;
    (*fitness) = fitnessElite;
}

int main() {
    int seed = 1;
    srand(seed);

    vector<double> sol;
    int dim = 30;

    for (int funcid = 1; funcid <= 30; funcid++) {
        vector<double> sol(dim);
        double fitness;

        cec17_init("ant_lion", funcid, dim);

        ant_lion_optimizer(&sol, &fitness);

        cout <<"Fitness[F" <<funcid <<"]: " << scientific <<cec17_error(fitness) <<endl;
    }
}
