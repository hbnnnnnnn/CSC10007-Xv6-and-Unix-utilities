#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        exit(1);
    }
    
    int ticks = atoi(argv[1]);
    if (ticks < 0) {
        exit(1);
    }
    fprintf(1, "(nothing happens for a little while)\n");
    sleep(ticks);
    exit(0);
}