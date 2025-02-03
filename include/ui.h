/**
    * @file: ui.h
    * @author: without eyes
    *
    * This file contains functions related to user
    * interface.
*/

#ifndef UI_H
#define UI_H

#include "../include/wsfs.h"

/**
    * Starts user interface.
    *
    * @param[in] currentDir The current working
    * directory.
    *
    * @pre currentDir != NULL
*/
void run_ui(struct FileNode* currentDir);

/**
    * Print file information in Linux's "ls -l"-like format.
    *
    * @param[in] fileNode The file which information will be
    * printed.
    *
    * @pre fileNode != NULL
*/
void print_file_info(const struct FileNode* fileNode);

/**
    * Print directory information and it's content information.
    *
    * @param[in] directory The directory which content information
    * will be printed.
    *
    * @pre directory != NULL
*/
void print_dir_content(const struct FileNode* directory);

#endif //UI_H
