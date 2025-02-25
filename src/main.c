/**
    * @file: main.c
    * @author: without eyes
    *
    * This file contains main function which runs WSFS.
*/

#include "../include/wsfs.h"
#include "../include/ui.h"

int main(void) {
    struct FileNode* root = wsfs_init();
    set_root_node(root);
    run_ui(root);
    wsfs_deinit(root);
    return 0;
}