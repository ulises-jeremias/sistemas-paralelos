# Programación con memoria compartida

## Introducción

Entrega correspondiente al trabajo de **_Programación con memoria compartida_**.

## Alumnos

-   Ulises Jeremias Cornejo Fandos 13566/7
-   Federico Ramón Gasquez 13598/6

## Ejercicio 1

### Idea general

-   **Secuencial**

Dadas las matrices de nxn._B_ y _L_ se obtienen _b_ y _l_ en iteraciones distintas,
dadas los diferentes espacios de memoria designado para cada una. Luego,
se calculan las matrices _AB_, _BD_ siendo estas `AB = A x B`, `BD = B x D`,
utilizando las mismas iteraciones para resolver la multiplicación por bloques,
permitiendo aprovechar la locación temporal y espacial en la memoria caché.
Luego, se calcula `ABC = AB x C` utilizando nuevamente el método de
multiplicación por bloques. Se resuelven por separado las siguientes operaciones,
`ABC <= l.ABC` y `L <= b.L`. A continuación, se calcula `R = L.BD`, utilizando
el método convencional de multiplicación de una matriz triangular por una matriz
cualquiera. Finalmente, se calcula `R <= ABC + R`.

-   **Pthread**

Se busca que cada hilo realice la misma cantidad de operaciones sobre la
misma cantidad de filas realizando así la misma cantidad de trabajo.
Es por esto que se divide, en cada uno de los bloques iterativos,
la cantidad de iteraciones totales sobre la cantidad de hilos a utilizar.

Sin embargo, esto no se cumple en aquellos bloques en los que se trabaje
con la matriz triangular _L_, dado que no se almacena la misma cantidad de
elementos para todas las filas, por lo tanto la carga de trabajo sobre los
procesadores queda desbalanceada.

-   **OpenMP**

Se engloba el bloque a paralelizar bajo la primitiva `parallel`
y se utiliza la primitiva `for` en los bloques iterativos para que se reparta la
cantidad de trabajo entre los hilos. Para no tener el mismo problema
de desbalance de carga, como en pthread, se usa un `schedule dynamic`
cuando se procesa la matriz triangular.

### Métricas

Las métricas mostradas corresponden a promedios de un conjunto de 5 mediciones.
Los tiempos dispuestos en las siguientes tablas están medidos en segundos.
La computadora utilizada para tomar los tiempos de ejecución de este ejercicio
cuenta con un procesador `AMD fx 8350 eight-core processor - 4.0GHz`.

##### 2 hilos

###### Tiempos

| Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
| :-----------------: | ----------------: | -------------: | ------------: |
|          512        |            2.3829 |          1.348 |          1.56 |
|         1024        |            19.063 |          9.848 |         11.75 |
|         2048        |             154.4 |          80.55 |          98.5 |

###### Speedup

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|          512        |    1.768 |  1.528 |
|         1024        |    1.935 |  1.622 |
|         2048        |    1.917 |  1.568 |

###### Eficiencia

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|          512        |    0.884 |  0.764 |
|         1024        |    0.968 |  0.811 |
|         2048        |    0.959 |  0.784 |

##### 4 hilos

###### Tiempos

| Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
| :-----------------: | ----------------: | -------------: | ------------: |
|          512        |            2.3829 |          0.682 |         0.783 |
|         1024        |            19.063 |          5.255 |         11.75 |
|         2048        |             154.4 |         42.975 |          49.5 |

###### Speedup

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|          512        |    3.494 |  3.043 |
|         1024        |    3.628 |  1.622 |
|         2048        |    3.593 |  3.119 |

###### Eficiencia

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|          512        |    0.874 |  0.761 |
|         1024        |    0.907 |  0.406 |
|         2048        |    0.898 |  0.779 |


## Ejercicio 2

### Idea general

-   **Secuencial**

Se recorre la totalidad del arreglo evaluando la paridad de cada elemento.

-   **Pthread**

Se divide la totalidad de iteraciones entre cada hilo. En cada uno
se define un contador local de pares, que luego se suma a un contador compartido
utilizando exclusión mutua.

-   **OpenMP**

Se utiliza la primitiva `parallel for` para repartir las iteraciones
entre los hilos y la primitiva `reduction` de la suma, _reduction(+)_, de la
variable contador para asegurar que el resultado final de la suma quede
la variable indicada.

### Métricas

Las métricas mostradas corresponden a promedios de un conjunto de 5 mediciones.
Los tiempos dispuestos en las siguientes tablas están medidos en segundos.
La computadora utilizada para tomar los tiempos de ejecución de este ejercicio
fue una de las computadoras de la sala de PC de postgrado.

##### 2 hilos

###### Tiempos

| Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
| :-----------------: | ----------------: | -------------: | ------------: |
|         2^27        |         0.3766822 |      0.3167518 |      0.314521 |
|         2^29        |             1.341 |           0.75 |          0.79 |

###### Speedup

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|         2^27        |    1.189 |  1.198 |
|         2^29        |    1.788 |   1.94 |

###### Eficiencia

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|         2^27        |    0.594 |  0.599 |
|         2^29        |     0.89 |   0.97 |

##### 4 hilos

###### Tiempos

| Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
| :-----------------: | ----------------: | -------------: | ------------: |
|         2^27        |         0.3766822 |       0.367245 |      0.357245 |
|         2^29        |             1.341 |           0.43 |          0.36 |

###### Speedup

| Longitud del vector | Pthreads |  OpenMP |
| :-----------------: | -------: | -----: |
|         2^27        |   1.0257 |  1.0544 |
|         2^29        |     3.11 |   3.725 |

###### Eficiencia

| Longitud del vector | Pthreads | OpenMP |
| :-----------------: | -------: | -----: |
|         2^27        |    0.256 |  0.263 |
|         2^29        |     0.77 |   0.93 |
