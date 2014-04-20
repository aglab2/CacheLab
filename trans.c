/*
 * trans.c - Транспонирование матрицы B = A^T
 *
 * Каждая функция транспонирования должна иметь прототип вида
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * Функция транспонирования оценивается по количеству промахов на
 * кэше прямого отображения размера 1KB с размером блока в 32 байта.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - Это функция, содержащая решение, по которому
 *   оценивается часть Б задания. Не меняйте строку описания "Transpose submission",
 *   так как драйвер ищет эту строку, чтобы понять, за какую именно функцию
 *   начислять вам баллы.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int ii, jj, i, j;
    int a1, a2, a3, a4, a5, a6, a7, a8;

    // we borrow (1, 57) -- (4, 64) as a higher level cache
    // diagonal
    for(ii = 0; ii < N; ii += 8) {
        for(jj = 0; jj < M; jj += 8) {
            if (ii == jj && ii != 56) {
                    // up 4 lines, = 8
                    for (i = ii; i < ii + 4; ++i) {
                        a1 = A[i][jj];
                        a2 = A[i][jj+1];
                        a3 = A[i][jj+2];
                        a4 = A[i][jj+3];
                        a5 = A[i][jj+4];
                        a6 = A[i][jj+5];
                        a7 = A[i][jj+6];
                        a8 = A[i][jj+7];

                        B[i][jj] = a1;
                        B[i][jj+1] = a2;
                        B[i][jj+2] = a3;
                        B[i][jj+3] = a4;
                        B[i][jj+4] = a5;
                        B[i][jj+5] = a6;
                        B[i][jj+6] = a7;
                        B[i][jj+7] = a8;

                        // save to cache, first time = 4 miss, else = 0 miss.
                        B[i - ii][56] = a5;
                        B[i - ii][57] = a6;
                        B[i - ii][58] = a7;
                        B[i - ii][59] = a8;
                    }
                    // up left
                    for (i = ii; i < ii + 4; ++i) {
                        for (j = i; j < jj + 4; ++j) {
                            a1 = B[i][j];
                            B[i][j] = B[j][i];
                            B[j][i] = a1;
                        }
                    }

                    // down 4 lines, = 8
                    for (i = ii + 4; i < ii + 8; ++i) {
                        a1 = A[i][jj];
                        a2 = A[i][jj+1];
                        a3 = A[i][jj+2];
                        a4 = A[i][jj+3];
                        a5 = A[i][jj+4];
                        a6 = A[i][jj+5];
                        a7 = A[i][jj+6];
                        a8 = A[i][jj+7];

                        B[i][jj] = a1;
                        B[i][jj+1] = a2;
                        B[i][jj+2] = a3;
                        B[i][jj+3] = a4;
                        B[i][jj+4] = a5;
                        B[i][jj+5] = a6;
                        B[i][jj+6] = a7;
                        B[i][jj+7] = a8;

                        // save to cache, first = 4 miss, else = 0 miss
                        B[i - ii - 4][60] = a1;
                        B[i - ii - 4][61] = a2;
                        B[i - ii - 4][62] = a3;
                        B[i - ii - 4][63] = a4;
                    }

                    // down right
                    for (i = ii + 4; i < ii + 8; ++i) {
                        for (j = i; j < jj + 8; ++j) {
                            a1 = B[i][j];
                            B[i][j] = B[j][i];
                            B[j][i] = a1;
                        }
                    }

                    // transpose 2 matrces in cache
                    for (i = 0; i < 4; ++i) {
                        for (j = 56 + i; j < 60; ++j) {
                            a1 = B[i][j];
                            B[i][j] = B[j - 56][i + 56];
                            B[j - 56][i + 56] = a1;
                        }
                    }

                    for (i = 0; i < 4; ++i) {
                        for (j = 60 + i; j < 64; ++j) {
                            a1 = B[i][j];
                            B[i][j] = B[j - 60][i + 60];
                            B[j - 60][i + 60] = a1;
                        }
                    }

                    // update to array B.
                    // left ---> down left
                    for (i = ii + 4; i < ii + 8; ++i) {
                        for (j = jj; j < jj + 4; ++j) {
                            B[i][j] = B[i - ii - 4][56 + j - jj];
                        }
                    }
                    // right --> up right
                    for (i = ii; i < ii + 4; ++i) {
                        for (j = jj + 4; j < jj + 8; ++j) {
                            B[i][j] = B[i - ii][60 + j - jj - 4];
                        }
                    }
            }
        }
    }

    for(ii = 0; ii < N; ii += 8) {
        for(jj = 0; jj < M; jj += 8) {
            if ( ii != jj ) {
                // for *up right* and *down left*
                // up 4 lines
                for (i = 0; i < 4; ++i) {
                    a1 = A[ii + i][jj];
                    a2 = A[ii + i][jj+1];
                    a3 = A[ii + i][jj+2];
                    a4 = A[ii + i][jj+3];
                    a5 = A[ii + i][jj+4];
                    a6 = A[ii + i][jj+5];
                    a7 = A[ii + i][jj+6];
                    a8 = A[ii + i][jj+7];

                    B[jj + i][ii] = a1;
                    B[jj + i][ii+1] = a2;
                    B[jj + i][ii+2] = a3;
                    B[jj + i][ii+3] = a4;
                    B[jj + i][ii+4] = a5;
                    B[jj + i][ii+5] = a6;
                    B[jj + i][ii+6] = a7;
                    B[jj + i][ii+7] = a8;
                }
                for (i = jj; i < jj + 4; ++i) {
                    for (j = ii + i - jj; j < ii + 4; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[jj + j - ii][ii + i - jj];
                        B[jj + j - ii][ii + i - jj] = a1;
                    }
                } // transpose upper left
                // down 4 lines
                for (i = 0; i < 4; ++i) {
                    a1 = B[jj + i][ii + 4];
                    a2 = B[jj + i][ii + 5];
                    a3 = B[jj + i][ii + 6];
                    a4 = B[jj + i][ii + 7];

                    a5 = A[ii + 4 + i][jj];
                    a6 = A[ii + 4 + i][jj+1];
                    a7 = A[ii + 4 + i][jj+2];
                    a8 = A[ii + 4 + i][jj+3]; // = 4 miss

                    B[jj + i][ii + 4] = a5;
                    B[jj + i][ii + 5] = a6;
                    B[jj + i][ii + 6] = a7;
                    B[jj + i][ii + 7] = a8; // = 0 miss

                    B[jj + 4 + i][ii] = a1;
                    B[jj + 4 + i][ii+1] = a2;
                    B[jj + 4 + i][ii+2] = a3;
                    B[jj + 4 + i][ii+3] = a4; // = 4 miss

                    a1 = A[ii + 4 + i][jj + 4];
                    a2 = A[ii + 4 + i][jj + 5];
                    a3 = A[ii + 4 + i][jj + 6];
                    a4 = A[ii + 4 + i][jj + 7]; // = 0 miss

                    B[jj + 4 + i][ii + 4] = a1;
                    B[jj + 4 + i][ii + 5] = a2;
                    B[jj + 4 + i][ii + 6] = a3;
                    B[jj + 4 + i][ii + 7] = a4; // = 0 miss
                }

                for (i = jj + 4; i < jj + 8; ++i) {
                    for (j = ii + i - jj; j < ii + 8; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[jj + j - ii][ii + i - jj];
                        B[jj + j - ii][ii + i - jj] = a1;
                    }
                } // transpose down right, = 0 miss

                for (i = jj + 4; i < jj + 8; ++i) {
                    for (j = ii + i - jj - 4; j < ii + 4; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[jj + 4 + j - ii][ii + i - jj - 4];
                        B[jj + 4 + j - ii][ii + i - jj - 4] = a1;
                    }
                } // down left, = 0 miss

                for (i = jj; i < jj + 4; ++i) {
                    for (j = ii + 4 + i - jj; j < ii + 8; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[jj + j - ii - 4][ii + 4 + i - jj];
                        B[jj + j - ii - 4][ii + 4 + i - jj] = a1;
                    }
                } // up right, = 4 miss
            } else if (ii == jj && ii == 56) {
                // up 4 lines, = 8
                for (i = ii; i < ii + 4; ++i) {
                    a1 = A[i][jj];
                    a2 = A[i][jj+1];
                    a3 = A[i][jj+2];
                    a4 = A[i][jj+3];
                    a5 = A[i][jj+4];
                    a6 = A[i][jj+5];
                    a7 = A[i][jj+6];
                    a8 = A[i][jj+7];

                    B[i][jj] = a1;
                    B[i][jj+1] = a2;
                    B[i][jj+2] = a3;
                    B[i][jj+3] = a4;
                    B[i][jj+4] = a5;
                    B[i][jj+5] = a6;
                    B[i][jj+6] = a7;
                    B[i][jj+7] = a8;
                }
                for (i = ii; i < ii + 4; ++i) {
                    for (j = i; j < jj + 4; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[j][i];
                        B[j][i] = a1;
                    }
                }

                for (i = ii; i < ii + 4; ++i) {
                    for (j = i + 4; j < jj + 8; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[j - 4][i + 4];
                        B[j - 4][i + 4] = a1;
                    }
                }

                // down 4 lines, = 8
                for (i = ii + 4; i < ii + 8; ++i) {
                    a1 = A[i][jj];
                    a2 = A[i][jj+1];
                    a3 = A[i][jj+2];
                    a4 = A[i][jj+3];
                    a5 = A[i][jj+4];
                    a6 = A[i][jj+5];
                    a7 = A[i][jj+6];
                    a8 = A[i][jj+7];

                    B[i][jj] = a1;
                    B[i][jj+1] = a2;
                    B[i][jj+2] = a3;
                    B[i][jj+3] = a4;
                    B[i][jj+4] = a5;
                    B[i][jj+5] = a6;
                    B[i][jj+6] = a7;
                    B[i][jj+7] = a8;
                }

                for (i = ii + 4; i < ii + 8; ++i) {
                    for (j = i; j < jj + 8; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[j][i];
                        B[j][i] = a1;
                    }
                }

                for (i = ii + 4; i < ii + 8; ++i) {
                    for (j = jj + i - ii - 4; j < jj + 4; ++j) {
                        a1 = B[i][j];
                        B[i][j] = B[j + 4][i - 4];
                        B[j + 4][i - 4] = a1;
                    }
                }

                // exchange, = 8
                for (i = 0; i < 4; ++i) {
                    a5 = B[ii + 4 + i][jj];
                    a6 = B[ii + 4 + i][jj+1];
                    a7 = B[ii + 4 + i][jj+2];
                    a8 = B[ii + 4 + i][jj+3];
                    a1 = B[ii + i][jj+4];
                    a2 = B[ii + i][jj+5];
                    a3 = B[ii + i][jj+6];
                    a4 = B[ii + i][jj+7];

                    B[ii + i][jj+4] = a5;
                    B[ii + i][jj+5] = a6;
                    B[ii + i][jj+6] = a7;
                    B[ii + i][jj+7] = a8;
                    B[ii + 4 + i][jj] = a1;
                    B[ii + 4 + i][jj+1] = a2;
                    B[ii + 4 + i][jj+2] = a3;
                    B[ii + 4 + i][jj+3] = a4;
                }
            }
        }
    }

}

/*
 * Вы можете объявлять дополнительные функции транспонирования ниже. Для примера
 * мы объявили одну такую функцию.
 */



