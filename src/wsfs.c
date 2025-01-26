/**
    * @file: wsfs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to Without eyeS's File System(WSFS).
*/

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
        print_dir_content(currentDir);

        printf("Options: (q)uit, create (f)ile or (d)irectory: ");
        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer
        switch (input) {
        case 'q': // quit
            isRunning = 0;
            break;

        case 'g': // go into directory
            printf("Enter directory name: ");
            char dirName[MAX_NAME_SIZE];
            fgets(dirName, MAX_NAME_SIZE, stdin);
            dirName[strcspn(dirName, "\n")] = 0;
            change_current_dir(&currentDir, dirName);
            break;

        case 'b' : // go back
            currentDir = currentDir->parent;
            break;

        case 'f': // create file
            struct FileNode* file = create_file(currentDir);
            add_to_dir(currentDir, file);
            break;

        case 'd': // create directory
            struct FileNode* dir = create_dir(currentDir);
            add_to_dir(currentDir, dir);
            break;

        default: // other
            printf("Invalid input.\n");
            break;
        }
    }

    // free fs
    wsfs_end(rootDir);

    return EXIT_SUCCESS;
}

void wsfs_end(struct FileNode* rootDir) {
    struct FileNode* currentFile = rootDir->attributes.directoryContent;
    while (currentFile != NULL) {
        struct FileNode* nextFile = currentFile->next;
        free(currentFile->attributes.name);
        free(currentFile);
        currentFile = nextFile;
    }
    free(rootDir->attributes.name);
    free(rootDir);
}

void print_file_info(const struct FileNode* file) {
    printf("%c %2u:%02u %s\n", get_file_type_letter(file->attributes.type), file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}

void print_dir_content(const struct FileNode* directory) {
    print_file_info(directory);
    const struct FileNode* currentFile = directory->attributes.directoryContent;
    while (currentFile != NULL) {
        print_file_info(currentFile);
        currentFile = currentFile->next;
    }
}

void change_current_dir(struct FileNode** currentDir, const char* dirName) {
    struct FileNode* currentFile = (*currentDir)->attributes.directoryContent;
    while (currentFile != NULL &&
        (currentFile->attributes.type != FILE_TYPE_DIR ||
        strcmp(currentFile->attributes.name, dirName) != 0)) {
        currentFile = currentFile->next;
    }

    if (currentFile != NULL) {
        *currentDir = currentFile;
    } else {
        printf("Can't open file as directory!");
    }
}