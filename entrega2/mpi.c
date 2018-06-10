#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>
#include "include/util.h"

void master(int N, int rank, int cant_proc);
void slave(int N, int rank, int cant_proc);

int
main(int argc, char *argv[])
{
        int rank, amount;
        int N = atoi(argv[1]);

        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &amount);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if (rank == 0)
        {
                master(N, rank, amount);
        }
        else
        {
                slave(N, rank, amount);
        }

        MPI_Finalize();
        return 0;
}

void
master(int N, int rank, int cant_proc)
{
        int *task_data;
        double total_worktime, worktime = 0., timetick;
        int recv;
        int i, yi = 1, xj = 0, mpi_flag = 0, solutions = 0, total = 0;
        int *positions;

        MPI_Status status;
        MPI_Request req;

        positions = (int*) malloc(N*sizeof(int));
        task_data = (int*) malloc(2*sizeof(int));

        memset(positions, ~0, N * sizeof(int));

        total_worktime = dwalltime();

        task_data[0] = 0;

        for (i = 1; i < cant_proc;)
        {
                for (; xj < N && !check_place(task_data, xj, yi); xj++);

                if (xj < N)
                {
                        task_data[1] = xj;
                        MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        MPI_Send(task_data, 2, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                        xj++;
                        i++;
                }
                else
                {
                        task_data[0]++;
                        xj = 0;

                        if (task_data[0] > N)
                        {
                                break;
                        }
                }
        }

        xj = task_data[1] + 1;

        for (i = 0; i == 0 && task_data[0] < N;)
        {
                for (; xj < N && !check_place((int *) task_data, xj, yi); xj++);

                if (xj < N)
                {
                        task_data[1] = xj;
                        xj++;
                        i = 1;
                }
                else
                {
                        task_data[0]++;
                        xj = 0;
                }
        }

        for(; task_data[0] < N;)
        {
                MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &mpi_flag, &status);

                if (!mpi_flag)
                {
                        positions[0] = task_data[0];
                        positions[1] = task_data[1];

                        timetick = dwalltime();

                        for (yi = 2, xj = 0; 2 <= yi;)
                        {
                                for (; xj < N && !check_place(positions, xj, yi); xj++);

                                if (xj < N)
                                {
                                        positions[yi] = xj;
                                        if ((yi + 1) == N)
                                        {
                                                solutions++;
                                                xj = N;
                                        }
                                        else
                                        {
                                                yi++;
                                                xj = 0;
                                        }
                                }

                                if (xj == N)
                                {
                                        if (yi-- >= 2)
                                        {
                                                xj = positions[yi] + 1;
                                        }
                                }
                        };

                        worktime += dwalltime() - timetick;
                }
                else
                {
                        MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                        MPI_Send(task_data, 2, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                }

                xj = task_data[1] + 1;

                for (i = 0; i == 0 && task_data[0] < N;)
                {
                        for (; xj < N && !check_place((int *) task_data, xj, yi); xj++);

                        if (xj < N)
                        {
                                task_data[1] = xj;
                                xj++;
                                i = 1;
                        }
                        else
                        {
                                task_data[0]++;
                                xj = 0;
                        }
                }

        }

        for (i = 1; i < cant_proc; ++i)
        {
                MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                MPI_Send(task_data, 2, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
        }

        MPI_Reduce(&solutions, &total, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);

        printf("Process: %i, Solutions: %i, Time in seconds %fs\n", rank, solutions, worktime);

        printf("Total time in seconds %fs\n", dwalltime() - total_worktime);
        printf("Found %d solutions\n", total);

        free(positions);
        free(task_data);
}

void
slave(int N, int rank, int cant_proc)
{
        int i, yi, xj, solutions = 0;
        int *positions, *task_data;
        double worktime = 0., timetick;

        MPI_Status status;
        MPI_Request req;

        positions = (int*) malloc(N*sizeof(int));
        task_data = (int*) malloc(2*sizeof(int));

        memset(positions, ~0, N * sizeof(int));

        MPI_Send(&xj, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Recv(task_data, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        for (; task_data[0] < N;)
        {

                positions[0] = task_data[0];
                positions[1] = task_data[1];

                timetick = dwalltime();

                for (yi = 2, xj = 0; 2 <= yi;)
                {
                        for (; xj < N && !check_place(positions, xj, yi); xj++);

                        if (xj < N)
                        {
                                positions[yi] = xj;
                                if ((yi + 1) == N)
                                {
                                        solutions++;
                                        xj = N;
                                }
                                else
                                {
                                        yi++;
                                        xj = 0;
                                }
                        }

                        if (xj == N)
                        {
                                if (yi-- >= 2)
                                {
                                        xj = positions[yi] + 1;
                                }
                        }
                };

                worktime += dwalltime() - timetick;

                MPI_Send(&xj, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                MPI_Recv(task_data, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }

        MPI_Reduce(&solutions, &xj, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        printf("Process: %i, Solutions: %i, Time in seconds %fs\n", rank, solutions, worktime);

        (void) cant_proc;

        free(positions);
        free(task_data);
}
