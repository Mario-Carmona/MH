/* PROGRAMA PRINCIPAL */

#include "global.h"


int main (int argc, char *argv[])

{
 STRUCTURE *temp;
 int i;
 float beneficio, peso;

 /* Lectura de los datos de entrada e inicializacion de variables y
    estructuras */
 Input (argc,argv);

 /*printf ("\nPrograma en ejecucion ...\n\n");*/

 /* Generacion de la poblacion inicial */
 Initialize ();

 /* Evaluacion de los individuos de la poblacion inicial */
 Evaluate ();
 Gen++;

 /* Ciclo general del algoritmo genetico */
 do
  {
   /* Intercambio de las poblaciones nueva y antigua */
   temp=Old;
   Old=New;
   New=temp;

   /* Seleccion mediante el metodo de Baker */
   Select ();

   /* Cruce */
   Cruce_Multipunto ();

   /* Mutacion */
   Mutacion_Uniforme ();

   /* Seleccion elitista */
   Elitist ();

   /* Evaluacion de los individuos de la poblacion actual */
   Evaluate ();

   /* Se avanza a la siguiente iteracion. Si es necesario, se graban las
	   estadisticas */
   Gen++;
   /*printf ("Generacion=%d; Mejor=%f\n",Gen-1,New[Best_guy].Perf);*/
   }
 while (Gen<=n_generaciones);

 /* Se imprime la mejor solucion encontrada */
 printf("Solucion encontrada por el AG:\nS=(");
 for (i=0;i<n_genes;i++)
  printf("%c,",New[Best_guy].Gene[i]);
 printf(")\n");
 
 beneficio=0.0; peso=0.0;
 for (i=0;i<n_genes;i++)
  if (New[Best_guy].Gene[i]=='1')
   {
    beneficio+=B[i];
    peso+=P[i];
    }
 if (peso>M)
  printf("La solucion hallada no es factible.\n");
 else
  printf("Beneficio: %f. Peso: %f.\n",beneficio,peso);  
 
 /* Se liberan las estructuras de datos dinamicas */
 free(B);
 free(P);
 
 for (i=0;i<long_poblacion;i++)
  {
   free (New[i].Gene);
   free (Old[i].Gene);
   }
 free (New);
 free (Old);
 free (sample);

 /*printf ("\n\n*****  FIN DEL PROGRAMA AG-MOCHILA *******\n\n");*/
 return (1);
 }

