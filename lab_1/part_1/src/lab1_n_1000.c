#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define SIZE 1000

int main() {
    printf("N is %d\n\n", SIZE);
    int **A, **B, **C;

    A = (int **)malloc(SIZE * sizeof(int *));
    B = (int **)malloc(SIZE * sizeof(int *));
    C = (int **)malloc(SIZE * sizeof(int *));
    
    for (int row = 0; row < SIZE; row++) {
        A[row] = (int *)malloc(SIZE * sizeof(int));
        B[row] = (int *)malloc(SIZE * sizeof(int));
        C[row] = (int *)malloc(SIZE * sizeof(int));
    }

    for (int n = 0; n < SIZE; n++) {
        for (int m = 0; m < SIZE; m++) {
            A[n][m] = rand() % 100;
            B[n][m] = rand() % 100;
        }
    }

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    clock_t begin = clock();
    // Matrix multiplication (m, j, k)
    for (int m = 0; m < SIZE; m++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time spent for order (m, j, k) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");
    time_spent = 0.0;

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    begin = clock();
    // Matrix multiplication (j, m, k)
    for (int j = 0; j < SIZE; j++) {
        for (int m = 0; m < SIZE; m++) {
            for (int k = 0; k < SIZE; k++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent for order (j, m, k) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");
    time_spent = 0.0;

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    begin = clock();
    // Matrix multiplication (m, k, j)
    for (int m = 0; m < SIZE; m++) {
        for (int k = 0; k < SIZE; k++) {
            for (int j = 0; j < SIZE; j++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent for order (m, k, j) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");
    time_spent = 0.0;

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    begin = clock();
    // Matrix multiplication (j, k, m)
    for (int j = 0; j < SIZE; j++) {
        for (int k = 0; k < SIZE; k++) {
            for (int m = 0; m < SIZE; m++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent for order (j, k, m) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");
    time_spent = 0.0;

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    begin = clock();
    // Matrix multiplication (k, m, j)
    for (int k = 0; k < SIZE; k++) {
        for (int m = 0; m < SIZE; m++) {
            for (int j = 0; j < SIZE; j++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent for order (k, m, j) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");
    time_spent = 0.0;

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }

    begin = clock();
    // Matrix multiplication (k, j, m)
    for (int k = 0; k < SIZE; k++) {
        for (int j = 0; j < SIZE; j++) {
            for (int m = 0; m < SIZE; m++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time spent for order (k, j, m) is %f seconds \n", time_spent);
    printf("------------------------------------------------\n");

    return 0;
}