/**
    * @file: file_structs.h
    * @author: without eyes
    *
    * This file contains structs and functions related to files
    * and file nodes.
*/

#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#define MAX_NAME_SIZE 32

enum FileType {
    FILE_TYPE_UNKNOWN = 0,
    FILE_TYPE_FILE = 1,
    FILE_TYPE_DIR = 2
};

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
    enum FileType type;
};

struct FileNode {
    struct FileAttributes attributes;
    struct FileNode* parent;
    struct FileNode* next;
};

/**
    * Create root directory "/". The caller is responsible for freeing
    * the memory allocated for the directory by calling free().
    *
    * @return Returns root directory as pointer to struct FileNode.
*/
struct FileNode* create_root_dir(void);

/**
    * Create file in "parent" directory. The caller must input name
    * of the file in console. The caller is responsible for freeing
    * the memory allocated for the file by calling free().
    *
    * @param[in] parent The directory where file will be located.
    * @return Returns file as pointer to struct FileNode.
    *
    * @pre parent != NULL
*/
struct FileNode* create_file(struct FileNode* parent);

/**
    * Create directory in "parent" directory. The caller must input
    * name of the directory in console. The caller is responsible for
    * freeing the memory allocated for the directory by calling free().
    *
    * @param[in] parent The directory where file will be located.
    * @return Returns directory as pointer to struct FileNode.
    *
    * @pre parent != NULL
*/
struct FileNode* create_dir(struct FileNode* parent);

/**
    * Add "child" file/directory to "parent" directory's linked list.
    *
    * @param[in,out] parent The directory where file/directory will be
    * located.
    * @param[in] child The file/directory that will be added to "parent"
    * directory.
    *
    * @pre parent != NULL && child != NULL
*/
void add_to_dir(struct FileNode* parent, struct FileNode* child);

/**
    * Get file type first letter(e.g. 'd' or 'f').
    *
    * @param[in] type The value from enum FileType.
    * @return Returns first letter of type as char.
*/
char get_file_type_letter(enum FileType type);

#endif //FILE_H