#include "extern.h"

void Evaluate()
/* Funcion fitness. Obtiene el fitness del mejor y peor cromosoma, asi como
   el valor medio del de la poblacion actual */

{
 float performance;
 int i, j;

 /*Ave_current_perf=0;*/

 for (i=0;i<long_poblacion;i++)
  {
   if (New[i].n_e)   /* Si no esta evaluado, se evalua */
    {
     New[i].Perf=eval (New[i].Gene);
     performance=New[i].Perf;
	  New[i].n_e=0;
     Trials++;       /* Aumenta en 1 el numero de cromosomas evaluados */

     /* Se inicializan los valores de las variables Best y Worst que almacenan
	la mejor y la peor adaptacion obtenidas a lo largo de toda la ejecu-
	cion del genetico */
     /*if (Trials==1)
      Best=Worst=performance;

      Obtencion del mejor individuo de toda la ejecucion */
     /*if (BETTER (performance,Best))
      Best=performance;

      Obtencion del peor individuo de toda la ejecucion */
     /*if (BETTER (Worst,performance))
      Worst=performance;*/
     }
   else
    performance=New[i].Perf;

   /* Calculo de la posicion del mejor individuo en la poblacion actual */
   if (i==0)
    {
	  Best_current_perf=performance;
	  Best_guy=0;
     }
   else
    if (BETTER (performance,Best_current_perf))
     {
      Best_current_perf=performance;
      Best_guy=i;
      }

   /* Calculo de la media de los fitness de los individuos de la poblacion
      actual */
   /*Ave_current_perf+=performance/long_poblacion;*/
   }
}

