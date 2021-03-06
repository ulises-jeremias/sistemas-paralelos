#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <matrix.h>

#define PRINTABLE_TIME(_x) ((long double) (clock() - _x)/CLOCKS_PER_SEC)

int
main(int argc, char const *argv[])
{
        size_t I, J, K, k, N, r, n, matrix_size, matrix_tri_size, block_size, NUM_THREADS;
        size_t desp1, desp2, desp3, desp, tmp1, gauss;
        double l = 0.0, b = 0.0, *A, *B, *C, *D, *L, *AB, *BD, *ABC, *R;
        double expected, result;
        clock_t cl;

        if (argc < 4)
        {
                printf("\n Falta un parametro ");
                printf("\n 1. Cantidad de bloques por dimension ");
                printf("\n 2. Dimension de cada bloque ");
                printf("\n 3. Numero de hilos");
                exit(-1);
        }

        n = atoi(argv[1]);
        r = atoi(argv[2]);
        NUM_THREADS = atoi(argv[3]);

        omp_set_num_threads(NUM_THREADS);

        N = n*r;
        matrix_size = N*N;
        matrix_tri_size = N*(N+1)/2;
        block_size = r*r;

        matrix_set_blocks_count(n);
        matrix_set_block_size(r);

        A = (double *) malloc(sizeof(double)*matrix_size);
        B = (double *) malloc(sizeof(double)*matrix_size);
        C = (double *) malloc(sizeof(double)*matrix_size);
        D = (double *) malloc(sizeof(double)*matrix_size);
        L = (double *) malloc(sizeof(double)*matrix_size);
        R = (double *) malloc(sizeof(double)*matrix_size);

        AB = (double *) calloc(matrix_size, sizeof(double));
        BD = (double *) calloc(matrix_size, sizeof(double));
        ABC = (double *) calloc(matrix_size, sizeof(double));

        matrix_init(A);
        matrix_init(B);
        matrix_init(C);
        matrix_init(D);
        matrix_triangular_init(L);

        #ifdef DEBUG
        matrix_printf(A);
        matrix_printf(B);
        matrix_printf(C);
        matrix_printf(D);
        matrix_triangular_printf(L);
        #endif

        cl = clock();

        #pragma omp parallel
        {
                size_t i, j;

                #pragma omp for reduction(+:b)
                for (i = 0; i < matrix_size; i++) b += B[i];

                #pragma omp for reduction(+:l)
                for (i = 0; i < matrix_tri_size; i++) l += L[i];

                #pragma omp single
                {
                        b /= matrix_size;
                        l /= matrix_size;
                }

                /*
                 * AB = A*B
                 * BD = B*D
                 *
                 */
                #pragma omp for private(desp1, desp2, desp3, desp, K, k, J)
                for (I = 0; I < n; I++)
                {
                        for (J = 0; J < n; J++)
                        {
                                desp3 = (I*n+J)*block_size;

                                for (K=0; K < n; K++)
                                {
                                        desp1 = (I*n+K)*block_size;
                                        desp2 = (K*n+J)*block_size;

                                        for (i = 0; i < r; i++)
                                        {
                                                for (j = 0; j < r; j++)
                                                {
                                                        desp = desp3 + i*r + j;

                                                        for (k = 0; k < r; k++)
                                                        {
                                                                AB[desp] += A[desp1 + i*r+k]*B[desp2 + k*r+j];
                                                                BD[desp] += B[desp1 + i*r+k]*D[desp2 + k*r+j];
                                                        }
                                                }
                                        }
                                }
                        }
                }

                /*
                 * ABC = AB*C
                 *
                 */
                #pragma omp for private(desp1, desp2, desp3, desp, K, k, J)
                for (I = 0; I < n; I++)
                {
                        for (J = 0; J < n; J++)
                        {
                                desp3 = (I*n+J)*block_size;

                                for (K = 0; K < n; K++)
                                {
                                        desp1 = (I*n+K)*block_size;
                                        desp2 = (K*n+J)*block_size;

                                        for (i = 0; i < r; i++)
                                        {
                                                for (j = 0; j < r; j++)
                                                {
                                                        desp = desp3 + i*r + j;

                                                        for (k = 0; k < r; k++)
                                                        {
                                                                ABC[desp] += AB[desp1 + i*r+k]*C[desp2 + k*r+j];
                                                        }
                                                }
                                        }
                                }
                        }
                }

                #pragma omp for
                for (i = 0; i < matrix_size; i++) ABC[i] *= l;

                #pragma omp for
                for (i = 0; i < matrix_tri_size; i++) L[i] *= b;

                #pragma omp for private(j, k, tmp1) schedule(dynamic)
                for(i = 0; i < N; i++)
                {
                        tmp1 = i + 1;
                        gauss = i*(i+1)/2;

                        for(j = 0; j < N; j++)
                        {
                                for(k = 0; k < tmp1; k++)
                                {
                                        R[i*N + j] += L[gauss + k]*BD[k + j*N];
                                }
                        }
                }

                #pragma omp for
                for (i = 0; i < matrix_size; i++) R[i] += ABC[i];
        }

        printf("TIME: %Lf\n", PRINTABLE_TIME(cl));

        expected = matrix_size*(l + 1);
        result = R[matrix_size - 1];

        #ifdef DEBUG
        matrix_printf(AB);
        matrix_printf(BD);
        matrix_printf(ABC);
        matrix_printf(R);

        printf("Results: \n\n");
        printf("  B average: %f\n", b);
        printf("  L average: %f\n", l);
        printf("  Expected result: %f\n", expected);
        printf("  Matrix result: %f\n", result);
        #endif

        if (expected != result) {
                printf("\nFailure\n");
                exit(-1);
        }

        printf("\nSuccess!\n");

        free(A);
        free(B);
        free(D);
        free(L);
        free(R);
        free(AB);
        free(BD);
        free(ABC);

        return 0;
}
