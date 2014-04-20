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
	int blocki=0, blockj = 0;
	int size=4, div = 4;
	int bi, bj;
	
	int i, j;
	switch(M){
		case 32:
		size=8;
		for (blocki=0; blocki<N; blocki+=size){
			for (blockj=0; blockj<M; blockj+=size){
				for (i=0; i<size; i++){
					for (j=0; j<size; j++){
						B[blockj+j][blocki+i] = A[blocki+i][blockj+j];
					}
				}
			}
		}
		break;
		
		default:
		size = 16;
		div = 4;
		for (blocki=0; blocki<N; blocki+=size){
			for (blockj=0; blockj<M; blockj+=size){

				for (bj = 0; bj < size; bj += size/div){
					for (bi = 0; bi < size; bi += size/div){
			
						for (i=0; i<size/div; i++){
							for (j=0; j<size/div; j++){
								B[bj+blockj+j][bi+blocki+i] = A[bi+blocki+i][bj+blockj+j];
							}
						}
					
					}
				}
			
			}
		}
		break;

	}
}

/*
 * Вы можете объявлять дополнительные функции транспонирования ниже. Для примера
 * мы объявили одну такую функцию.
 */

/*
 * trans - Простая функция транспонирования, без оптимизаций по кэшу.
 */
char transpose_magic_desc[] = "Inside blocking";
void transpose_magic(int M, int N, int A[N][M], int B[M][N]){
	int blocki=0, blockj=0;
	int i, j;
	int size=4;
	size=8;
	int k=0;
	
	for(k=0; k<16; k++){
		blocki = 8 * (k%2 + 2*((k/4)%2));
		blockj = 8 * (((k%4)/2) + 2*(k/8));
			for (i = blocki; i < blocki+size; ++i) {
				for (j = blockj; j < blockj+size; ++j) {
					B[j][i] = A[i][j];
				}
			}
		
	}
}

#define SIZE 16
#define DIV 4
char transpose_test_desc[] = "test";
void transpose_test(int M, int N, int A[N][M], int B[M][N]){
	int i, j;
	int blocki, blockj;
	int bi, bj;
	
	for (blockj=0; blockj<M; blockj+=SIZE){
	for (blocki=0; blocki<N; blocki+=SIZE){
		

			for (bj = 0; bj < SIZE; bj += SIZE/DIV){
			for (bi = 0; bi < SIZE; bi += SIZE/DIV){
			
					for (i=0; i<SIZE/DIV; i++){
						for (j=0; j<SIZE/DIV; j++){
							B[bj+blockj+j][bi+blocki+i] = A[bi+blocki+i][bj+blockj+j];
						}
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

