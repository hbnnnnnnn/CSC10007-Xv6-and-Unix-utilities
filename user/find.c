#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void formFullPath(char* fullPath, int size, char* path, char* fileName) {
    int i = 0;

    while (path[i] != '\0' && i < size) {
        fullPath[i] = path[i];
        i++;
    }

    fullPath[i++] = '/';

    if (i >= size - 1) {
        return;
    }

    int j = 0;
    while (fileName[j] != '\0' && i < size - 1) {
        fullPath[i++] = fileName[j++];
    }

    fullPath[i] = '\0';
}


void find(char* path, char* fileName) {
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
            continue;
        }

        char fullPath[512];
        formFullPath(fullPath, sizeof(fullPath), path, de.name);
        
        if (stat(fullPath, &st) < 0) {
            printf("find: cannot stat %s\n", fullPath);
            continue;
        }

        if (st.type == T_DIR) {
            find(fullPath, fileName);
        } else {
            if (strcmp(de.name, fileName) == 0) {
                printf("%s\n", fullPath);
            }
        }
    }

    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: find <path> <file-name>\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}