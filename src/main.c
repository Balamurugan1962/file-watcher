#include <unistd.h>
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
    time_t value;
}item;

void rec_list_dir(char* path,item** hash){
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
            time_t mod_time = st.st_mtime;

            char timebuf[100];
            strftime(timebuf, sizeof(timebuf),"%Y-%m-%d %H:%M:%S",localtime(&mod_time));

            int idx = shgeti(*hash, buffer);

            if (idx == -1) {
                printf("[%s] NEW FILE \t\t %s\n",timebuf, buffer);
                shput(*hash, strdup(buffer), mod_time);
            } else{
                time_t old_time = (*hash)[idx].value;
                if (old_time != mod_time) {
                    printf("[%s] FILE CHANGED \t %s\n",timebuf, buffer);
                    (*hash)[idx].value = mod_time;
                }
            }
        }

        if(S_ISDIR(st.st_mode)){
            rec_list_dir(buffer,hash);
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
    item* hash = NULL;

    DIR* dir = opendir(path);

    if (dir) {
        closedir(dir);
        while(1){
            rec_list_dir(path,&hash);
            sleep(3);
        }
    } else if (ENOENT == errno) {
        perror("DIR DOES NOT EXISTS");
        return 1;
    } else {
        perror("opendir() failed");
        return 1;
    }

    return 0;
}
