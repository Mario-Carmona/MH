#include "extern.h"

/* -------------------------------------------------------------------------
                               FUNCION FITNESS
  ------------------------------------------------------------------------- */

float eval_fuerte (TipoCromosoma cromosoma)
/* Funcion fitness basada en la penalizacion fuerte */

{
 int i;
 float beneficio, peso, dist, diff, penal;

 /* Se calcula el beneficio y el peso de la solucion que codifica el
    cromosoma actual */
 beneficio=0.0; peso=0.0;
 for (i=0;i<n_genes;i++)
  if (cromosoma[i]=='1')
   {
    beneficio+=B[i];
    peso+=P[i];
    }
 
 /* Se calcula la penalizacion asociada al cromosoma en caso de ser
    necesario penalizarlo */
    
 if (peso<=M)
  penal=0.0;
 else 
  {
   dist=M-peso;
   diff=abs(Max_peso-M);
   if (diff>M)
    diff=M;
   penal=beneficio*(dist*dist/diff);
   }
 return (beneficio-penal);
 }


