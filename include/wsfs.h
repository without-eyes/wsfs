/**
    * @file: wsfs.h
    * @author: without eyes
    *
    * This file contains functions related to Without eyeS's
    * File System(WSFS).
*/

#ifndef WSFS_H
#define WSFS_H

#include "file_node_funcs.h"

/**
    * Starts file system.
    *
    * @return Returns root directory.
*/
struct FileNode* wsfs_init(void);

/**
    * Free memory after program ends.
    *
    * @param[in] root The root directory.
*/
void wsfs_deinit(struct FileNode* root);

#endif //WSFS_H