#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int N, *positions;

/* int xi */
#define ROWS_LOOP(_b, _l, block_) do {         \
                int _yj;                       \
                for (_yj = _b; _yj < _l; _yj++)  \
                { block_; }                   \
} while(0);

#define BOARD_LOOP(_b, _l, block_a_, block_b_) do {                 \
                int _xi, _yj;                                        \
                for (_xi = _b; _xi < _l; _xi++)                       \
                {                                                  \
                        for (_yj = 0; _yj < _l; _yj++) { block_a_; }  \
                        {block_b_;}                                \
                }                                                  \
} while(0);

double
dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv, NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

#ifdef DEBUG

/*
 * Show the queens positions on the board in compressed form,
 * each number represent the occupied column position in the corresponding row.
 */
void
show_short_board()
{
        ROWS_LOOP(0, nqueens->size, {
                printf("%d ", positions[_yj]);
        });

        printf("\n");
}

/*
 * Show the full NxN board
 */
void
show_full_board()
{
        BOARD_LOOP(0, nqueens->size, {
                (positions[_yj] == _xi) ?
                printf("Q ") :
                printf(". ");
        }, {
                printf("\n");
        });

        printf("\n");
}

#endif

int
check_place(int column, int row)
{
        ROWS_LOOP(0, row, {
                if (positions[_yj] == column || (positions[_yj] - _yj) == (column - row) || (positions[_yj] + _yj) == (column + row))
                {
                        return 0;
                }
        });

        return 1;
}

int
main(int argc, char const *argv[])
{
        double timetick;
        int solutions = 0;
        int xi = 0, yj = 0, k = 0;
        int flag = 1;

        N = (int) atoi(argv[1]);

        positions = (int *) malloc(N * sizeof(int));
        memset(positions, ~0, N * sizeof(int));

        timetick = dwalltime();

        for (; 0 <= yj;)
        {
                if (flag) //If no backtrack occured
                        xi = 0;

                for (; xi < N && !check_place(xi, yj); xi++);

                if (xi < N)
                {
                        positions[yj] = xi;
                        if ((yj + 1) == N)
                        {
                                solutions++;
                                xi = N;

                                #ifdef DEBUG
                                show_short_board();
                                show_full_board();
                                #endif
                        }
                        else
                        {
                                yj++;
                                flag = 1;
                        }
                }

                if (xi == N)
                {
                        if (yj-- >= 0) {
                                xi = positions[yj] + 1;
                        }

                        flag = 0;
                }
        };

        printf("Time in seconds %fs\n", dwalltime() - timetick);
        printf("Found %d solutions\n", solutions);

        free(positions);

        return 0;
}
