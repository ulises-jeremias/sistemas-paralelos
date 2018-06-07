#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/util.h"

int
main(int argc, char const *argv[])
{
        double timetick;
        int solutions = 0;
        int xj = 0, yi = 0, k = 0;
        int flag = 1;
        int N, *positions;

        N = (int) atoi(argv[1]);

        positions = (int *) malloc(N * sizeof(int));
        memset(positions, ~0, N * sizeof(int));

        timetick = dwalltime();

        for (; 0 <= yi;)
        {
                if (flag) //If no backtrack occured
                        xj = 0;

                for (; xj < N && !check_place(positions, xj, yi); xj++);

                if (xj < N)
                {
                        positions[yi] = xj;
                        if ((yi + 1) == N)
                        {
                                solutions++;
                                xj = N;

                                #ifdef DEBUG
                                show_short_board(N, positions);
                                show_full_board(N, positions);
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
