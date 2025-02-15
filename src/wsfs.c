/**
    * @file: wsfs.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to Without eyeS's File System(WSFS).
*/

#include "../include/wsfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileNode* wsfs_init(void) {
    return create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
}

void wsfs_deinit(struct FileNode* root) {
    free_file_node_recursive(root);
}