extern "C" {
#include "cec17.h"
}
#include <iostream>
#include <ant_lion_optimizer.h>

using namespace std;

int main() {
  vector<double> sol;
  int dim = 10;

  for (int funcid = 1; funcid <= 30; funcid++) {
    vector<double> sol(dim);
    double fitness;

    cec17_init("ant_lion", funcid, dim);

    ant_lion_optimizer(&sol);

    fitness = cec17_fitness(&sol[0]);
    cout <<"Fitness[F" <<funcid <<"]: " << scientific <<cec17_error(fitness) <<endl;
  }

}
