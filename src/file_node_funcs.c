/**
    * @file: file_node_funcs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to files and file nodes.
*/

#include "../include/file_node_funcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"

struct FileNode* create_file_node(struct FileNode* parent, const char* name, const enum FileType type, const enum Permissions permissions) {
    struct FileNode* fileNode = malloc(sizeof(struct FileNode));
    fileNode->attributes.name = strdup(name);
    fileNode->attributes.createdAt = get_current_time();
    fileNode->attributes.type = type;
    fileNode->attributes.permissions = permissions;
    fileNode->attributes.directoryContent = NULL;
    fileNode->attributes.fileContent = NULL;
    fileNode->attributes.symlinkTarget = NULL;
    fileNode->next = NULL;
    fileNode->parent = strcmp(name, "\\") == 0 ? fileNode : parent;
    if (parent != NULL) add_to_dir(parent, fileNode);
    return fileNode;
}

void change_permissions(struct FileNode* node, const enum Permissions permissions) {
    if (node == NULL) return;

    node->attributes.permissions = permissions;
}

int is_permissions_equal(const enum Permissions left, const enum Permissions right) {
    return (left & right) == right;
}

size_t get_file_node_size(const struct FileNode* node) {
    if (node == NULL ||
        !is_permissions_equal(node->attributes.permissions, PERM_READ)) return 0;

    size_t total_size = sizeof(struct FileNode);

    if (node->attributes.name) {
        total_size += strlen(node->attributes.name) + 1;
    }

    if (node->attributes.type == FILE_TYPE_FILE && node->attributes.fileContent) {
        total_size += strlen(node->attributes.fileContent) + 1;
    }

    if (node->attributes.type == FILE_TYPE_DIR) {
        const struct FileNode* child = node->attributes.directoryContent;
        while (child != NULL) {
            total_size += get_file_node_size(child);
            child = child->next;
        }
    }

    return total_size;
}

void change_current_dir(struct FileNode** currentDir, struct FileNode* newCurrentDir) {
    if (*currentDir == NULL || newCurrentDir == NULL ||
        !is_permissions_equal(newCurrentDir->attributes.permissions, PERM_READ) ||
        !is_permissions_equal(newCurrentDir->attributes.permissions, PERM_EXEC)) return;

    newCurrentDir = get_symlink_target(newCurrentDir);

    *currentDir = newCurrentDir;
}

void add_to_dir(struct FileNode* parent, struct FileNode* child) {
    if (parent == NULL || child == NULL ||
        !is_permissions_equal(parent->attributes.permissions, PERM_WRITE)) return;

    if (parent->attributes.directoryContent == NULL) {
        parent->attributes.directoryContent = child;
        return;
    }

    struct FileNode* current = parent->attributes.directoryContent;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = child;
}

char get_file_type_letter(const enum FileType type) {
    switch (type) {
        case FILE_TYPE_DIR:         return 'd';
        case FILE_TYPE_FILE:        return 'f';
        case FILE_TYPE_SYMLINK:     return 's';
        case FILE_TYPE_UNKNOWN:     return '-';
        default:                    return '?';
    }
}

char get_permission_letter(const enum Permissions permission) {
    switch (permission) {
        case PERM_READ:     return 'r';
        case PERM_WRITE:    return 'w';
        case PERM_EXEC:     return 'x';
        case PERM_NONE:     return '-';
        default:            return '?';
    }
}

void set_symlink_target(struct FileNode* symlink, struct FileNode* target) {
    if (symlink == NULL || target == NULL ||
        !is_permissions_equal(symlink->attributes.permissions, PERM_WRITE)) return;

    symlink->attributes.symlinkTarget = target;
}

struct FileNode* get_symlink_target(struct FileNode* symlink) {
    if (symlink == NULL ||
        !is_permissions_equal(symlink->attributes.permissions, PERM_READ)) return NULL;

    struct FileNode* current = symlink;
    while (current != NULL && current->attributes.type == FILE_TYPE_SYMLINK) {
        current = current->attributes.symlinkTarget;
    }

    return current;
}

void write_to_file(struct FileNode* node, const char* content) {
    if (node == NULL || content == NULL ||
        !is_permissions_equal(node->attributes.permissions, PERM_WRITE)) return;

    struct FileNode* current = get_symlink_target(node);

    free(current->attributes.fileContent);
    current->attributes.fileContent = malloc(strlen(content) + 1);
    strcpy(current->attributes.fileContent, content);
}

