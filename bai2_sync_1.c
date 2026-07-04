
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 100

int a[N];
int count = 0;
sem_t mutex; // bao ve truy xuat count va mang a

void* producer(void* arg) {
    for (int i = 0; i < 20; i++) {
        int val = rand() % 1000;

        sem_wait(&mutex);
        if (count < N) {
            a[count] = val;
            count++;
            printf("[Producer] them %d, count = %d\n", val, count);
        }
        sem_post(&mutex);

        usleep(rand() % 50000);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 20; i++) {
        sem_wait(&mutex);
        if (count > 0) {
            int val = a[count - 1];
            count--;
            printf("                  [Consumer] lay %d, count = %d\n", val, count);
        } else {
            printf("                  [Consumer] Nothing in array a\n");
        }
        sem_post(&mutex);

        usleep(rand() % 70000);
    }
    return NULL;
}

int main() {
    pthread_t p1, p2;
    sem_init(&mutex, 0, 1);

    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, consumer, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    sem_destroy(&mutex);
    return 0;
}
