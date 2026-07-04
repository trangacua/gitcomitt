
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define LIMIT     33     // TODO: thay bang 10 + 2 so cuoi MSSV cua ban
#define SO_LAN    50

int products = 0;
int sells    = 0;

sem_t sem_produce;   // dem so "cho trong" con co the san xuat them (toi da LIMIT)
sem_t sem_sell;      // dem so san pham da san xuat nhung CHUA ban, dung de seller cho
pthread_mutex_t lock;

void* producer(void* arg) {
    for (int i = 0; i < SO_LAN; i++) {
        sem_wait(&sem_produce);        // neu products - sells == LIMIT thi phai cho
        pthread_mutex_lock(&lock);
        products++;
        printf("[Produce] products = %2d , sells = %2d\n", products, sells);
        pthread_mutex_unlock(&lock);
        sem_post(&sem_sell);           // bao hieu co them 1 san pham de ban
        usleep(rand() % 100000);
    }
    return NULL;
}

void* seller(void* arg) {
    for (int i = 0; i < SO_LAN; i++) {
        sem_wait(&sem_sell);           // neu chua co san pham nao de ban thi phai cho
        pthread_mutex_lock(&lock);
        sells++;
        printf("                    [Sell] sells = %2d , products = %2d\n", sells, products);
        pthread_mutex_unlock(&lock);
        sem_post(&sem_produce);        // giai phong 1 "cho trong" de san xuat tiep
        usleep(rand() % 150000);
    }
    return NULL;
}

int main() {
    pthread_t tProd, tSell;

    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_produce, 0, LIMIT);  // ban dau con du LIMIT cho trong de san xuat
    sem_init(&sem_sell, 0, 0);         // ban dau chua co gi de ban

    pthread_create(&tProd, NULL, producer, NULL);
    pthread_create(&tSell, NULL, seller, NULL);

    pthread_join(tProd, NULL);
    pthread_join(tSell, NULL);

    sem_destroy(&sem_produce);
    sem_destroy(&sem_sell);
    pthread_mutex_destroy(&lock);
    return 0;
}
