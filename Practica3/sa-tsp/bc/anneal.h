#ifndef ANNEALH
#define ANNEALH

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Definicion del generador de numeros aleatorios */
#define MASK 2147483647
#define PRIME 65539
#define SCALE 0.4656612875e-9

#define Rand()  (( Seed = ( (Seed * PRIME) & MASK) ) * SCALE )


/* Parametro para el enfriamiento de la temperatura por
   descenso proporcional*/
#define TFACTR 0.9


/* Prototipo de la funcion de costo */
float Costo (int *, float *, float *, int);

/* Prototipo de la funcion que implementa el algoritmo Simulated Annealing */
void anneal (float *, float *, int *, int, int, float, float, float);

#endif

