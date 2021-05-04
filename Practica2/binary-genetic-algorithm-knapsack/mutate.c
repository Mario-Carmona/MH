#include "extern.h"

void Mutacion_Uniforme (void)
/* Operador de Mutacion Uniforme */

{
 int posiciones, i, j;
 float m;

 posiciones=n_genes*long_poblacion;

 if (prob_mutacion>0)
  while (Mu_next<posiciones)
 	{
    /* Se determina el cromosoma y el gen que corresponden a la posicion que
       se va a mutar */
    i=Mu_next/n_genes;
	 j=Mu_next%n_genes;

    /* Se efectua la mutacion sobre ese gen */
    if (New[i].Gene[j]=='0')
     New[i].Gene[j]='1';
    else
     New[i].Gene[j]='0';

    /* Se marca el cromosoma mutado para su posterior evaluacion */
    New[i].n_e=1;

    /* Se calcula la siguiente posicion a mutar */
	 if (prob_mutacion<1)
     {
      m = Rand();
      Mu_next += ceil (log(m) / log(1.0 - prob_mutacion));
      }
    else
	  Mu_next += 1;
    }

 Mu_next -= posiciones;
 }

