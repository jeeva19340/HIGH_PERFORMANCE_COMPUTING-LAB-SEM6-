#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000

int arr[MAX];
int n,i, thread_count;
long  global_sum = 0;
long thread;

void *partial_sum(void *rank)
{
    long my_rank = (long) rank;

    int local_n = n / thread_count;
    int first = my_rank * local_n;
    int last = first + local_n - 1;
    long local_sum = 0;
    printf("Thread %ld:First index=%d,Last index=%d\n",my_rank,first,last);
    for(i = first; i <= last; i++)
    {
        local_sum += arr[i];
    }
    global_sum += local_sum;
    pthread_exit(NULL);
}

int main()
{
    printf("ARRAY OF N NUMBERS USING PTHREADS\n");
    pthread_t *thread_handles;
    printf("Enter number of elements: ");
    scanf("%d", &n);
    printf("Enter the elements:\n");
    for(i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    printf("Enter number of threads: ");
    scanf("%d", &thread_count);
    thread_handles = malloc(thread_count * sizeof(pthread_t));
    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread],NULL,partial_sum, (void*) thread);
    }

    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }
    printf("\nTotal Sum = %ld\n", global_sum);
    free(thread_handles);

    return 0;
}
