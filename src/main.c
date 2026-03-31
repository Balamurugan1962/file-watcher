#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#define STB_DS_IMPLEMENTATION
#include "../inc/stb_ds.h"
#include "types.h"

typedef struct {
    char* key;
    char* value;
}item;

void rec_list_dir(char* path){
    DIR* dir = opendir(path);

    if (!dir){
        perror("DIR DOES NOT EXISTS");
        exit(1);
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if( strcmp(entry->d_name,".") == 0  ||
            strcmp(entry->d_name,"..") == 0)
            continue;

        char buffer[PATH_MAX];
        snprintf(buffer, sizeof(buffer), "%s/%s", path, entry->d_name);

        struct stat st;

        if (stat(buffer, &st) == 0) {
            char timebuf[100];
            strftime(timebuf, sizeof(timebuf),"%Y-%m-%d %H:%M:%S",localtime(&st.st_mtime));
            printf("%s (modified: %s)\n", entry->d_name, timebuf);
        }

        if(entry->d_type == DT_DIR){
            rec_list_dir(buffer);
        }
    }

    closedir(dir);
}

i32 main(i32 argc, char *argv[]) {
    if(argc != 2){
        perror("Usage: file-watcher <file>");
        return 1;
    }
    char* path = argv[1];

    DIR* dir = opendir(path);

    if (dir) {
        closedir(dir);
        rec_list_dir(path);
    } else if (ENOENT == errno) {
        perror("DIR DOES NOT EXISTS");
        return 1;
    } else {
        perror("opendir() failed");
        return 1;
    }

    item* hash = NULL;

    hmput(hash,"file1","time1");
    hmput(hash,"file1/file","time2");
    hmput(hash,"file2","time2");

    printf("%s - \n",hmget(hash,"file1"));
    printf("%s - \n",hmget(hash,"file1/file"));

    return 0;
}
