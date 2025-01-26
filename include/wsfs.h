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
    * Print file information in Linux's "ls -l"-like format.
    *
    * @param[in] file The file which information will be
    * printed.
    *
    * @pre file != NULL
*/
void print_file_info(const struct FileNode* file);

/**
    * Print directory information and it's content information.
    *
    * @param[in] directory The directory which content information
    * will be printed.
    *
    * @pre directory != NULL
*/
void print_dir_content(const struct FileNode* directory);

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