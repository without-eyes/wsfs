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
    FILE_TYPE_DIR = 2,
    FILE_TYPE_SYMLINK = 3
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
        struct FileNode* symlinkTarget;
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
    * @param[in] parent The directory where file node will be
    * located.
    * @param[in] name The name of new file node.
    * @param[in] type The type of new file node.
    *
    * @return Returns created file node.
    *
    * @pre parent != NULL && name != NULL
*/
struct FileNode* create_file_node(struct FileNode* parent, const char* name, enum FileType type);

/**
    * Change current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] name The name of the directory where user
    * wants to go.
    *
    * @pre currentDir != NULL && name != NULL
*/
void change_current_dir(struct FileNode** currentDir, const char* name);

/**
    * Add "child" file node to "parent" directory's linked list.
    *
    * @param[in] parent The directory where file node will be
    * located.
    * @param[in] child The file node that will be added to "parent"
    * directory.
    *
    * @pre parent != NULL && child != NULL
*/
void add_to_dir(struct FileNode* parent, struct FileNode* child);

/**
    * Get file type first letter.
    *
    * @param[in] type The value from enum FileType.
    *
    * @return Returns first letter of type as char.
*/
char get_file_type_letter(enum FileType type);

/**
    * Write content into file.
    *
    * @param[in] node The file in which text will be written.
    * @param[in] content The content which will be written into file.
    *
    * @pre node != NULL && content != NULL
*/
void write_to_file(struct FileNode* node, char* content);

/**
    * Reads content from file.
    *
    * @param[in] node The file node from which content will
    * be read.
    *
    * @return Returns content of file.
    *
    * @pre node != NULL
*/
char* read_file_content(const struct FileNode* node);

/**
    * Find file node by name in current directory.
    *
    * @param[in] currentDir The directory where user is currently
    * located.
    * @param[in] name The name of the file node which user
    * wants to find.
    *
    * @return Returns found file node.
    *
    * @pre currentDir != NULL && name != NULL
*/
struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* name);

/**
    * Find file node by name in entire file system.
    *
    * @param[in,out] root The root directory.
    * @param[in] name The name of the file node which user
    * wants to find.
    *
    * @return Returns found file node.
    *
    * @pre root != NULL && name != NULL
*/
struct FileNode* find_file_node_in_fs(const struct FileNode* root, const char* name);

/**
    * Get file node path. The caller is responsible for freeing
    * the memory allocated for the file node by calling free().
    *
    * @param[in] currentDir The directory where user is currently
    * located.
    * @param[in] name The name of the file node which path
    * user wants to get.
    *
    * @return Returns path to file node.
    *
    * @pre currentDir != NULL && name != NULL
*/
char* get_file_node_path(const struct FileNode* currentDir, const char* name);

/**
    * Delete file node (and it's children if it is a directory) in
    * current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] name The name of the file node which
    * user wants to delete.
    *
    * @pre currentDir != NULL && name != NULL
*/
void delete_file_node(struct FileNode* currentDir, const char* name);

/**
    * Recursively free allocated memory of file node (and it's children
    * if it is a directory).
    *
    * @param[in] fileNode The file node user wants to free.
*/
void free_file_node_recursive(struct FileNode* fileNode);

#endif //FILE_H