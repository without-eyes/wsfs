#include "../include/wsfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_structs.h"

int wsfs_init(void) {
    // create tree with root in /
    struct FileNode* rootDir = create_root_dir();

    // show UI
    struct FileNode* currentDir = rootDir;
    int isRunning = 1;
    while (isRunning) {
        puts("");
        print_dir_content(rootDir);

        printf("Options: (q)uit, (c)reate file: ");
        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer
        switch (input) {
        case 'q':
            isRunning = 0;
            break;

        case 'c':
            struct FileNode* file = create_file(currentDir);
            add_to_dir(currentDir, file);

        default:
            printf("Invalid input.\n");
            break;
        }
    }

    // free fs
    struct FileNode* currentFile = rootDir->attributes.directoryContent;
    while (currentFile != NULL) {
        struct FileNode* nextFile = currentFile->next;
        free(currentFile->attributes.name);
        free(currentFile);
        currentFile = nextFile;
    }
    free(rootDir->attributes.name);
    free(rootDir);

    return EXIT_SUCCESS;
}

void print_file_info(const struct FileNode* file) {
    printf("%c %2u:%02u %s\n", file->attributes.type, file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}

void print_dir_content(const struct FileNode* directory) {
    print_file_info(directory);
    const struct FileNode* currentFile = directory->attributes.directoryContent;
    while (currentFile != NULL) {
        print_file_info(currentFile);
        currentFile = currentFile->next;
    }
}