#ifndef _UTILS_H
#define _UTILS_H

#ifdef __STDC__
        #define PREDEF_STANDARD_C89
        #ifdef __STDC_VERSION__
                #if __STDC_VERSION__ >= 199901L
                        #define PREDEF_STANDARD_C99
                        #if __STDC_VERSION__ >= 201112L
                                #define PREDEF_STANDARD_C11
                        #endif
                #endif
        #endif
#endif

#ifdef _MSC_VER
        #define __INLINE __forceinline
#else
        #define __INLINE inline __attribute((always_inline))
#endif

/* Use `extern inline` for C99 or later */
#ifdef PREDEF_STANDARD_C99
        #define __EXTERN_INLINE extern __INLINE
#else
        #undef __INLINE

        #define __INLINE
        #define __EXTERN_INLINE
#endif

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

#ifdef DEBUG
void show_short_board(int N, int *positions);
void show_full_board(int N, int *positions);
#endif

double dwalltime();
int check_place(int *positions, int column, int row);

#endif
