/**
    * @file: file_node_funcs.h
    * @author: without eyes
    *
    * This file contains functions related to files and file nodes.
*/

#ifndef FILE_H
#define FILE_H

#include "file_node_structs.h"
#include <stddef.h>
#define MAX_NAME_SIZE 32

/**
    * Create file node in "parent" directory. The caller is
    * responsible for freeing the memory allocated for the
    * file node and it's name by calling free(). If name of
    * the directory is "\" , it will be it's own parent.
    *
    * @param[in] parent The directory where file node will be
    * located.
    * @param[in] name The name of new file node.
    * @param[in] type The type of new file node(use FILE_TYPE_*)
    * @param permissions The permission of file(use PERM_*).
    *
    * @return Returns created file node.
    *
    * @pre parent != NULL && name != NULL
*/
struct FileNode* create_file_node(struct FileNode* parent, const char* name, enum FileType type, enum Permissions permissions);

/**
    * Changes the permissions of file node.
    *
    * @param[in] node The file node which permissions will be
    * changed.
    * @param permissions The permission of file(use PERM_*).
    *
    * @pre node != NULL
*/
void change_permissions(struct FileNode* node, enum Permissions permissions);

/**
    * Gets size of file node recursively.
    *
    * @param[in] node The node which size user wants to
    * get.
    *
    * @return Returns size of file node.
    *
    * @pre node != NULL
    * @pre node must have READ permission
*/
size_t get_file_node_size(const struct FileNode* node);

/**
    * Changes current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] newCurrentDir The directory where user wants to go.
    *
    * @pre currentDir != NULL && newCurrentDir != NULL
    * @pre newCurrentDir must have READ and EXEC permission
*/
void change_current_dir(struct FileNode** currentDir, struct FileNode* newCurrentDir);

/**
    * Add "child" file node to "parent" directory's linked list.
    *
    * @param[in] parent The directory where file node will be
    * located.
    * @param[in] child The file node that will be added to "parent"
    * directory.
    *
    * @pre parent != NULL && child != NULL
    * @pre parent must have WRITE permission
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
    * Get permission's letter.
    *
    * @param[in] permission The permission from enum Permissions.
    *
    * @return Returns first letter of type as char.
*/
char get_permission_letter(enum Permissions permission);

/**
    * Sets the target of symbolic link.
    *
    * @param[in] symlink The symbolic link which target will
    * be set.
    * @param[in] target The file node that will be the target
    * of symbolic link.
    *
    * @pre symlink != NULL && target != NULL
    * @pre symlink must have WRITE permission
*/
void set_symlink_target(struct FileNode* symlink, struct FileNode* target);

/**
    * Gets the target of symbolic link.
    *
    * @param[in] symlink The symbolic link whose target user
    * wants to get.
    *
    * @return Returns the target of symbolic link.
    *
    * @pre symlink != NULL
    * @pre symlink must have READ permission
*/
struct FileNode* get_symlink_target(struct FileNode* symlink);

/**
    * Write content into file.
    *
    * @param[in] node The file in which text will be written.
    * @param[in] content The content which will be written into file.
    *
    * @pre node != NULL && content != NULL
    * @pre node must have WRITE permission
*/
void write_to_file(struct FileNode* node, const char* content);

/**
    * Reads content from file.
    *
    * @param[in] node The file node from which content will
    * be read.
    *
    * @return Returns content of file.
    *
    * @pre node != NULL
    * @pre node must have READ permission
*/
char* read_file_content(struct FileNode* node);

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
    * @pre currentDir must have READ and EXEC permission
*/
struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* name);

/**
    * Find file node by name in entire file system recursively.
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
    * @param[in] node The node which path user wants to get.
    *
    * @return Returns path to file node.
    *
    * @pre node != NULL
*/
char* get_file_node_path(const struct FileNode* node);

/**
    * Changes file node location.
    *
    * @param[in,out] node The file node whose location will be changed.
    * @param[in] location The file node where node will be located.
    *
    * @pre node != NULL && location != NULL
    * @pre location must have WRITE permission
    * @pre node must have WRITE permission
*/
void change_file_node_location(struct FileNode* location, struct FileNode* node);

/**
    * Changes file node name. The caller is responsible for freeing
    * the memory allocated for the file node's name by calling free().
    *
    * @param[in,out] node The file node whose name will be changed.
    * @param[in] name The new name of file node.
    *
    * @pre node != NULL && name != NULL
    * @pre node must have WRITE permission
*/
void change_file_node_name(struct FileNode* node, const char* name);

/**
    * Delete file node (and it's children if it is a directory) in
    * current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] node The file node which user wants to delete.
    *
    * @pre currentDir != NULL && node != NULL
*/
void delete_file_node(struct FileNode* currentDir, struct FileNode* node);

/**
    * Recursively free allocated memory of file node (and it's children
    * if it is a directory).
    *
    * @param[in] fileNode The file node user wants to free.
*/
void free_file_node_recursive(struct FileNode* fileNode);

#endif //FILE_H