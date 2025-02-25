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
    * @return Returns NULL if memory allocation failed, else
    * returns created file node.
    *
    * @note If pass name as NULL, the name of the node will be "?".
    * @note From the given permissions will be subtracted PERMISSION_MASK.
*/
struct FileNode* create_file_node(struct FileNode* parent, const char* name, enum FileType type, enum Permissions permissions);

/**
    * Changes the permissions of file node.
    *
    * @param[in] node The file node which permissions will be
    * changed.
    * @param permissions The permission of file(use PERM_*).
    *
    * @return Returns 1 if preconditions aren't met, else
    * returns 0.
    *
    * @pre node != NULL
*/
uint8_t change_permissions(struct FileNode* node, enum Permissions permissions);

/**
    * Compares permissions.
    *
    * @param[in] left The first permissions.
    * @param[in] right The second permissions.
    *
    * @return Returns 0 if false, other number if true.
*/
uint8_t is_permissions_equal(enum Permissions left, enum Permissions right);

/**
    * Sets root node.
    *
    * @param[in] node The node which will become root.
    *
    * @pre node != NULL
*/
void set_root_node(struct FileNode* node);

/**
    * Returns root node.
    *
    * @return Returns root node.
*/
struct FileNode* get_root_node();

/**
    * Gets size of file node recursively.
    *
    * @param[in] node The node which size user wants to
    * get.
    *
    * @return Returns 0 if preconditions aren't met, else
    * returns size of file node.
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
    * @return Returns 1 if preconditions are not met, else return 0.
    *
    * @pre currentDir != NULL && newCurrentDir != NULL
    * @pre newCurrentDir must have READ and EXEC permission
*/
uint8_t change_current_dir(struct FileNode** currentDir, struct FileNode* newCurrentDir);

/**
    * Add "child" file node to "parent" directory's linked list.
    *
    * @param[in] parent The directory where file node will be
    * located.
    * @param[in] child The file node that will be added to "parent"
    * directory.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre parent != NULL && child != NULL
    * @pre parent must have WRITE permission
*/
uint8_t add_to_dir(struct FileNode* restrict parent, struct FileNode* restrict child);

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
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre symlink != NULL && target != NULL
    * @pre symlink must have WRITE permission
*/
uint8_t set_symlink_target(struct FileNode* symlink, struct FileNode* target);

/**
    * Gets the target of symbolic link.
    *
    * @param[in] symlink The symbolic link whose target user
    * wants to get.
    *
    * @return Returns NULL if preconditions aren't met, else
    * returns the target of symbolic link.
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
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre node != NULL && content != NULL
    * @pre node must have WRITE permission
*/
uint8_t write_to_file(struct FileNode* node, const char* content);

/**
    * Reads content from file.
    *
    * @param[in] node The file node from which content will
    * be read.
    *
    * @return Returns NULL if preconditions aren't met, else
    * returns the content of file.
    *
    * @pre node != NULL
    * @pre node must have WRITE and READ permission
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
    * @return Returns NULL if preconditions aren't met, else
    * returns found file node.
    *
    * @pre currentDir != NULL && name != NULL
    * @pre currentDir must have READ and EXEC permission
*/
struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* name);

/**
    * Find file node by name in entire file system.
    *
    * @param[in,out] root The root directory.
    * @param[in] name The name of the file node which user
    * wants to find.
    *
    * @return Returns NULL if preconditions aren't met, else
    * returns found file node.
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
    * @return Returns NULL if preconditions aren't met, else
    * returns path to file node.
    *
    * @pre node != NULL
*/
char* get_file_node_path(const struct FileNode* node);

/**
    * Copy file node to location.
    *
    * @param[in,out] node The file node which will be copied.
    * @param[in] location The directory where node will be copied.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre node != NULL && location != NULL
    * @pre location must have WRITE permission
    * @pre node must have WRITE permission
*/
uint8_t change_file_node_location(struct FileNode* restrict location, struct FileNode* restrict node);

/**
    * Changes file node location. The caller is responsible for freeing
    * the memory allocated for the file node by calling free().
    *
    * @param[in,out] node The file node whose location will be changed.
    * @param[in] location The file node where node will be located.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre node != NULL && location != NULL
    * @pre location must have FILE_TYPE_DIR
    * @pre location must have WRITE permission
*/
uint8_t copy_file_node(struct FileNode* restrict location, const struct FileNode* restrict node);

/**
    * Changes file node name. The caller is responsible for freeing
    * the memory allocated for the file node's name by calling free().
    *
    * @param[in,out] node The file node whose name will be changed.
    * @param[in] name The new name of file node.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre node != NULL && name != NULL
    * @pre node must have WRITE permission
*/
uint8_t change_file_node_name(struct FileNode* node, const char* name);

/**
    * Delete file node (and it's children if it is a directory) in
    * current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] node The file node which user wants to delete.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre currentDir != NULL && node != NULL
*/
uint8_t delete_file_node(struct FileNode* restrict currentDir, struct FileNode* restrict node);

/**
    * Frees allocated memory of file node (and it's children
    * if it is a directory).
    *
    * @param[in] node The file node user wants to free.
    *
    * @return Returns 1 if preconditions aren't met, else returns 0.
    *
    * @pre node != NULL
*/
uint8_t free_file_node_recursive(struct FileNode* node);

/**
    * Retrieves the current local time (hour and minute).
    *
    * @return Returns the current hour and minute.
*/
struct Timestamp get_current_time(void);

/**
    * Check if used memory isn't bigger than MAX_MEMORY_SIZE.
    *
    * @return Returns 1 if there is memory available, else
    * returns 0.
*/
uint8_t is_enough_memory(unsigned long long newMemory);

/**
    * Check if file count is smaller than MAX_FILE_COUNT.
    *
    * @return Returns 1 if file count is within limit, else
    * returns 0.
*/
uint8_t is_file_count_within_limit();

#endif //FILE_H