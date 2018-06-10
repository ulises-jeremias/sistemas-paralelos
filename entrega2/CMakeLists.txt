cmake_minimum_required(VERSION 3.0)

project(entrega2)

add_executable(secuencial secuencial.c util.c)
add_executable(mpi mpi.c util.c)

# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_C_COMPILE_FLAGS ${CMAKE_C_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_C_LINK_FLAGS ${CMAKE_C_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(MPI_INCLUDE_PATH)
target_link_libraries(mpi ${MPI_LIBRARIES})

enable_testing()

add_test(NAME secuencia_10 COMMAND secuencial 10)
add_test(NAME secuencia_11 COMMAND secuencial 11)
add_test(NAME secuencia_12 COMMAND secuencial 12)
add_test(NAME secuencia_13 COMMAND secuencial 13)
add_test(NAME secuencia_14 COMMAND secuencial 14)
add_test(NAME secuencia_15 COMMAND secuencial 15)
add_test(NAME secuencia_16 COMMAND secuencial 16)

add_test(NAME mpi_10 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 10)
add_test(NAME mpi_11 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 11)
add_test(NAME mpi_12 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 12)
add_test(NAME mpi_13 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 13)
add_test(NAME mpi_14 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 14)
add_test(NAME mpi_15 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 15)
add_test(NAME mpi_16 COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS} mpi ${MPIEXEC_POSTFLAGS} 16)
