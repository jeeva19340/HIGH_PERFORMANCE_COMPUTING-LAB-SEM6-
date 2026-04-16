#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i;
    double *A, *B, *C;

    // 1. Get size from user
    printf("Enter the size of the arrays: ");
    if (scanf("%d", &n) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    // 2. Allocate memory dynamically
    A = (double *)malloc(n * sizeof(double));
    B = (double *)malloc(n * sizeof(double));
    C = (double *)malloc(n * sizeof(double));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // 3. Get array elements from user
    printf("Enter elements for Array A:\n");
    for (i = 0; i < n; i++) {
        scanf("%lf", &A[i]);
    }

    printf("Enter elements for Array B:\n");
    for (i = 0; i < n; i++) {
        scanf("%lf", &B[i]);
    }

    // 4. Parallel addition using OpenMP
    // 'i' is automatically private in OpenMP 'for' loops
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }

    // 5. Display the result
    printf("\nResultant Array C (A + B):\n");
    for (i = 0; i < n; i++) {
        printf("%.2f ", C[i]);
    }
    printf("\n");

    // 6. Free memory
    free(A);
    free(B);
    free(C);

    return 0;
}
