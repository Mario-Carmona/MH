#include "define.h"

STRUCTURE *Old, *New, Mejor;
float *B, *P;

int n_generaciones,
    long_poblacion,
    n_genes;

float prob_cruce,
      prob_mutacion,
      semilla,
      M,
      Max_peso;

char fich_datos[40];

int *sample;
unsigned long Seed;
unsigned long OrigSeed;

/* Funciones */
float (*eval)(TipoCromosoma);
float eval_fuerte (TipoCromosoma);
void Input (int, char**), Initialize (void), Evaluate (void),
     Select (void), Cruce_Multipunto (void), Mutacion_Uniforme (void),
     Elitist (void);


float  Ave_current_perf;
float  Best;
float  Best_current_perf;
int    Best_guy;
int    Bestsize;
int    Gen;
unsigned int Initseed;
int    Mu_next;
int    Trials;
float  Worst;
float  Worst_current_perf;

