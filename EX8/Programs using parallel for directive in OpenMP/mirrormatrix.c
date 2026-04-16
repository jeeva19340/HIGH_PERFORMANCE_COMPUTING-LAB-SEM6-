#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int rows, cols;

    // 1. Get matrix dimensions from user
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);

    // 2. Dynamically allocate memory for matrices
    int **matrix = (int **)malloc(rows * sizeof(int *));
    int **mirror = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        mirror[i] = (int *)malloc(cols * sizeof(int));
    }

    // 3. Get matrix elements from user
    printf("Enter the matrix elements:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }

    // 4. Mirror logic using OpenMP parallel for
    // This distributes rows among available threads
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Horizontal mirror: reverse the column index
            mirror[i][cols - 1 - j] = matrix[i][j];
        }
    }

    // 5. Display results
    printf("\nOriginal Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\nMirrored Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", mirror[i][j]);
        }
        printf("\n");
    }

    // 6. Free allocated memory
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
        free(mirror[i]);
    }
    free(matrix);
    free(mirror);

    return 0;
}
