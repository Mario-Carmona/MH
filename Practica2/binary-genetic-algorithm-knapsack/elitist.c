#include "extern.h"


void Elitist (void)
/* Seleccion elitista */

{
 int i, k, found;

 /* Se estudia a ver si el mejor cromosoma de la poblacion anterior ha sido
    seleccionado para formar parte de la nueva */
 for (i=0, found=0; i<long_poblacion && (!found); i++)
  for (k=0, found=1; (k<n_genes) && (found); k++)
   found = (New[i].Gene[k] == Old[Best_guy].Gene[k]);

 /* Si el mejor cromosoma no ha perdurado, se sustituye el ultimo de la
    poblacion por este. */
 if (!found)
  {
   for (k=0; k<n_genes; k++)
	 New[long_poblacion-1].Gene[k]=Old[Best_guy].Gene[k];
	New[long_poblacion-1].Perf=Old[Best_guy].Perf;
   New[long_poblacion-1].n_e=0;
	}
}

