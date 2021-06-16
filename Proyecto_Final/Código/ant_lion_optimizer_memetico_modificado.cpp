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

void clip(vector<double> &sol, int lower, int upper) {
  for (auto &val : sol) {
    if (val < lower) {
      val = lower;
    }
    else if (val > upper) {
      val = upper;
    }
  }
}

void increm_bias(vector<double> &bias, vector<double> dif) {
  for (unsigned i = 0; i < bias.size(); i++) {
    bias[i] = 0.2*bias[i]+0.4*(dif[i]+bias[i]);
  }
}

void decrement_bias(vector<double> &bias, vector<double> dif) {
  for (unsigned i = 0; i < bias.size(); i++) {
    bias[i] = bias[i]-0.4*(dif[i]+bias[i]);
  }
}

/**
 * Aplica el Solis Wets
 *
 * @param  sol solucion a mejorar.
 * @param fitness fitness de la solución.
 */
template <class Random>
void soliswets(vector<double> &sol, double &fitness, double delta, int maxevals, int lower, int upper, Random &random) {
  const size_t dim = sol.size();
  vector<double> bias (dim), dif (dim), newsol (dim);
  double newfit;
  size_t i;

  int evals = 0;
  int num_success = 0;
  int num_failed = 0;

  while (evals < maxevals) {
    std::uniform_real_distribution<double> distribution(0.0, delta);

    for (i = 0; i < dim; i++) {
      dif[i] = distribution(random);
      newsol[i] = sol[i] + dif[i] + bias[i];
    }

    clip(newsol, lower, upper);
    newfit = cec17_fitness(&newsol[0]);
    evals += 1;

    if (newfit < fitness) {
      sol = newsol;
      fitness = newfit;
      increm_bias(bias, dif);
      num_success += 1;
      num_failed = 0;
    }
    else if (evals < maxevals) {

      for (i = 0; i < dim; i++) {
        newsol[i] = sol[i] - dif[i] - bias[i];
      }

      clip(newsol, lower, upper);
      newfit = cec17_fitness(&newsol[0]);
      evals += 1;

      if (newfit < fitness) {
        sol = newsol;
        fitness = newfit;
        decrement_bias(bias, dif);
        num_success += 1;
        num_failed = 0;
      }
      else {
        for (i = 0; i < dim; i++) {
          bias[i] /= 2;
        }

        num_success = 0;
        num_failed += 1;
      }
    }

    if (num_success >= 5) {
      num_success = 0;
      delta *= 2;
    }
    else if (num_failed >= 3) {
      num_failed = 0;
      delta /= 2;
    }
  }

}

