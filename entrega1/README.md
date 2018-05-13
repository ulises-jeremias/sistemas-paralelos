# Programación con memoria compartida

## Introducción

Entrega correspondiente al trabajo de **Programación con memoria compartida**.

## Alumnos

-   Ulises Jeremias Cornejo Fandos 13566/7
-   Federico Ramón Gasquez

## Ejercicio 1

### Idea general

-   **Secuencial**

Dadas las matrices de nxn._B_ y _L_ se obtienen _b_ y _l_ en iteraciones distintas,
dadas los diferentes espacios de memoria designado para cada una. Luego,
se calculan las matrices _AB_, _BD_ siendo estas _AB = A x B_, _BD = B x D_,
utilizando las mismas iteraciones para resolver la multiplicación por bloques,
permitiendo aprovechar la locación temporal y espacial en la memoria caché.
Luego, se calcula _ABC = AB x C_ utilizando nuevamente el método de
multiplicación por bloques. Se resuelven por separado las siguientes operaciones,
_ABC &lt;- l.ABC_ y _L &lt;- b.L_. A continuación, se calcula _R = L.BD_, utilizando
el método convencional de multiplicación de una matriz triangular por una matriz
cualquiera. Finalmente, se calcula _R &lt;- ABC + R_.

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

-   **2 hilos**

    -   **Tiempos**

    | Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
    | :-----------------: | ----------------: | -------------: | ------------: |
    |         2^27        |         0.3766822 |      0.3167518 |      0.314521 |
    |         2^29        |             1.341 |           0.75 |          0.79 |

    -   **Speedup**

    | Longitud del vector | Pthreads | OpenMP |
    | :-----------------: | -------: | -----: |
    |         2^27        |    1.189 |  1.198 |
    |         2^29        |    1.788 |   1.94 |

    -   **Eficiencia**

    | Longitud del vector | Pthreads | OpenMP |
    | :-----------------: | -------: | -----: |
    |         2^27        |          |        |
    |         2^29        |     0.89 |   0.97 |

-   **4 hilos**

    -   **Tiempos**

    | Longitud del vector | Tiempo secuencial | Tiempo pthread | Tiempo openmp |
    | :-----------------: | ----------------: | -------------: | ------------: |
    |         2^27        |         0.3766822 |       0.367245 |      0.357245 |
    |         2^29        |             1.341 |           0.43 |          0.36 |

    -   **Speedup**

    | Longitud del vector | Pthreads |  OpenMP |
    | :-----------------: | -------: | -----: |
    |         2^27        |   1.0257 |  1.0544 |
    |         2^29        |     3.11 |   3.725 |

    -   **Eficiencia**

    | Longitud del vector | Pthreads | OpenMP |
    | :-----------------: | -------: | -----: |
    |         2^27        |          |        |
    |         2^29        |     0.77 |   0.93 |
