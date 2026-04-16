#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SIZE 5
#define TOTAL_TASKS 10

typedef struct { int a, b; char op; } Task;

Task buffer[SIZE];
int in = 0, out = 0, produced = 0, consumed = 0;
sem_t empty, full, mutex;
FILE *fp;

double calculate(Task t) {
    if (t.op == '+') return t.a + t.b;
    if (t.op == '-') return t.a - t.b;
    if (t.op == '*') return t.a * t.b;
    if (t.op == '/' && t.b != 0) return (double)t.a / (double)t.b;
    return 0;
}

void* producer(void* arg) {
    char ops[] = {'+', '-', '*', '/'};
    while (1) {
        sem_wait(&empty);
        sem_wait(&mutex);

        if (produced >= TOTAL_TASKS) {
            sem_post(&mutex);
            sem_post(&empty);
            break;
        }

        Task t = { .a = rand() % 100, .b = (rand() % 99) + 1, .op = ops[rand() % 4] };
        buffer[in] = t;
        in = (in + 1) % SIZE;
        produced++;

        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);

        if (consumed >= TOTAL_TASKS) {
            sem_post(&mutex);
            sem_post(&full);
            break;
        }

        Task t = buffer[out];
        out = (out + 1) % SIZE;
        consumed++;

        sem_post(&mutex);
        sem_post(&empty);

        double result = calculate(t);
        fprintf(fp, "Thread %lu: %d %c %d = %.2lf\n", (unsigned long)pthread_self(), t.a, t.op, t.b, result);
        fflush(fp);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t prod, c[4];
    fp = fopen("result1.txt", "w");
    if (!fp) return 1;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    int i;
    pthread_create(&prod, NULL, producer, NULL);
    for(i=0; i<4; i++) pthread_create(&c[i], NULL, consumer, NULL);

    pthread_join(prod, NULL);
    for(i=0; i<4; i++) pthread_join(c[i], NULL);

    fclose(fp);
    sem_destroy(&empty); sem_destroy(&full); sem_destroy(&mutex);

    printf("Results written to result1.txt\n");
    return 0;
}