ListMatDouble::const_iterator RouletteWheel(const ListMatDouble& M_ant_lion, mt19937& gen) {
    int numIndividuos = M_ant_lion.size();

    // Calcula el fitness acumulado total
    double fitnessTotal = 0.0;
    for(auto it = M_ant_lion.begin(); it != M_ant_lion.end(); ++it) {
        fitnessTotal += it->second;
    }

    // Calcular el peso de cada individuo
    VecDouble pesos;
    for(auto it = M_ant_lion.begin(); it != M_ant_lion.end(); ++it) {
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
    uniform_real_distribution<> dis(0.0, 1.0);
    double prob = dis(gen);

    auto elegido = M_ant_lion.begin();
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

void actualizarAnt(ListMatDouble& M_ant, const ListMatDouble& M_ant_lion, int iteraciones, double incremento, const VecDouble& lb, const VecDouble& ub, vector<double>& X, vector<double>& min_X, vector<double>& max_X, vector<double>& X_e, vector<double>& min_X_e, vector<double>& max_X_e, mt19937& gen) {
    for(auto it = M_ant.begin(); it != M_ant.end(); ++it) {
        auto elegido = RouletteWheel(M_ant_lion, gen);

        for(int j = 0; j < it->first.size(); ++j) {
            double c_i = lb[j] + ((double)iteraciones*incremento);
            double d_i = ub[j] - ((double)iteraciones*incremento);

            double c_e = lb[j] + ((double)iteraciones*incremento);
            double d_e = ub[j] - ((double)iteraciones*incremento);

            uniform_real_distribution<> dis(0.0, 1.0);

            // Actualizar X
            double probabilidad = dis(gen);
            double valor = (probabilidad > 0.5) ? 1.0 : 0.0;
            X[j] = X[j] + (2.0*valor - 1.0);
            if(min_X[j] > X[j]) {
                min_X[j] = X[j];
            }
            if(max_X[j] < X[j]) {
                max_X[j] = X[j];
            }

            // Actualizar X_e
            probabilidad = dis(gen);
            valor = (probabilidad > 0.5) ? 1.0 : 0.0;
            X_e[j] = X_e[j] + (2.0*valor - 1.0);
            if(min_X_e[j] > X_e[j]) {
                min_X_e[j] = X_e[j];
            }
            if(max_X_e[j] < X_e[j]) {
                max_X_e[j] = X_e[j];
            }

            double X_random_walk = (((X[j]-min_X[j])*(d_i - c_i)) / (max_X[j] - min_X[j])) + c_i;
            double X_e_random_walk = (((X_e[j]-min_X_e[j])*(d_e - c_e)) / (max_X_e[j] - min_X_e[j])) + c_e;

            it->first[j] = (X_random_walk + X_e_random_walk) / 2.0;

            if(it->first[j] < lb[j]) {
                it->first[j] = lb[j];
            }
            if(it->first[j] > ub[j]) {
                it->first[j] = ub[j];
            }
        }
    }
}

void actualizarAntLion(ListMatDouble& M_ant, ListMatDouble& M_ant_lion) {
    int elegidos_ant = 0;
    int elegidos_ant_lion = 0;
    int tamPoblacion = M_ant.size();

    for(int i = 0; i < tamPoblacion; ++i) {
        if(M_ant.front().second < M_ant_lion.front().second) {
            M_ant_lion.push_back( M_ant.front() );
            M_ant.pop_front();
            ++elegidos_ant;
        }
        else {
            M_ant_lion.push_back( M_ant_lion.front() );
            M_ant_lion.pop_front();
            ++elegidos_ant_lion;
        }
    }

    if(elegidos_ant_lion != tamPoblacion) {
        for(int i = 0; i < tamPoblacion; ++i) {
            if(elegidos_ant == tamPoblacion) {
                M_ant.push_back( M_ant_lion.front() );
                M_ant_lion.pop_front();
            }
            else {
                if(elegidos_ant_lion == tamPoblacion) {
                    M_ant.push_back( M_ant.front() );
                    M_ant.pop_front();
                }
                else {
                    if(M_ant.front().second < M_ant_lion.front().second) {
                        M_ant.push_back( M_ant.front() );
                        M_ant.pop_front();
                        ++elegidos_ant;
                    }
                    else {
                        M_ant.push_back( M_ant_lion.front() );
                        M_ant_lion.pop_front();
                        ++elegidos_ant_lion;
                    }
                }
            }
        }
    }
}

void ant_lion_optimizer_memetico(VecDouble& sol, double& fitness, const VecDouble& lb, const VecDouble& ub, mt19937& gen) {
    std::uniform_real_distribution<> distri_limites(lb[0],ub[0]);
    
    int tamPoblacion = obtenerTamanioPoblacion(sol.size());

    int dim = sol.size();

    int evalucionesTotal = 10000 * dim;
    int evaluaciones = 0;

    int intendidad_BL = 500;
    double probabilidad = 0.1;
    double delta = 0.2;
    int generaciones = 0;
    int max_generaciones = 20;

    int iteraciones = 0;
    int maxIteraciones = (double)evalucionesTotal/ ((double)tamPoblacion + intendidad_BL/max_generaciones);
    double incremento = ub[0]/(double)(maxIteraciones+1);
    
    


    // Inicialización de la población
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

    auto it_ant = M_ant.begin();
    auto it_ant_lion = M_ant_lion.begin();
    for(int i = 0; i < M_ant.size(); ++i, ++it_ant, ++it_ant_lion) {
        it_ant->second = cec17_fitness(&it_ant->first[0]);
        ++evaluaciones;
        it_ant_lion->second = cec17_fitness(&it_ant_lion->first[0]);
        ++evaluaciones;
    }

    M_ant_lion.sort(compare_menorFitness);

    VecDouble elite = M_ant_lion.front().first;
    double fitnessElite = M_ant_lion.front().second;

    vector<double> X(dim,0.0);
    vector<double> min_X = X;
    vector<double> max_X = X;

    vector<double> X_e(dim,0.0);
    vector<double> min_X_e = X_e;
    vector<double> max_X_e = X_e;


    while(evaluaciones < evalucionesTotal) {
        actualizarAnt(M_ant, M_ant_lion, iteraciones, incremento, lb, ub, X, min_X, max_X, X_e, min_X_e, max_X_e, gen);


        // Calcular fitness de todas las hormigas
        for(auto it = M_ant.begin(); it != M_ant.end(); ++it) {
            it->second = cec17_fitness(&it->first[0]);
            ++evaluaciones;

            if(evaluaciones >= evalucionesTotal) {
                break;
            }
        }
        

        M_ant.sort(compare_menorFitness);


        actualizarAntLion(M_ant, M_ant_lion);

        ++generaciones;
        ++iteraciones;

        if(generaciones == max_generaciones) {
            
            int numAnts = M_ant_lion.size() * probabilidad;

            auto it = M_ant_lion.begin();
            for(int i = 0; i < numAnts; ++i, ++it) {
                int max_evals = min(intendidad_BL, (evalucionesTotal-evaluaciones));
                soliswets(it->first, it->second, delta, max_evals, lb[0], ub[0], gen);

                evaluaciones += max_evals;
                if(evaluaciones >= evalucionesTotal) {
                    break;
                }
            }

            generaciones = 0;
        }


        M_ant_lion.sort(compare_menorFitness);


        if(M_ant_lion.front().second < fitnessElite) {
            elite = M_ant_lion.front().first;
            fitnessElite = M_ant_lion.front().second;
        }
        else {
            M_ant_lion.pop_front();
            M_ant_lion.push_front(pair<vector<double>,double>(elite, fitnessElite));
        }
    }

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

            cec17_init("ant_lion_memetico_modificado", funcid, dim);

            ant_lion_optimizer_memetico(sol, fitness, lb, ub, gen);

            cout <<"Fitness[F" <<funcid <<"]: " << scientific <<cec17_error(fitness) <<endl;
        }
    }
}
