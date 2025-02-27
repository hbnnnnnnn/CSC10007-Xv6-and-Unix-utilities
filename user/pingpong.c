#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc != 1) {
        printf("Usage: pingpong");
        exit(0);
    }

    char buf[5];
    int p[2];

    if (pipe(p) < 0) {
        printf("pingpong: pipe failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("pingpong: fork failed\n");
        exit(1);
    } else if (pid == 0) {
        // child 
        read(p[0], buf, sizeof(buf));
        printf("%d: received ping\n", getpid());
        write(p[1], "pong", 5);

        close(p[0]);
        close(p[1]);

        exit(0);
    } else {
        // parent
        write(p[1], "ping", 5);
        wait(0);

        read(p[0], buf, sizeof(buf));
        printf("%d: received pong\n", getpid());

        close(p[0]);
        close(p[1]);
    }

    exit(0);
}