#include "../include/wsfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_structs.h"
#include "../include/utils.h"

int wsfs_init(void) {
    // create tree with root in /
    struct FileNode* rootDir = create_root_dir();

    // show UI
    while (1) {
        print_file_info(rootDir);
        getchar();
    }

    // free fs
    free(rootDir);

    return EXIT_SUCCESS;
}

struct FileNode* create_root_dir(void) {
    struct FileNode* rootDir = malloc(sizeof(struct FileNode));
    strncpy(rootDir->attributes.name, "/", 1);
    rootDir->attributes.type = 'd';
    rootDir->attributes.createdAt = get_current_time();
    return rootDir;
}

void print_file_info(struct FileNode* file) {
    printf("%c %2u:%2u %s\n", file->attributes.type, file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}