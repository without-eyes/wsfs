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
    struct FileNode* currentDir = wsfs_init();
    run_ui(currentDir);
    wsfs_deinit(currentDir);
    return EXIT_SUCCESS;
}