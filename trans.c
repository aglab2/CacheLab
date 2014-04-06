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
}

/*
 * Вы можете объявлять дополнительные функции транспонирования ниже. Для примера
 * мы объявили одну такую функцию.
 */

/*
 * trans - Простая функция транспонирования, без оптимизаций по кэшу.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
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
    registerTransFunction(trans, trans_desc);

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

