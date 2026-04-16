#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i;
    float constant;
    float *arr;

    // 1. Get array size and constant value from user
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    printf("Enter the constant value to add: ");
    scanf("%f", &constant);

    // 2. Dynamically allocate memory
    arr = (float *)malloc(n * sizeof(float));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // 3. Get array elements from user
    printf("Enter %d elements:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%f", &arr[i]);
    }

    // 4. Parallel addition of constant
    // Each thread takes a portion of the loop iterations
    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        arr[i] = arr[i] + constant;
    }

    // 5. Display the result
    printf("\nUpdated Array:\n");
    for (i = 0; i < n; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    // 6. Clean up
    free(arr);

    return 0;
}
