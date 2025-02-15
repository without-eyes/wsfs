/**
    * @file: file_node_structs.h
    * @author: without eyes
    *
    * This file contains structs related to files and file nodes.
*/

#ifndef FILE_NODE_STRUCTS_H
#define FILE_NODE_STRUCTS_H

#include <stdint.h>

enum FileType {
    FILE_TYPE_UNKNOWN = 0,
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2,
    FILE_TYPE_SYMLINK = 3
};

enum Permissions {
    PERM_NONE = 0,
    PERM_READ = 4,
    PERM_WRITE = 2,
    PERM_EXEC = 1,
};

struct FileNode;

struct Timestamp {
    uint8_t hour, minute;
};

struct FileAttributes {
    char* name;
    union {
        struct FileNode* directoryContent;
        struct FileNode* symlinkTarget;
        char* fileContent;
    };
    struct Timestamp createdAt;
    enum FileType type;
    enum Permissions permissions;
};

struct FileNode {
    struct FileAttributes attributes;
    struct FileNode* parent;
    struct FileNode* next;
};

#endif //FILE_NODE_STRUCTS_H
