# Programación con memoria compartida

## Introducción

Entrega correspondiente al trabajo de __Programación con memoria compartida__.

## Alumnos

-   Ulises Jeremias Cornejo Fandos 13566/7
-   Federico Ramón Gasquez

## Ejercicio 1

### Idea general

-   **Secuencial**

Dadas las matrices de nxn, _B_ y _L_ se obtienen _b_ y _l_ en iteraciones distintas,
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
