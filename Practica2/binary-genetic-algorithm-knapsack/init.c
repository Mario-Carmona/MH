#include "extern.h"

void Initialize (void)
/*  Funcion que genera la poblacion inicial de un modo aleatorio */

{
 int i, j;

 /* Calculo del cromosoma en el que se realizara la primera mutacion y la
    posicion en la que se producira esta (se emplea en el modulo mutate.c */
 if (prob_mutacion < 1.0)
  Mu_next = ceil (log(Rand()) / log(1.0 - prob_mutacion));
 else
  Mu_next = 1;

 Trials = 0;
 Gen = 0;

 for (i=0;i<long_poblacion;i++)
  {
   for (j=0;j<n_genes;j++)
    if (Randint(0,1)==0)
     New[i].Gene[j]='0';
    else
     New[i].Gene[j]='1';
   New[i].n_e=1;
   }
 }

