/**
    * @file: wsfs.h
    * @author: without eyes
    *
    * This file contains functions related to Without eyeS's
    * File System(WSFS).
*/

#ifndef WSFS_H
#define WSFS_H

#include "file_structs.h"

/**
    * Start, run in loop and free file system.
    *
    * @return Returns EXIT_SUCCESS is everything worked well.
*/
int wsfs_init(void);

/**
    * Close UI, free memory and end program.
    *
    * @param[in] rootDir The root directory.
    *
    * @pre rootDir != NULL
*/
void wsfs_deinit(struct FileNode* rootDir);

/**
    * Change current directory.
    *
    * @param[in,out] currentDir The directory where user is currently
    * located.
    * @param[in] dirName The name of the child directory where user
    * wants to go.
    *
    * @pre currentDir != NULL && dirName != NULL
*/
void change_current_dir(struct FileNode** currentDir, const char* dirName);

#endif //WSFS_H