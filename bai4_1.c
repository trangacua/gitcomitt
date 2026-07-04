
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>

#define SO_LAN 40

typedef struct {
    int x;
    pthread_mutex_t mutex;
} shared_data_t;

int main() {
    shared_data_t *shared = mmap(NULL, sizeof(shared_data_t),
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shared->x = 0;

    // Khoi tao mutex co the dung chung giua nhieu process
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared->mutex, &attr);

    pid_t pid = fork();

    if (pid == 0) {
        // ----- PROCESS B (con) -----
        for (int i = 0; i < SO_LAN; i++) {
            pthread_mutex_lock(&shared->mutex);
            shared->x = shared->x + 1;
            if (shared->x == 20) shared->x = 0;
            printf("B: x = %d\n", shared->x);
            pthread_mutex_unlock(&shared->mutex);
            usleep(10000);
        }
        exit(0);
    } else {
        // ----- PROCESS A (cha) -----
        for (int i = 0; i < SO_LAN; i++) {
            pthread_mutex_lock(&shared->mutex);
            shared->x = shared->x + 1;
            if (shared->x == 20) shared->x = 0;
            printf("A: x = %d\n", shared->x);
            pthread_mutex_unlock(&shared->mutex);
            usleep(10000);
        }
        wait(NULL);
    }

    pthread_mutex_destroy(&shared->mutex);
    pthread_mutexattr_destroy(&attr);
    munmap(shared, sizeof(shared_data_t));
    return 0;
}
