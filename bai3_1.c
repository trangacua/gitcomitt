

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define SO_LAN 40

int main() {
    // Cap phat vung nho chia se giua process cha va con
    int *x = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *x = 0;

    pid_t pid = fork();

    if (pid == 0) {
        // ----- PROCESS B (con) -----
        for (int i = 0; i < SO_LAN; i++) {
            (*x) = (*x) + 1;
            if (*x == 20) *x = 0;
            printf("B: x = %d\n", *x);
            usleep(10000);
        }
        exit(0);
    } else {
        // ----- PROCESS A (cha) -----
        for (int i = 0; i < SO_LAN; i++) {
            (*x) = (*x) + 1;
            if (*x == 20) *x = 0;
            printf("A: x = %d\n", *x);
            usleep(10000);
        }
        wait(NULL); // cho process con ket thuc
    }

    munmap(x, sizeof(int));
    return 0;
}
