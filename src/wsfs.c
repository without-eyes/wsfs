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

        case 'c':
            struct FileNode* file = create_file(currentDir);
            add_file_to_dir(currentDir, file);

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

struct FileNode* create_file(struct FileNode* parent) {
    printf("Enter file name: ");
    char fileName[MAX_FILE_NAME];
    fgets(fileName, MAX_FILE_NAME, stdin);

    struct FileNode* file = malloc(sizeof(struct FileNode));
    strncpy(file->attributes.name, fileName, MAX_FILE_NAME);
    file->attributes.type = 'd';
    file->attributes.createdAt = get_current_time();
    file->parent = parent;
    file->next = NULL;
    return file;
}

void add_file_to_dir(const struct FileNode* parent, struct FileNode* child) {
    struct FileNode* emptySlot = parent->attributes.directoryContent;
    while (emptySlot->next != NULL) {
        emptySlot = emptySlot->next;
    }
    emptySlot->next = child;
}

void print_file_info(struct FileNode* file) {
    printf("%c %2u:%2u %s\n", file->attributes.type, file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}