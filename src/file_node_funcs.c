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
#include <time.h>

struct FileNode* create_file_node(struct FileNode* parent, const char* name, const enum FileType type, const enum Permissions permissions) {
    struct FileNode* node = malloc(sizeof(struct FileNode));
    if (node == NULL) return NULL;

    node->info.metadata.name = name != NULL ? strdup(name) : strdup("?");
    node->info.metadata.creationTime = get_current_time();
    node->info.properties.type = type;
    node->info.properties.permissions = permissions;
    node->info.data.directoryContent = NULL;
    node->info.data.fileContent = NULL;
    node->info.data.symlinkTarget = NULL;
    node->next = NULL;
    node->parent = strcmp(name, "\\") == 0 ? node : parent;
    if (parent != node) add_to_dir(parent, node);

    return node;
}

uint8_t change_permissions(struct FileNode* node, const enum Permissions permissions) {
    if (node == NULL) return EXIT_FAILURE;

    node->info.properties.permissions = permissions;

    return EXIT_SUCCESS;
}

uint8_t is_permissions_equal(const enum Permissions left, const enum Permissions right) {
    return (left & right) == right;
}

size_t get_file_node_size(const struct FileNode* node) {
    if (node == NULL ||
        !is_permissions_equal(node->info.properties.permissions, PERM_READ)) {
        return 0;
    }

    size_t totalSize = 0;
    const struct FileNode* stack[512];
    int top = -1;

    stack[++top] = node;

    while (top >= 0) {
        const struct FileNode* topNode = stack[top--];

        if (topNode == NULL) continue;

        totalSize += sizeof(struct FileNode);

        if (topNode->info.metadata.name) {
            totalSize += strlen(topNode->info.metadata.name) + 1;
        }

        if (topNode->info.properties.type == FILE_TYPE_FILE && topNode->info.data.fileContent) {
            totalSize += strlen(topNode->info.data.fileContent) + 1;
        }

        if (topNode->info.properties.type == FILE_TYPE_DIR) {
            const struct FileNode* child = topNode->info.data.directoryContent;
            while (child != NULL) {
                stack[++top] = child;
                child = child->next;
            }
        }
    }

    return totalSize;
}

uint8_t change_current_dir(struct FileNode** currentDir, struct FileNode* newCurrentDir) {
    if (*currentDir == NULL || newCurrentDir == NULL ||
        !is_permissions_equal(newCurrentDir->info.properties.permissions, PERM_READ) ||
        !is_permissions_equal(newCurrentDir->info.properties.permissions, PERM_EXEC)) return EXIT_FAILURE;

    newCurrentDir = get_symlink_target(newCurrentDir);

    *currentDir = newCurrentDir;

    return EXIT_SUCCESS;
}

