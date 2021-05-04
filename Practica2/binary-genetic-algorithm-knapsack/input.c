#include "extern.h"

#define FORMATO_ENT "Fichero de datos = %s \
Numero de Generaciones = %d \
Longitud de la Poblacion = %d \
Probabilidad de Cruce = %f \
Probabilidad de Mutacion = %f \
Semilla = %f"

#define VAR_ENT  fich_datos,&n_generaciones,&long_poblacion,\
                 &prob_cruce,&prob_mutacion,&semilla



void Input (int argc, char *argv[])

/* Funcion que efectua la lectura de los datos de entrada desde el fichero
   entradas */

{
 int i, j;
 FILE *fp;
 char fichero_ent[100], fichero_sal[100];

 /* Se procede a la lectura de los datos de entrada */
 sprintf (fichero_ent,"%s",argv[1]);
 if ((fp=fopen (fichero_ent,"r")) == NULL)
  {
   printf ("No se puede abrir el fichero de datos de entrada %s",fichero_ent);
   abort ();
   }
 fscanf (fp,FORMATO_ENT,VAR_ENT);
 fclose (fp);

 Seed=(unsigned long) semilla;
 
 /* Asignamos a eval la funcion de adaptacion empleada. Podriamos tener varias y
    escogerlas en funcion de parametros de entrada */
 eval=eval_fuerte;
 
 /* Lectura del fichero de datos */
 if ((fp=fopen (fich_datos,"r")) == NULL)
  {
   printf ("No se puede abrir el fichero con la instancia del problema %s",fich_datos);
   abort ();
   }

 /* El numero de genes es el numero de reglas, cada gen valdra 0 o 1 depen-
    diendo de si dicha regla pertenece o no a la base final */
 fscanf (fp,"%f\n",&M);
 fscanf (fp,"%d\n",&n_genes);
 
 /* Reserva de memoria para los vectores en los que se almacena el peso y
    el beneficio de cada objeto */
 B = (float *) calloc ((unsigned) n_genes,sizeof(float));
 if (B == NULL)
  {
   printf("Error en Calloc 1");
   abort();
   }

 P = (float *) calloc ((unsigned) n_genes,sizeof(float));
 if (P == NULL)
  {
   printf("Error en Calloc 1");
   abort();
   }

 /* Lectura de los datos comentados para todos los objetos */
 for (i=0;i<n_genes;i++)
  fscanf (fp,"%f %f",&B[i],&P[i]);

 fclose (fp);
 
 /* Calculo de la probabilidad de mutacion  */
 prob_mutacion=prob_mutacion/(float)n_genes;

 /* Calculo del peso de todos los objetos posibles */
 Max_peso=0.0;
 for (i=0;i<n_genes;i++)
  Max_peso+=P[i];

 /* Reserva de memoria para las poblaciones */
 Old = (STRUCTURE *) calloc ((unsigned) long_poblacion, sizeof(STRUCTURE));
 New = (STRUCTURE *) calloc ((unsigned) long_poblacion, sizeof(STRUCTURE));
 if (Old == NULL || New == NULL)
  {
   printf("Error en Calloc 3");
   abort();
   }

 for (i=0; i<long_poblacion; i++)
  {
	Old[i].Gene = (TipoCromosoma) calloc ((unsigned)n_genes,sizeof(TipoGen));
	New[i].Gene = (TipoCromosoma) calloc ((unsigned)n_genes,sizeof(TipoGen));
   if (Old[i].Gene == NULL || New[i].Gene == NULL)
    {
     printf("Error en Calloc 4");
     abort();
     }
   }

 /* Reserva de memoria para el vector en el que se almacena el numero de
    copias esperadas de cada cromosoma en la seleccion de Baker */
 sample = (int *) calloc ((unsigned) long_poblacion,sizeof(int));
 if (sample == NULL)
  {
   printf("Error en Calloc 5");
   abort();
   }

 }

