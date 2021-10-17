#include <stdio.h>
#include <omp.h>
#include <chrono>
#include <time.h>
#include <stdlib.h>

using namespace std;


void print(long long** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lld ", matrix[i][j]);
        }
        printf("\n");
    }
}

void mulLinear(long long** res, long long** m1, long long** m2, int r1, int c1, int r2, int c2) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                res[i][j] += m1[i][k]*m2[k][j];
            }
        }
    }
}

void mulParallel(long long** res, long long** m1, long long** m2, int r1, int c1, int r2, int c2) {
    #pragma omp parallel for
    for (int i = 0; i < r1; i++) {
        printf("external, i = %d; tid=%d\n", i, omp_get_thread_num());
        for (int j = 0; j < c2; j++) {
            printf("inner, i = %d, j = %d; tid=%d\n", i, j, omp_get_thread_num());
            for (int k = 0; k < c1; k++) {   
                // printf("double inner, i = %d, j = %d, k = %d, tid=%d\n", i, j, k, omp_get_thread_num());            
                res[i][j] += m1[i][k]*m2[k][j];
            }
        }
    }
}

void nulify(long long** m, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            m[i][j] = 0;
        }
    }
}


int main() {
    srand(time(nullptr));
    int rows1=3, cols1=3;
    int rows2=3, cols2=3;

    if (cols1 != rows2) {
        printf("Error\n");
        return -1;
    }
    printf("Matrix 1 size: %d x %d\n", rows1, cols1);
    printf("Matrix 2 size: %d x %d\n", rows2, cols2);

    long long** matrix1 = new long long*[rows1];
    for (int i = 0; i < rows1; i++) {
        matrix1[i] = new long long[cols1];
        for (int j = 0; j < cols1; j++) {
            matrix1[i][j] = rand() % 10;
        }
    }
    long long** matrix2 = new long long*[rows2];
    for (int i = 0; i < rows2; i++) {
        matrix2[i] = new long long[cols2];
        for (int j = 0; j < cols2; j++) {
            matrix2[i][j] = rand() % 10;
        }
    }
    printf("\n");

    // print(matrix1, rows1, cols1);
    // printf("\n");
    // print(matrix2, rows2, cols2);
    // printf("\n");

    int resRow = rows1, resCol = cols2;
    long long** resMatrix = new long long*[resRow];
    for (int i = 0; i < resRow; i++) {
        resMatrix[i] = new long long[resCol];
    }
    nulify(resMatrix, resRow, resCol);

    printf("Linear multiplication\n");
    auto start = chrono::high_resolution_clock::now();
    mulLinear(resMatrix, matrix1, matrix2, rows1, cols1, rows2, cols2);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    printf("Elapsed time %f s\n", duration.count());

    // print(resMatrix, resRow, resCol);
    // printf("\n");

    nulify(resMatrix, resRow, resCol);

    printf("Parallel multiplication\n");
    start = chrono::high_resolution_clock::now();
    mulParallel(resMatrix, matrix1, matrix2, rows1, cols1, rows2, cols2);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    printf("Elapsed time %f s\n", duration.count());

    // print(resMatrix, resRow, resCol);
    // printf("\n");
}