uint8_t add_to_dir(struct FileNode* restrict parent, struct FileNode* restrict child) {
    if (parent == NULL || child == NULL ||
        !is_permissions_equal(parent->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    if (parent->info.data.directoryContent == NULL) {
        parent->info.data.directoryContent = child;
        return EXIT_FAILURE;
    }

    struct FileNode* current = parent->info.data.directoryContent;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = child;

    return EXIT_SUCCESS;
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

uint8_t set_symlink_target(struct FileNode* symlink, struct FileNode* target) {
    if (symlink == NULL || target == NULL ||
        !is_permissions_equal(symlink->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    symlink->info.data.symlinkTarget = target;

    return EXIT_SUCCESS;
}

struct FileNode* get_symlink_target(struct FileNode* symlink) {
    if (symlink == NULL ||
        !is_permissions_equal(symlink->info.properties.permissions, PERM_READ)) return NULL;

    struct FileNode* current = symlink;
    while (current != NULL && current->info.properties.type == FILE_TYPE_SYMLINK) {
        current = current->info.data.symlinkTarget;
    }

    return current;
}

uint8_t write_to_file(struct FileNode* node, const char* content) {
    if (node == NULL || content == NULL ||
        !is_permissions_equal(node->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    struct FileNode* current = get_symlink_target(node);

    free(current->info.data.fileContent);
    current->info.data.fileContent = strdup(content);
    if (current->info.data.fileContent == NULL) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

char* read_file_content(struct FileNode* node) {
    if (node == NULL ||
        !is_permissions_equal(node->info.properties.permissions, PERM_READ)) return NULL;

    const struct FileNode* current = get_symlink_target(node);

    return current->info.data.fileContent;
}

struct FileNode* find_file_node_in_curr_dir(const struct FileNode* currentDir, const char* name) {
    if (currentDir == NULL || name == NULL ||
        !is_permissions_equal(currentDir->info.properties.permissions, PERM_READ) ||
        !is_permissions_equal(currentDir->info.properties.permissions, PERM_EXEC)) return NULL;

    struct FileNode* current = currentDir->info.data.directoryContent;
    while (current != NULL && strcmp(current->info.metadata.name, name) != 0) {
        current = current->next;
    }

    return current;
}

struct FileNode* find_file_node_in_fs(const struct FileNode* root, const char* name) {
    if (root == NULL || name == NULL) return NULL;

    const struct FileNode* stack[512];
    int top = -1;

    stack[++top] = root;

    while (top >= 0) {
        const struct FileNode* node = stack[top--];

        if (node == NULL) continue;

        if (strcmp(node->info.metadata.name, name) == 0) {
            return (struct FileNode*)node;
        }

        if (node->info.properties.type == FILE_TYPE_DIR) {
            const struct FileNode* child = node->info.data.directoryContent;
            while (child != NULL) {
                stack[++top] = child;
                child = child->next;
            }
        }
    }

    return NULL;
}

char* get_file_node_path(const struct FileNode* node) {
    if (node == NULL) return NULL;

    const struct FileNode* temp = node;
    size_t pathLength = 1;
    while (strcmp(temp->info.metadata.name, "\\") != 0) {
        pathLength += strlen(temp->info.metadata.name) + 1;
        temp = temp->parent;
    }

    char* path = malloc(pathLength);
    path[pathLength - 1] = '\0';
    size_t pos = pathLength - 1;
    const struct FileNode* current = node;
    while (strcmp(current->info.metadata.name, "\\") != 0) {
        const size_t nameLen = strlen(current->info.metadata.name);
        pos -= nameLen;
        memcpy(&path[pos], current->info.metadata.name, nameLen);
        if (pos > 0) {
            path[--pos] = '\\';
        }
        current = current->parent;
    }

    return path;
}

uint8_t change_file_node_location(struct FileNode* restrict location, struct FileNode* restrict node) {
    if (node == NULL || location == NULL ||
        node->parent == location ||
        !is_permissions_equal(location->info.properties.permissions, PERM_WRITE) ||
        !is_permissions_equal(node->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    if (node->parent != NULL) {
        struct FileNode** prev_ptr = &node->parent->info.data.directoryContent;

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

    return EXIT_SUCCESS;
}

uint8_t copy_file_node(struct FileNode* restrict location, const struct FileNode* restrict node) {
    if (location == NULL || node == NULL ||
        location->info.properties.type != FILE_TYPE_DIR ||
        !is_permissions_equal(location->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    struct FileNode* nodeCopy = malloc(sizeof(struct FileNode));
    if (nodeCopy == NULL) return EXIT_FAILURE;
    memcpy(nodeCopy, node, sizeof(struct FileNode));

    nodeCopy->info.metadata.name = NULL;
    nodeCopy->info.data.fileContent = NULL;
    nodeCopy->info.data.directoryContent = NULL;
    nodeCopy->next = NULL;

    if (node->info.metadata.name != NULL) {
        nodeCopy->info.metadata.name = strdup(node->info.metadata.name);
    }

    if (node->info.properties.type == FILE_TYPE_FILE && node->info.data.fileContent != NULL) {
        nodeCopy->info.data.fileContent = strdup(node->info.data.fileContent);
    }

    nodeCopy->parent = location;
    add_to_dir(location, nodeCopy);

    if (node->info.properties.type == FILE_TYPE_DIR) {
        struct FileNode* child = node->info.data.directoryContent;
        struct FileNode* prevCopy = NULL;

        while (child != NULL) {
            struct FileNode* childCopy = malloc(sizeof(struct FileNode));
            if (childCopy == NULL) return EXIT_FAILURE;
            memcpy(childCopy, child, sizeof(struct FileNode));

            childCopy->info.metadata.name = NULL;
            childCopy->info.data.fileContent = NULL;
            childCopy->info.data.directoryContent = NULL;
            childCopy->next = NULL;

            if (child->info.metadata.name != NULL) {
                childCopy->info.metadata.name = strdup(child->info.metadata.name);
            }

            if (child->info.properties.type == FILE_TYPE_FILE && child->info.data.fileContent != NULL) {
                childCopy->info.data.fileContent = strdup(child->info.data.fileContent);
            }

            childCopy->parent = nodeCopy;

            if (prevCopy == NULL) {
                nodeCopy->info.data.directoryContent = childCopy;
            } else {
                prevCopy->next = childCopy;
            }

            prevCopy = childCopy;
            child = child->next;
        }
    }

    return EXIT_SUCCESS;
}

uint8_t change_file_node_name(struct FileNode* node, const char* name) {
    if (!is_permissions_equal(node->info.properties.permissions, PERM_WRITE)) return EXIT_FAILURE;

    free(node->info.metadata.name);
    node->info.metadata.name = strdup(name);
    if (node->info.metadata.name == NULL) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

uint8_t delete_file_node(struct FileNode* restrict currentDir, struct FileNode* restrict node) {
    if (currentDir == NULL || node == NULL) return EXIT_FAILURE;

    struct FileNode* currentFileNode = currentDir->info.data.directoryContent;
    if (currentFileNode == node) {
        currentDir->info.data.directoryContent = currentDir->info.data.directoryContent->next;
        free_file_node_recursive(node);
        return EXIT_FAILURE;
    }

    while (currentFileNode->next != node) {
        currentFileNode = currentFileNode->next;
    }

    currentFileNode->next = currentFileNode->next->next;
    free_file_node_recursive(node);

    return EXIT_SUCCESS;
}

uint8_t free_file_node_recursive(struct FileNode* node) {
    if (node == NULL) return EXIT_FAILURE;

    struct FileNode* stack[512];
    int top = -1;

    stack[++top] = node;

    while (top >= 0) {
        struct FileNode* topNode = stack[top--];

        if (topNode->info.properties.type == FILE_TYPE_DIR) {
            struct FileNode* child = topNode->info.data.directoryContent;
            while (child != NULL) {
                struct FileNode* next = child->next;
                stack[++top] = child;
                child = next;
            }
        }

        if (topNode->info.properties.type == FILE_TYPE_FILE) {
            free(topNode->info.data.fileContent);
        }
        free(topNode->info.metadata.name);
        free(topNode);
    }

    return EXIT_SUCCESS;
}

struct Timestamp get_current_time(void) {
    time_t rawTime;
    time(&rawTime);
    const struct tm* timeInfo = localtime(&rawTime);

    struct Timestamp currentTime;
    currentTime.year = timeInfo->tm_year + 1900;
    currentTime.month = timeInfo->tm_mon + 1;
    currentTime.day = timeInfo->tm_mday;
    currentTime.hour = timeInfo->tm_hour;
    currentTime.minute = timeInfo->tm_min;

    return currentTime;
}