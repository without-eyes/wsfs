/**
    * @file: wsfs.c
    * @author: without eyes
    *
    * This file contains definition of functions related
    * to Without eyeS's File System(WSFS).
*/

#include "../include/wsfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FileNode* wsfs_init(void) {
    struct FileNode* root = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    set_root_node(root);
    return root;
}

void wsfs_deinit(struct FileNode* root) {
    free_file_node_recursive(root);
}