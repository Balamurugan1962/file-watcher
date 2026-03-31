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
    return 0;
}
