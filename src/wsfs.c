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
            add_file_to_dir(currentDir, file);

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

struct FileNode* create_root_dir(void) {
    struct FileNode* rootDir = malloc(sizeof(struct FileNode));
    rootDir->attributes.name = malloc(2);
    strcpy(rootDir->attributes.name, "/");
    rootDir->attributes.type = 'd';
    rootDir->attributes.createdAt = get_current_time();
    rootDir->attributes.directoryContent = NULL;
    rootDir->parent = NULL;
    rootDir->next = NULL;
    return rootDir;
}

struct FileNode* create_file(struct FileNode* parent) {
    printf("Enter file name: ");
    char fileName[MAX_FILE_NAME];
    fgets(fileName, MAX_FILE_NAME, stdin);
    fileName[strcspn(fileName, "\n")] = 0;

    struct FileNode* file = malloc(sizeof(struct FileNode));
    file->attributes.name = malloc(strlen(fileName) + 1);
    strcpy(file->attributes.name, fileName);
    file->attributes.type = 'f';
    file->attributes.createdAt = get_current_time();
    file->parent = parent;
    file->next = NULL;
    return file;
}

void add_file_to_dir(struct FileNode* parent, struct FileNode* child) {
    if (parent->attributes.directoryContent == NULL) {
        parent->attributes.directoryContent = child;
        return;
    }

    struct FileNode* current = parent->attributes.directoryContent;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = child;
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