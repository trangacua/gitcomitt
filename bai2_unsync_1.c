
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 100

int a[N];
int count = 0; // so phan tu hien co trong mang a

void* producer(void* arg) {
    for (int i = 0; i < 20; i++) {
        int val = rand() % 1000;
        if (count < N) {
            a[count] = val;
            count++;                 // KHONG DONG BO -> co the xay ra tranh chap
            printf("[Producer] them %d, count = %d\n", val, count);
        }
        usleep(rand() % 50000);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 20; i++) {
        if (count > 0) {
            int val = a[count - 1];
            count--;                 // KHONG DONG BO -> co the xay ra tranh chap
            printf("                  [Consumer] lay %d, count = %d\n", val, count);
        } else {
            printf("                  [Consumer] Nothing in array a\n");
        }
        usleep(rand() % 70000);
    }
    return NULL;
}

int main() {
    pthread_t p1, p2;
    pthread_create(&p1, NULL, producer, NULL);
    pthread_create(&p2, NULL, consumer, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    return 0;
}
