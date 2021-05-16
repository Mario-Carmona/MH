#include"anneal.h"


float Costo (int solucion[], float x[], float y[], int n_ciudades)
/* Calcula el costo del circuito codificado en solucion */
 {
  #define ALEN(a,b,c,d) sqrt(((b)-(a))*((b)-(a))+((d)-(c))*((d)-(c)))

  int i, i1, i2;
  float costo=0.0;

  for (i=0;i<n_ciudades-1;i++)
   {
    i1=solucion[i];
    i2=solucion[i+1];
    costo += ALEN(x[i1],x[i2],y[i1],y[i2]);
    }
  i1=solucion[n_ciudades-1];
  i2=solucion[0];
  costo += ALEN(x[i1],x[i2],y[i1],y[i2]);
  return (costo);
  }


int main (int argc, char *argv[])
 {
  int n_ciudades;   /* numero de ciudades para el caso actual del problema */
  float *X, *Y;     /* vectores de dimension n_ciudades que almacenan las coor-
                       denadas en el plano de las ciudades del caso actual */
  int *sol_inicial; /* vector de dimension n_ciudades que codifica la solucion
                       actual empleando una representacion de orden */
  int f;
  
  /* Las siguientes variables son los parametros del annealing:
     	t_inicial -> Temperatura inicial.
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
                                        
  int n_enfriamientos;
  float max_vecinos, max_exitos, t_inicial;


  /* Inicializacion del numero de ciudades del caso del problema */
  n_ciudades=50;
  
  /* Inicializacion de los parametros del algoritmo */
  max_vecinos = 100 * n_ciudades;
  max_exitos = 10 * n_ciudades;
  t_inicial=0.5;
  n_enfriamientos=100;

  /* Inicializacion de la semilla para el generador de numeros aleatorios */
  Seed = 111;

  /* Reserva de memoria para las estructuras X, Y y sol_inicial.
     Las funciones empleadas se encuentran en nrutil.c */
  X= (float *) malloc (n_ciudades*sizeof(float));
  if (X==NULL)
   {
    printf("\nERROR DE MEMORIA.\n");
    abort();
    }

  Y= (float *) malloc (n_ciudades*sizeof(float));
  if (Y==NULL)
   {
    printf("\nERROR DE MEMORIA.\n");
    abort();
    }

  sol_inicial= (int *) malloc (n_ciudades*sizeof(int));
  if (sol_inicial==NULL)
   {
    printf("\nERROR DE MEMORIA.\n");
    abort();
    }

  /* Generacion de un caso aleatorio del problema con n_ciudades 
     en el plano [0,1] x [0,1] */
  for (f=0;f<n_ciudades;f++)
   {
    X[f]=Rand();
    Y[f]=Rand();
    }

  /* Generacion de la solucion inicial */
  for (f=0;f<n_ciudades;f++)
   sol_inicial[f]=f;

  /* Llamada a la rutina que implementa el algoritmo SA para el problema
     del Viajante de comercio */
  anneal(X,Y,sol_inicial,n_ciudades,n_enfriamientos,t_inicial,max_vecinos,max_exitos);

  /* Escritura en pantalla de la solucion obtenida, asi como de su costo */
  printf ("\nSolucion final:\n");
  for (f=0;f<n_ciudades;f++)
   printf ("%d ",sol_inicial[f]);
  printf ("\nCoste: %f\n\n",Costo(sol_inicial,X,Y,n_ciudades));

  /* Liberacion de la memoria ocupada por las estructuras empleadas.
     Las funciones empleadas se encuentran en nrutil.c */
  free (X);
  free (Y);
  free (sol_inicial);

  /* Fin del programa */
  return (0);
  }
  
#undef ALEN

