#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "include/util.h"

#ifdef DEBUG

/*
 * Show the queens positions on the board in compressed form,
 * each number represent the occupied column position in the corresponding row.
 */
__EXTERN_INLINE void
show_short_board(int N, int *positions)
{
        ROWS_LOOP(0, N, {
                printf("%d ", positions[_yi]);
        });

        printf("\n");
}

/*
 * Show the full NxN board
 */
__EXTERN_INLINE void
show_full_board(int N, int *positions)
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

__EXTERN_INLINE double
dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv, NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

__EXTERN_INLINE int
check_place(int *positions, int column, int row)
{
        ROWS_LOOP(0, row, {
                if (positions[_yi] == column || (positions[_yi] - _yi) == (column - row) || (positions[_yi] + _yi) == (column + row))
                {
                        return 0;
                }
        });

        return 1;
}
