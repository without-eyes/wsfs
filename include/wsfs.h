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
    * @return Returns root directory as pointer
    * to struct FileNode.
*/
struct FileNode* wsfs_init(void);

/**
    * Free memory after program ends. In almost most cases
    * you should pass root directory as parameter.
    *
    * @param[in] fileNode The file node that needs
    * to be free.
*/
void wsfs_deinit(struct FileNode* fileNode);

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