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
    char* name;
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
    struct FileNode* next;
};

struct FileNode* create_root_dir();

struct FileNode* create_file(struct FileNode* parent);

#endif //FILE_H