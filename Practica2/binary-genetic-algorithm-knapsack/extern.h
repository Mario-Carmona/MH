#include "define.h"

extern STRUCTURE *Old, *New, Mejor;
extern float *B, *P;

extern int n_generaciones,
           long_poblacion,
           n_genes;

extern float prob_cruce,
             prob_mutacion,
             semilla,
             M,
             Max_peso;

extern char fich_datos[40];

extern int *sample;
extern unsigned long Seed;
extern unsigned long OrigSeed;

/* Funciones */
extern float (*eval)(TipoCromosoma);
extern float eval_fuerte (TipoCromosoma);
extern void Input (int, char**), Initialize (void), Evaluate (void),
         	Select (void), Cruce_Multipunto (void), Mutacion_Uniforme (void),
            Elitist (void);


extern float  Ave_current_perf;
extern float  Best;
extern float  Best_current_perf;
extern int    Best_guy;
extern int    Bestsize;
extern int    Gen;
extern unsigned int Initseed;
extern int    Mu_next;
extern int    Trials;
extern float  Worst;
extern float  Worst_current_perf;

