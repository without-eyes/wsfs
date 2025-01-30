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
    * Create file node in "parent" directory. The caller is
    * responsible for freeing the memory allocated for the
    * file node by calling free(). If name of the directory is
    * "\" , it will be it's own parent.
    *
    * @param[in] parent The directory where file will be located.
    * @param[in] fileNodeName The name of new file node.
    * @param[in] fileType The type of new file node.
    * @return Returns file as pointer to struct FileNode.
    *
    * @pre parent != NULL
*/
struct FileNode* create_file_node(struct FileNode* parent, const char* fileNodeName, enum FileType fileType);

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

/**
    * Write text into file.
    *
    * @param[in] file The file in which text will be written.
    * @param[in] text The text which will be written into file.
*/
void write_to_file(struct FileNode* file, char* text);

/**
    * Find file node by name.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] fileNodeName The name of the file node which user
    * wants to find.
    *
    * @pre currentDir != NULL && fileNodeName != NULL
*/
struct FileNode* find_file_node(const struct FileNode* currentDir, const char* fileNodeName);

#endif //FILE_H