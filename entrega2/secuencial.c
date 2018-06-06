#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int N, *positions;

/* int _yi */
#define ROWS_LOOP(_b, _l, block_) do {            \
                int _yi;                         \
                for (_yi = _b; _yi < _l; _yi++)  \
                { block_; }                      \
} while(0);

#define BOARD_LOOP(_b, _l, block_a_, block_b_) do {                    \
                int _xj, _yi;                                         \
                for (_xj = _b; _xj < _l; _xj++)                       \
                {                                                     \
                        for (_yi = 0; _yi < _l; _yi++) { block_a_; }  \
                        {block_b_;}                                   \
                }                                                     \
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
        ROWS_LOOP(0, N, {
                printf("%d ", positions[_yi]);
        });

        printf("\n");
}

/*
 * Show the full NxN board
 */
void
show_full_board()
{
        BOARD_LOOP(0, N, {
                (positions[_yi] == _xj) ?
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
                if (positions[_yi] == column || (positions[_yi] - _yi) == (column - row) || (positions[_yi] + _yi) == (column + row))
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
        int xj = 0, yi = 0, k = 0;
        int flag = 1;

        N = (int) atoi(argv[1]);

        positions = (int *) malloc(N * sizeof(int));
        memset(positions, ~0, N * sizeof(int));

        timetick = dwalltime();

        for (; 0 <= yi;)
        {
                if (flag) //If no backtrack occured
                        xj = 0;

                for (; xj < N && !check_place(xj, yi); xj++);

                if (xj < N)
                {
                        positions[yi] = xj;
                        if ((yi + 1) == N)
                        {
                                solutions++;
                                xj = N;

                                #ifdef DEBUG
                                show_short_board();
                                show_full_board();
                                #endif
                        }
                        else
                        {
                                yi++;
                                flag = 1;
                        }
                }

                if (xj == N)
                {
                        if (yi-- >= 0) {
                                xj = positions[yi] + 1;
                        }

                        flag = 0;
                }
        };

        printf("Time in seconds %fs\n", dwalltime() - timetick);
        printf("Found %d solutions\n", solutions);

        free(positions);

        return 0;
}
