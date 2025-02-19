/**
    * @file: file_node_structs.h
    * @author: without eyes
    *
    * This file contains structs related to files and file nodes.
*/

#ifndef FILE_NODE_STRUCTS_H
#define FILE_NODE_STRUCTS_H

#include <stdint.h>

/**
 * @enum FileType
 * @brief Represents different types of files.
 */
enum __attribute__ ((__packed__)) FileType {
    FILE_TYPE_UNKNOWN = 0, /**< Unknown file type */
    FILE_TYPE_FILE = 1,    /**< Regular file */
    FILE_TYPE_DIR = 2,     /**< Directory */
    FILE_TYPE_SYMLINK = 3  /**< Symbolic link */
};

/**
 * @enum Permissions
 * @brief Defines file access permissions.
 */
enum __attribute__ ((__packed__)) Permissions {
    PERM_NONE = 0,  /**< No permissions */
    PERM_EXEC = 1,  /**< Execute permission */
    PERM_WRITE = 2, /**< Write permission */
    PERM_READ = 4   /**< Read permission */
};

struct FileNode; /**< Forward declaration of FileNode struct */

/**
 * @struct Timestamp
 * @brief Represents a time value with hour and minute.
 */
struct Timestamp {
    uint8_t hour;   /**< Hour value (0-23) */
    uint8_t minute; /**< Minute value (0-59) */
};

/**
 * @struct FileMetadata
 * @brief Contains metadata related to a file.
 */
struct FileMetadata {
    char* name;                     /**< Name of the file */
    struct Timestamp creationTime;  /**< Timestamp of file creation */
};

/**
 * @struct FileProperties
 * @brief Stores file type and permissions.
 */
struct FileProperties {
    enum FileType type;             /**< Type of the file */
    enum Permissions permissions;   /**< File permissions */
};

/**
 * @struct FileData
 * @brief Represents the content of a file.
 */
struct FileData {
    union {
        struct FileNode* directoryContent; /**< Pointer to directory content (if directory) */
        struct FileNode* symlinkTarget;    /**< Pointer to symbolic link target (if symlink) */
        char* fileContent;                 /**< Pointer to file content (if regular file) */
    };
};

/**
 * @struct FileInfo
 * @brief Holds complete information about a file.
 */
struct FileInfo {
    struct FileMetadata metadata;       /**< Metadata of the file */
    struct FileProperties properties;   /**< Properties such as type and permissions */
    struct FileData data;               /**< File data/content */
};

/**
 * @struct FileNode
 * @brief Represents a file node in the file system.
 */
struct FileNode {
    struct FileInfo info;      /**< Information about the file */
    struct FileNode* parent;   /**< Pointer to the parent node */
    struct FileNode* next;     /**< Pointer to the next node */
};

#endif //FILE_NODE_STRUCTS_H
