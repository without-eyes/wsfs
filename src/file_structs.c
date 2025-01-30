/**
    * @file: file_structs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to files and file nodes.
*/

#include "../include/file_structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

struct FileNode* create_file_node(struct FileNode* parent, const char* fileNodeName, const enum FileType fileType) {
    struct FileNode* fileNode = malloc(sizeof(struct FileNode));
    fileNode->attributes.name = malloc(strlen(fileNodeName) + 1);
    strcpy(fileNode->attributes.name, fileNodeName);
    fileNode->attributes.createdAt = get_current_time();
    fileNode->next = NULL;

    if (fileType == FILE_TYPE_DIR) {
        fileNode->attributes.type = FILE_TYPE_DIR;
        fileNode->attributes.directoryContent = NULL;
    } else if (fileType == FILE_TYPE_FILE) {
        fileNode->attributes.type = FILE_TYPE_FILE;
        fileNode->attributes.fileContent = NULL;
    }

    if (strcmp(fileNodeName, "\\") == 0 ) {
        fileNode->parent = fileNode;
    } else {
        fileNode->parent = parent;
    }

    return fileNode;
}

void add_to_dir(struct FileNode* parent, struct FileNode* child) {
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

char get_file_type_letter(const enum FileType type) {
    switch (type) {
        case FILE_TYPE_DIR:
            return 'd';

        case FILE_TYPE_FILE:
            return 'f';

        case FILE_TYPE_UNKNOWN:
            return '-';

        default:
            return '?';
    }
}

void write_to_file(struct FileNode* file, char* text) {
    file->attributes.fileContent = text;
}

struct FileNode* find_file_node(const struct FileNode* currentDir, const char* fileNodeName) {
    struct FileNode* currentFile = currentDir->attributes.directoryContent;
    while (currentFile != NULL && strcmp(currentFile->attributes.name, fileNodeName) != 0) {
        currentFile = currentFile->next;
    }
    return currentFile;
}