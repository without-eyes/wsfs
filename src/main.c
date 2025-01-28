/**
    * @file: main.c
    * @author: without eyes
    *
    * This file contains main function which runs WSFS.
*/

#include <stdlib.h>

#include "../include/wsfs.h"
#include "../include/ui.h"

int main(void) {
    struct FileNode* rootDir = wsfs_init();
    run_ui(rootDir);
    wsfs_deinit(rootDir);
    return EXIT_SUCCESS;
}