#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#define MAX_FILE_NAME 32

struct FileAttributes {
    uint64_t size;
    char name[MAX_FILE_NAME];
    union {
        struct FileNode* directoryContent;
        char* fileContent;
    };
    uint8_t hourCreatedAt;
    uint8_t minuteCreatedAt;
    char type;
};

struct FileNode {
    struct FileAttributes attributes;
    struct FileNode* parent;
};

#endif //FILE_H