#include <stdio.h>
#include <omp.h>

int main() {
    int n, i, j;
    printf("Enter array size: ");
    scanf("%d", &n);

    int a[n], res[n];
    printf("Enter %d elements: ", n);
    for (i = 0; i < n; i++) {
        scanf(" %d", &a[i]);
    }
    #pragma omp parallel for private(i, j)
    for (i = 0; i < n; i++) {
        int current_min = a[0];

        for (j = 1; j <= i; j++) {
            if (a[j] < current_min) {
                current_min = a[j];
            }
        }
        res[i] = current_min;
    }

    printf("\nRunning Minimum Result:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");
