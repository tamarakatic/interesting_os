#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define SIZE 3

int main() {
    int C[SIZE][SIZE];

    int A[SIZE][SIZE] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int B[SIZE][SIZE] = {
        {1, 1, 1},
        {2, 3, 4},
        {3, 3, 3}
    };

    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            C[m][n] = 0;
        }
    }
    // Matrix multiplication
    for (int m = 0; m < SIZE; m++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                C[m][j] += A[m][k] * B[k][j];
            }
        }
    }
    
    for (int m = 0; m < SIZE; m++) {
        for (int n = 0; n < SIZE; n++) {
            printf("%d\t", C[m][n]);
        }
        printf("\n");
    }

    return 0;
}