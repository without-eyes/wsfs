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
    struct FileNode* currentDir = rootDir;
    int isRunning = 1;
    while (isRunning) {
        const char input = getchar();

        switch (input) {
        case 'q':
            isRunning = 0;
            break;

        default:
            break;
        }

        print_file_info(rootDir);
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
    rootDir->parent = NULL;
    rootDir->next = NULL;
    return rootDir;
}

void print_file_info(struct FileNode* file) {
    printf("%c %2u:%2u %s\n", file->attributes.type, file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}