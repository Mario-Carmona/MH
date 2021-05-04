#include "extern.h"

void Cruce_Multipunto (void)
/* Operador de cruce multipunto en dos puntos */

{
 int mom, dad, xpoint1, xpoint2, i, j;
 static int last, firstflag=1;
 TipoGen temp;

 if (firstflag)
  {
   last=(prob_cruce*long_poblacion) - 0.5 ;
   firstflag=0;
   }

 for (mom=0;mom<last;mom+=2)
  {
   dad=mom+1;

   /* Se generan los dos puntos de cruce*/
   xpoint1=Randint (0,n_genes-1);
   if (xpoint1!=n_genes-1)
    xpoint2=Randint (xpoint1+1,n_genes-1);
   else
    xpoint2=n_genes-1;

   /* Se cruza las partes contenidas entre ambos puntos */
   for (i=xpoint1;i<=xpoint2;i++)
    {
     temp=New[mom].Gene[i];
     New[mom].Gene[i]=New[dad].Gene[i];
     New[dad].Gene[i]=temp;
     }

   /* Se marcan los dos nuevos cromosomas para su posterior evaluacion */
   New[mom].n_e=1;
   New[dad].n_e=1;
   }
}

