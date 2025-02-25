#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_NUM 280

void primes(int) __attribute__((noreturn)); 

void primes (int left) {
    int prime;
    if (read(left, &prime, 4) <= 0) {
        close(left);
        exit(1);
    }
    fprintf(1, "prime %d\n", prime);
    int right[2];
    if (pipe(right) < 0) {
        fprintf(2, "pipe error!\n");
        close(left);
        exit(1);
    }
    int pid = fork();
    if (pid == 0) {
        close(right[1]);
        close(left);
        primes(right[0]);
        exit(0);
    }
    else {
        close(right[0]);
        int n;
        while(read(left, &n, 4) > 0) {
            if (n % prime != 0) {
                write(right[1], &n, 4);
            }
        }
        close(left);
        close(right[1]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    //pipe: 0 for read, 1 for write
    int p[2];
    if(pipe(p) < 0) {
        fprintf(2, "pipe error!\n");
        exit(1);
    }
    int pid = fork();
    //fork(): 0 for child, other for parent
    //child process read and filter primes
    //parent process wait for child to finish
    if (pid == 0) {
        close(p[1]);
        primes(p[0]);
        exit(0);
    }
    else {
        close(p[0]);
        for(int i = 2; i <= MAX_NUM; i++) {
            if (write(p[1], &i, 4) < 0) {
                fprintf(2, "write error!\n");
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}