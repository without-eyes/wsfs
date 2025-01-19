#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#define MAX_FILE_NAME 32

struct FileNode;

struct Timestamp {
    uint8_t hour;
    uint8_t minute;
};

struct FileAttributes {
    char name[MAX_FILE_NAME];
    union {
        struct FileNode* directoryContent;
        char* fileContent;
    };
    struct Timestamp createdAt;
    char type;
};

struct FileNode {
    struct FileAttributes attributes;
    struct FileNode* parent;
};

#endif //FILE_H