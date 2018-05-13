# sistemas-paralelos
Entregas de Sistemas Paralelos, Facultad de Informatica, UNLP.

## Entrega 1

Puede verificarse el buen funcionamiento de la primer entrega de la siguiente forma.

```bash
$ git clone https://github.com/ulises-jeremias/sistemas-paralelos.git
$ cd sistemas-paralelos
$ cd entrega1
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ctest
```

Los binarios generados se encuentran en los directorios
`entrega1/build/ejercicio1` y `entrega1/build/ejercicio2` respectivamente
luego de ejecutar el comando `make`. Pueden leerse los `CMakeLists.txt` de
cada ejercicio de la entrega para ver los parametros de ejecucion utilizados
en los tests.
