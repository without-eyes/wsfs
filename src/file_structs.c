#include "../include/file_structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

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
    char fileName[MAX_NAME_SIZE];
    fgets(fileName, MAX_NAME_SIZE, stdin);
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

struct FileNode* create_dir(struct FileNode* parent) {
    printf("Enter directory name: ");
    char directoryName[MAX_NAME_SIZE];
    fgets(directoryName, MAX_NAME_SIZE, stdin);
    directoryName[strcspn(directoryName, "\n")] = 0;

    struct FileNode* dir = malloc(sizeof(struct FileNode));
    dir->attributes.name = malloc(strlen(directoryName) + 1);
    strcpy(dir->attributes.name, directoryName);
    dir->attributes.type = 'd';
    dir->attributes.createdAt = get_current_time();
    dir->attributes.directoryContent = NULL;
    dir->parent = parent;
    dir->next = NULL;
    return dir;
}