char* read_file_content(struct FileNode* node) {
    if (node == NULL ||
        !is_permissions_equal(node->attributes.permissions, PERM_READ)) return NULL;

    const struct FileNode* current = get_symlink_target(node);

    return current->attributes.fileContent;
}

struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* name) {
    if (currentDir == NULL || name == NULL ||
        !is_permissions_equal(currentDir->attributes.permissions, PERM_READ) ||
        !is_permissions_equal(currentDir->attributes.permissions, PERM_EXEC)) return NULL;

    struct FileNode* current = currentDir->attributes.directoryContent;
    while (current != NULL && strcmp(current->attributes.name, name) != 0) {
        current = current->next;
    }

    return current;
}

struct FileNode* find_file_node_in_fs(const struct FileNode* root, const char* name) {
    if (root == NULL || name == NULL) return NULL;

    if (strcmp(root->attributes.name, name) == 0) {
        return (struct FileNode*)root;
    }

    const struct FileNode* current = root->attributes.directoryContent;
    while (current) {
        struct FileNode* result = find_file_node_in_fs(current, name);
        if (result != NULL) return result;
        current = current->next;
    }

    return NULL;
}

char* get_file_node_path(const struct FileNode* node) {
    if (node == NULL) return NULL;

    const struct FileNode* temp = node;
    size_t pathLength = 1;
    while (strcmp(temp->attributes.name, "\\") != 0) {
        pathLength += strlen(temp->attributes.name) + 1;
        temp = temp->parent;
    }

    char* path = malloc(pathLength);
    path[pathLength - 1] = '\0';
    size_t pos = pathLength - 1;
    const struct FileNode* current = node;
    while (strcmp(current->attributes.name, "\\") != 0) {
        const size_t nameLen = strlen(current->attributes.name);
        pos -= nameLen;
        memcpy(&path[pos], current->attributes.name, nameLen);
        if (pos > 0) {
            path[--pos] = '\\';
        }
        current = current->parent;
    }

    return path;
}

void change_file_node_location(struct FileNode* location, struct FileNode* node) {
    if (node == NULL || location == NULL ||
        !is_permissions_equal(location->attributes.permissions, PERM_WRITE) ||
        !is_permissions_equal(node->attributes.permissions, PERM_WRITE)) return;
    if (node->parent == location) return;

    if (node->parent != NULL) {
        struct FileNode** prev_ptr = &node->parent->attributes.directoryContent;

        while (*prev_ptr && *prev_ptr != node) {
            prev_ptr = &(*prev_ptr)->next;
        }

        if (*prev_ptr == node) {
            *prev_ptr = node->next;
        }
    }

    node->next = NULL;
    node->parent = location;
    add_to_dir(location, node);
}

void change_file_node_name(struct FileNode* node, const char* name) {
    if (!is_permissions_equal(node->attributes.permissions, PERM_WRITE)) return;

    free(node->attributes.name);
    node->attributes.name = malloc(strlen(name) + 1);
    strcpy(node->attributes.name, name);
}

void delete_file_node(struct FileNode* currentDir, struct FileNode* node) {
    if (currentDir == NULL || node == NULL) return;

    struct FileNode* currentFileNode = currentDir->attributes.directoryContent;
    if (currentFileNode == node) {
        currentDir->attributes.directoryContent = currentDir->attributes.directoryContent->next;
        free_file_node_recursive(node);
        return;
    }

    while (currentFileNode->next != node) {
        currentFileNode = currentFileNode->next;
    }

    currentFileNode->next = currentFileNode->next->next;
    free_file_node_recursive(node);
}

void free_file_node_recursive(struct FileNode* fileNode) {
    if (fileNode == NULL) return;

    if (fileNode->attributes.type == FILE_TYPE_DIR) {
        struct FileNode* current = fileNode->attributes.directoryContent;
        while (current != NULL) {
            struct FileNode* next = current->next;
            free_file_node_recursive(current);
            current = next;
        }
    }

    if (fileNode->attributes.type == FILE_TYPE_FILE) {
        free(fileNode->attributes.fileContent);
    }
    free(fileNode->attributes.name);
    free(fileNode);
}