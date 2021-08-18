# Prácticas MH
*Contiene todas la prácticas realizadas en la asignatura MH en la UGR durante el curso 20/21*

Todas las prácticas tienen como base el problema de la máxima diversidad (MDP)

Para calcular la calidad de cada algoritmo desarrollado en las prácticas se mide la desviación de los resultados respecto de las estimaciones existentes para el valor de los óptimos de una serie de casos del problema, y el tiempo de ejecución del algoritmo de una serie de casos del problema.

## [Práctica 1: Técnicas de Búsqueda Local y Algoritmos Greedy](./Practica1)

El objetivo de esta práctica es estudiar el funcionamiento de las Técnicas de Búsqueda Local y de los algoritmos Greedy en la resolución del problema de la máxima diversidad (MDP). Para ello se han desarrollado los siguientes algoritmos:

- **Algoritmo Greedy básico**
- **Algoritmo de Búsqueda Local (BL)**

### Algoritmo Greedy

El algoritmo se basa en la heurística de ir seleccionando los elementos más lejanos a los previamente seleccionados.

### Búsqueda Local

Para este algoritmo será obligatorio emplear la factorización.

## [Práctica 2: Técnicas de Búsqueda basadas en Poblaciones](./Practica2)

El objetivo de esta práctica es estudiar el funcionamiento de las Técnicas de Búsqueda basadas en Poblaciones en la resolución del problema de la máxima diversidad (MDP). Para ello se han desarrollado los siguientes algoritmos:

- **Algoritmos genéticos**
    - Dos variantes generacionales elitistas (AGG)
    - Dos variantes generacionales estacionarias (AGE)
- **Algoritmos meméticos**

### Algoritmo genético

Este algoritmo presenta los siguientes componentes:

- Esquema de evolución
    - Esquema generacional con elitismo
    - Esquema estacionario
- Operador de selección
    - Torneo binario
- Esquema de reemplazamiento (depende del esquema de evolución)
- Operador de cruce
    - Cruce uniforme
    - Cruce de posición
- Operador de mutación
    - Intercambio de genes

### Algoritmo memético

Este algoritmo consistirá en hibridar el algoritmo genético generacional (AGG) que mejor resultado haya proporcionado con la BL.

Se estudiarán tres posibilidades de hibridación:

- Cada 10 generaciones se aplica la BL sobre todos los cromosomas de la población
- Cada 10 generaciones se aplica la BL sobre un subconjunto de cromosomas de la población seleccionado aleatoriamente
- Cada 10 generaciones se aplica la BL sobre el 10% de los mejores cromosomas de la población actual

## [Práctica 3: Búsquedas por Trayectorias](./Practica3)

El objetivo de esta práctica es estudiar el funcionamiento de las Técnicas de Búsqueda basadas en Trayectorias (tanto simples como múltiples) en la resolución del problema de la máxima diversidad (MDP). Para ello se han desarrollado los siguientes algoritmos:

- **Enfriamiento Simulado (ES)**
- **Búsqueda Multiarranque Básica (BMB)**
- **Búsqueda Local Reiterada (ILS)**
- **Hibridación de ILS y ES (ILS-ES)**

## [Práctica Final: Implementación de una metaheurística bioinspirada](./Proyecto_Final)

En mi caso he realizado la implementación de la metaheurística Ant Lion, basada en el compotamiento de las hormigas león durante la captura de las hormigas.
