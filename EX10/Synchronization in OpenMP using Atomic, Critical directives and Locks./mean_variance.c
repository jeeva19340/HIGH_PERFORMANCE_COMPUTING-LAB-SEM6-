#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>

#define MAX_QUEUE 100
#define SIZE 50

int queue[MAX_QUEUE][SIZE];
int sizes[MAX_QUEUE];

int front = 0, rear = 0, count = 0;
omp_lock_t lock;

int done = 0;   // ✅ IMPORTANT FLAG

/* ---------------- ENQUEUE ---------------- */
void enQueue(int* arr, int size){
    omp_set_lock(&lock);

    for(int i = 0; i < size; i++)
        queue[rear][i] = arr[i];

    sizes[rear] = size;

    rear = (rear + 1) % MAX_QUEUE;
    count++;

    omp_unset_lock(&lock);
}

/* ---------------- DEQUEUE ---------------- */
int deQueue(int* arr, int* size){
    int success = 0;

    omp_set_lock(&lock);

    if(count > 0){
        *size = sizes[front];

        for(int i = 0; i < *size; i++)
            arr[i] = queue[front][i];

        front = (front + 1) % MAX_QUEUE;
        count--;

        success = 1;
    }

    omp_unset_lock(&lock);

    return success;
}

/* ---------------- MEAN ---------------- */
int mean(int* arr, int size){
    int sum = 0;

    for(int i = 0; i < size; i++)
        sum += arr[i];

    return sum / size;
}

/* ---------------- VARIANCE ---------------- */
int variance(int* arr, int size){
    int sum = 0, sum2 = 0;

    for(int i = 0; i < size; i++)
        sum += arr[i];

    int avg = sum / size;

    for(int i = 0; i < size; i++)
        sum2 += (arr[i] - avg) * (arr[i] - avg);

    return sum2 / size;
}

/* ---------------- MAIN ---------------- */
int main(){
    omp_init_lock(&lock);

    int num_threads = 5;
    int global_mean = 0, global_var = 0;

    #pragma omp parallel num_threads(num_threads)
    {
        int id = omp_get_thread_num();

        /* ---------------- PRODUCER ---------------- */
        if(id == 0){
            int k;
            printf("Enter number of arrays: ");
            scanf("%d", &k);

            for(int x = 0; x < k; x++){
                int size;
                printf("Enter size of array %d: ", x+1);
                scanf("%d", &size);

                int* arr =(int*)malloc(size * sizeof(int));

                for(int i = 0; i < size; i++)
                    scanf("%d", &arr[i]);

                enQueue(arr, size);
                free(arr);
            }

            done = 1;   // ✅ producer finished
        }

        /* ---------------- CONSUMERS ---------------- */
        else{
            int arr[SIZE], size;

            while(1){
                if(deQueue(arr, &size)){
                    int m = mean(arr, size);
                    int v = variance(arr, size);

                    #pragma omp critical
                    {
                        global_mean += m;
                        global_var  += v;
                    }
                }
                else{
                    if(done) break;   // ✅ correct termination
                }
            }
        }
    }

    printf("\nGLOBAL MEAN = %d\n", global_mean);
    printf("GLOBAL VARIANCE = %d\n", global_var);

    omp_destroy_lock(&lock);
    return 0;
}