#define SIZE 8
char transpose_test_desc[] = "test";
void transpose_test(int M, int N, int A[N][M], int B[M][N]){
	int i, j;
	int blocki, blockj;
	int a0, a1, a2, a3, a4, a5, a6, a7;

	if (M == 32 && N == 32){

		for (blocki=0; blocki<N; blocki+=SIZE){	
		for (blockj=0; blockj<M; blockj+=SIZE){

			for (i=0; i<SIZE; i++){
				a0 = A[blocki+i][blockj+0];
				a1 = A[blocki+i][blockj+1];
				a2 = A[blocki+i][blockj+2];
				a3 = A[blocki+i][blockj+3];
				a4 = A[blocki+i][blockj+4];
				a5 = A[blocki+i][blockj+5];
				a6 = A[blocki+i][blockj+6];
				a7 = A[blocki+i][blockj+7];
			
				B[blockj+0][blocki+i] = a0;
				B[blockj+1][blocki+i] = a1;
				B[blockj+2][blocki+i] = a2;
				B[blockj+3][blocki+i] = a3;
				B[blockj+4][blocki+i] = a4;
				B[blockj+5][blocki+i] = a5;
				B[blockj+6][blocki+i] = a6;
				B[blockj+7][blocki+i] = a7;
			}
		}		
		}
	}
	
	if (M == 64 && N == 64){
		for (blockj=0; blockj<M; blockj+=SIZE){
		for (blocki=0; blocki<N; blocki+=SIZE){	
			for(i=0; i<8; i++){
			    a0 = A[blocki+i][blockj+0];
                a1 = A[blocki+i][blockj+1];
                a2 = A[blocki+i][blockj+2];
                a3 = A[blocki+i][blockj+3];
                if(!i){
					a4 = A[blocki][blockj+4];
					a5 = A[blocki][blockj+5];
                    a6 = A[blocki][blockj+6];
					a7 = A[blocki][blockj+7];
				}
                B[blockj+0][blocki+i] = a0;
                B[blockj+1][blocki+i] = a1;
                B[blockj+2][blocki+i] = a2;
                B[blockj+3][blocki+i] = a3;
			}
			for(i = 7; i > 0; i--){
				a0 = A[blocki+i][blockj+4];
				a1 = A[blocki+i][blockj+5];
				a2 = A[blocki+i][blockj+6];
				a3 = A[blocki+i][blockj+7];
				B[blockj+4][blocki+i] = a0;
				B[blockj+5][blocki+i] = a1;
				B[blockj+6][blocki+i] = a2;
				B[blockj+7][blocki+i] = a3;
			}

            B[blockj+4][blocki] = a4;
            B[blockj+5][blocki] = a5;
            B[blockj+6][blocki] = a6;
            B[blockj+7][blocki] = a7;
		}
		}
	}
	
	if (M == 61 && N == 67){
		for (blocki=0; blocki<N; blocki+=SIZE){	
		for (blockj=0; blockj<M; blockj+=SIZE){
			for (j=blockj; j<blockj+SIZE && j<M; j++){
			for (i=blocki; i<blocki+SIZE && i<N; i++){
				B[j][i] = A[i][j];
			}
			}			
		}
		}
	}
}


/*
 * registerFunctions - Эта функция регистрирует ваши функции транспонирования.
 *   Во время исполнения драйвер оценит каждую из зарегистрированных функций
 *   и напишет отчёт об их работе. Это удобный способ экспериментировать с различными
 *   стратегиями траспонирования.
 */
void registerFunctions()
{
    /* Зарегистрировать ваше окончательное решение */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Зарегистрировать любые другие функции транспонирования */
    //registerTransFunction(transpose_magic, transpose_magic_desc);
    registerTransFunction(transpose_test, transpose_test_desc);
}


/*
 * is_transpose - Эта вспомогательная функция проверяет, что B является
 *   транспонированной версией A. Вы можете проверить корректность вашей
 *   функции транспонирования, перед возвратом вызвав is_transpose.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

