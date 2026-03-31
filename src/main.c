#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "types.h"

i32 main(i32 argc, char *argv[]) {
    if(argc != 2){
        perror("Usage: file-watcher <file>");
        return 1;
    }
    char* path = argv[1];

    DIR* dir = opendir(path);

    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        printf("DIR DOES NOT EXISTS");
        return 1;
    } else {
        perror("opendir() failed");
        return 1;
    }

    return 0;
}
