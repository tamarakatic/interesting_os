#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define SIZE 5000

void generate_array(int *arr) {
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 10000;
    }
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j+1] < arr[j]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void mergeArrays(int *array1, int *array2, int *array3, int n1, int n2) {
    int i = 0; 
    int j = 0;
    int k = 0;

    while (i < n1 && j < n2) {
        // compare elements of two array
        if (array1[i] < array2[j]) {
            array3[k++] = array1[i++];
        } else {
            array3[k++] = array2[j++];
        }
    }

    while (i < n1) {
        array3[k++] = array1[i++];
    }

    while (j < n2) {
        array3[k++] = array2[j++];
    }
}

void printArrayInFile(int *arr, char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "w+");
    for (int i=0; i < SIZE; i++) {
        fprintf(fp, "%d ", arr[i]);
    }
    fclose(fp);
    printf("\n");
}

void printArray(int *arr, int n) {
    for (int i=0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int array[SIZE];

    generate_array(array);

	int l = 0; 
	int r = SIZE-1;
	int m = l + (r - l) / 2;
    int n1 = m - l + 1; 
    int n2 = r - m;

    int array1[n1];
    int array2[n2];

    for (int i=0; i < n1; i++) {
        array1[i] = array[l+i];
    }

    for (int i=0; i < n2; i++) {
        array2[i] = array[m + 1 + i]; 
    }

    printf("\n");

    int array3[SIZE];
    
    clock_t begin = clock();
    bubbleSort(array, SIZE);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    char file_name[100] = "part_2/results/sorting_result_n_5000.txt";

    printArrayInFile(array, file_name);
    printf("------------------------------------------------\n");
    printf("Time spent for the BubbleSort for N=%d is %f seconds \n\n", SIZE, time_spent);

    printf("---------------------------------------------------------------------------------\n");

    begin = clock();
    bubbleSort(array1, n1);
    end = clock();
    double time_spent_1 = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("------------------------------------------------\n");
    printf("Time spent to sort first half with BubbleSort for N=%d is %f seconds \n\n", (SIZE/2), time_spent_1);

    begin = clock();
    bubbleSort(array2, n2);
    end = clock();
    double time_spent_2 = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("------------------------------------------------\n");
    printf("Time spent to sort second half with BubbleSort for N=%d is %f seconds \n\n", (SIZE/2), time_spent_2);

    begin = clock();
    mergeArrays(array1, array2, array3, n1, n2);
    end = clock();
    double time_spent_3 = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("------------------------------------------------\n");
    printf("Time spent to merge two halfs is %f seconds \n\n", time_spent_3);

    double total_time = time_spent_1 + time_spent_2 + time_spent_3;

    printf("------------------------------------------------\n");
    printf("Total time spent to sort two halfs and merge them is %f seconds \n\n", total_time);

    char file_name_merge[100] = "part_2/results/sorting_result_n_2500_merge_sort.txt";
    printArrayInFile(array3, file_name_merge);

    return 0;
}
