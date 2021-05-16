#include "anneal.h"


int metrop (float de, float t)
/* Implementacion del criterio de Metropolis. Devuelve 1 si el vecino
   es aceptado y 0 en caso contrario. Acepta automaticamente dicho
   vecino si es mejor que la solucion actual (es decir, si
   de = F(Sactual) - F(Snueva) es negativo). En caso contrario, lo
   acepta o rechaza probabilisticamente en funcion del valor de la
   temperatura, t, y la diferencia de costos, de. */

{
 return de < 0.0 || Rand() < exp(-de/t);
 }



void Genera_Vecino (int *sol_act, int *vecino, int n_ciudades, int *pos1, int *pos2)
/* Implementacion del operador de intercambio para la generacion de
   vecinos. Se generan dos posiciones aleatorias en el vector que
   representa la solucion actual y se intercambian sus valores para
   obtener un vecino. */
   
{
 int i, temp;
 
 for (i=0;i<n_ciudades;i++)
  vecino[i]=sol_act[i];
       
 *pos1 = (int) (Rand() * (n_ciudades-1)) + 1;
 do
  *pos2 = (int) (Rand() * (n_ciudades-1)) + 1;
 while (*pos1==*pos2); 
 
 temp=vecino[*pos1];
 vecino[*pos1]=vecino[*pos2];
 vecino[*pos2]=temp;
 }




void anneal (float *x, float *y, int *sol_act, int n_ciudades, 
             int n_enfriamientos, float t, float max_vecinos, float max_exitos)

/* Parametros:
      x,y -> Vectores que almacenan las coordenadas de las 
         	 ciudades en el plano.
      sol_act -> solucion actual del algoritmo (al principio 
            	  contiene la solucion inicial de la que parte).
      n_ciudades -> numero de ciudades del caso del problema.
      t -> Temperatura (al principio contiene el valor de la
           temperatura inicial).
      n_enfriamientos -> Numero maximo de enfriamientos de la temperatura
                     	 a realizar.
      max_vecinos -> Numero maximo de vecinos a generar en cada iteracion 
               	   (para cada valor de la temperatura) antes de enfriarla.
      max_exitos -> Numero de exitos a generar en cada iteracion 
               	  (para cada valor de la temperatura) antes de enfriarla.
                    Se contabiliza un exito por cada vecino aceptado, ya sea
                    directamente o probabilisticamente por el criterio de
                    Metropolis.

   De este modo, el annealing salta a la siguiente iteracion, es decir, enfria
   la temperatura, cuando se hayan generado max_vecinos O se hayan contabilizado
   max_exitos (lo primero que ocurra) para la temperatura actual.
     
   En cambio, el algoritmo finaliza su ejecucion cuando se han realizado
   n_enfriamientos iteraciones (es decir, se ha enfriado la temperatura
   n_enfriamientos veces) O cuando para una temperatura no se ha contabilizado
   ningun exito (lo que ocurra antes). */

{
   int i, j, k, n_exitos, aceptar, pos1, pos2, temp;
   int *vecino, *mejor_sol;
   float coste_act, coste_vecino, coste_mejor, de;

   
   /* Reserva de memoria para las estructuras que almacenan el
      vecino generado para la solucion actual y la mejor solucion
      hallada hasta el momento en la exploracion del espacio */
   vecino = (int *) malloc (n_ciudades*sizeof(int));
   if (vecino==NULL)
    {
     printf("\nERROR DE MEMORIA.\n");
     abort();
     }

   mejor_sol= (int *) malloc (n_ciudades*sizeof(int));
   if (mejor_sol==NULL)
    {
     printf("\nERROR DE MEMORIA.\n");
     abort();
     }
   
   /* Calculo del coste de la solucion inicial e inicializacion de la
      mejor solucion encontrada hasta el momento a la solucion inicial */
   coste_act=Costo (sol_act,x,y,n_ciudades);
   coste_mejor=coste_act;
   for (i=0;i<n_ciudades;i++)
    mejor_sol[i]=sol_act[i];

	/* Bucle principal del SA */
   for (j=1;j<=n_enfriamientos;j++)
    {
     /* Inicializamos el contador de exitos a 0 para la temperatura actual. */
     n_exitos=0;
	  
     /* Bucle interno: generacion de vecinos para la temperatura actual.
        Finaliza cuando se hayan generado max_vecinos o cuando se hayan
        contabilizado max_exitos. */
     for (k=1;k<=max_vecinos;k++)
      {
       /* Obtencion de un vecino de la solucion actual por inversion */
       Genera_Vecino (sol_act, vecino, n_ciudades, &pos1, &pos2);

       /* Estudiamos si el nuevo vecino es aceptado */
       coste_vecino = Costo (vecino,x,y,n_ciudades);
       de = coste_vecino - coste_act;
       aceptar = metrop(de,t);
		 
       /* En el caso en que el nuevo vecino es aceptado: */
       if (aceptar) 
        {
         /* Contamos un nuevo exito */
         n_exitos++;
         
         /* Actualizamos la solucion actual */
         coste_act = coste_vecino;
         temp = sol_act[pos1];
         sol_act[pos1] = sol_act[pos2];
         sol_act[pos2] = temp;
         
         /* Actualizamos la mejor solucion hallada hasta el momento */
         if (coste_act<coste_mejor)
          {
           coste_mejor = coste_act;
           for (i=0;i<n_ciudades;i++)
            mejor_sol[i]=sol_act[i];
           }
         } 
		 
       /* Saltamos a la siguiente iteracion (es decir, enfriamos la temperatura)
          si ya hemos sobrepasado el numero de exitos especificado */
       if (n_exitos >= max_exitos) break;
       }
      
     /* Informe en pantalla */
     printf("\nIteracion: %d.\n",j);
     printf("   T= %10.6f. Numero de exitos: %6d \n",t,n_exitos);
     printf("   Coste Sol. actual = %12.6f. Coste Mejor Sol. = %12.6f \n",coste_act,coste_mejor);

     /* Enfriamiento proporcional de la temperatura */
     t *= TFACTR;
		
     /* Terminamos la ejecucion del algoritmo en el caso en que no se consiga
        ningun exito para una temperatura concreta */
     if (n_exitos == 0)
      break;
     }

   /* Copiamos la mejor solucion encontrada para devolverla */
   for (i=0;i<n_ciudades;i++)
    sol_act[i]=mejor_sol[i];
        
   /* Liberamos la memoria empleada */
   free (vecino);
   free (mejor_sol);
   return;
}

#undef TFACTR
