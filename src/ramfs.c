#include "../include/ramfs.h"

#include <stdlib.h>
#include <string.h>
#include "../include/file_structs.h"
#include "../include/utils.h"

int ramfs_init() {
    // create tree with root in /
    struct FileNode* rootDir = create_root_directory();

    // show UI using ncurses

    // free fs
    free(rootDir);

    return EXIT_SUCCESS;
}

struct FileNode* create_root_dir() {
    struct FileNode* rootDir = malloc(sizeof(struct FileNode));
    strncpy(rootDir->attributes.name, "/", 1);
    rootDir->attributes.type = 'd';
    rootDir->attributes.createdAt = get_current_time();
    return rootDir;
}