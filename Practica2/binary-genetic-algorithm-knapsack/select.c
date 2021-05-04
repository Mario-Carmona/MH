#include "extern.h"


void Select()
/* Seleccion basada en el Muestreo Universal Estocastico de Baker.
   Recuerdese que estamos maximizando. */

{
 float expected, factor, perf, ptr, sum, rank_max, rank_min;
 int i, j, k, best, temp;

 /* La seleccion se hara siguiendo el modelo de ranking lineal r_min=0.75 */
 rank_min=0.75;

 /* Asignamos a cada elemento su ranking en la poblacion:
       rank = long_poblacion-1 para el mejor y ranking = 0 para el peor.
    Se usa el campo n_e para almacenar el ranking de cada elemento */
 for (i=0;i<long_poblacion;i++)
  Old[i].n_e=0;

 for (i=0;i<long_poblacion-1;i++)
  {
   /* Se encuentra el mejor elemento de los que restan por ordenar*/
	best=-1;
	perf=0.0;
	for (j=0;j<long_poblacion;j++)
	 {
	  if (Old[j].n_e==0 && (best==-1 || BETTER(Old[j].Perf,perf)))
	   {
		 perf=Old[j].Perf;
		 best=j;
		 }
	  }
	/* Se marca dicho elemento con su ranking */
	Old[best].n_e=long_poblacion-1-i;
   }

 /* Se normaliza para crear las probabilidades */
 rank_max=2.0-rank_min;
 factor=(rank_max-rank_min)/(float)(long_poblacion-1);

 /* Se asigna el numero de copias esperadas a cada cromosoma en funcion de
    la probabilidad de seleccion que tenga asociada. Se procede a la seleccion
    de los cromosomas segun el metodo de Baker */
 k=0;
 ptr=Rand ();
 for (sum=i=0;i<long_poblacion;i++)
  {
   expected=rank_min + Old[i].n_e*factor;
   for (sum+=expected; sum>=ptr; ptr++)
    sample[k++]=i;
   }

 if (k!=long_poblacion)
  {
   /* Aseguro que se seleccione toda la poblacion si falta algun miembro */
   for (;k<long_poblacion;k++)
    sample[k]=Randint (0,long_poblacion);
   /*printf("select: Help! %d samples selected instead of %d\n", k, long_poblacion);
	abort();*/
	}

 /* Se procede a barajar los cromosomas seleccionados para aplicar posterior-
    mente los operadores geneticos */
 for (i=0;i<long_poblacion;i++)
  {
	j=Randint (i,long_poblacion-1);
	temp=sample[j];
	sample[j]=sample[i];
	sample[i]=temp;
	}

 /* Se crea la nueva poblacion a partir de ese baraje  */
 for (i=0;i<long_poblacion;i++)
  {
   k=sample[i];
   for (j=0;j<n_genes;j++)
    New[i].Gene[j]=Old[k].Gene[j];
   New[i].Perf=Old[k].Perf;
   New[i].n_e=0;
   }
}

