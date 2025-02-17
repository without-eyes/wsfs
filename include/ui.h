/**
    * @file: ui.h
    * @author: without eyes
    *
    * This file contains functions related to user
    * interface.
*/

#ifndef UI_H
#define UI_H

#include "file_node_structs.h"

/**
    * Starts the user interface with the specified current
    * directory.
    *
    * @param[in] currentDir The current working
    * directory.
    *
    * @pre currentDir != NULL
*/
void run_ui(struct FileNode* currentDir);

/**
    * Prints file information in Linux's "ls -l"-like format.
    *
    * @param[in] node The file whose information will be
    * printed.
    *
    * @pre node != NULL
    *
    * @note The output will be printed to stdout in a format similar to:
    *       f 42 00:01 test_file
*/
void print_file_info(const struct FileNode* node);

/**
    * Prints directory's and it's content's information.
    *
    * @param[in] directory The directory which content information
    * will be printed.
    *
    * @pre directory != NULL
*/
void print_dir_content(const struct FileNode* directory);

/**
    * Prints list of commands.
*/
void print_help();

/**
    * Handles file node creation.
    *
    * @param[in] currentDir The directory where user is currently
    * located.
    * @param[in] type The type of file node that will be created.
    *
    * @pre currentDir != NULL
*/
void handle_create(struct FileNode* currentDir, enum FileType type);

/**
    * Handles reading file / writing into file.
    *
    * @param[in] currentDir The directory where user is currently
    * located.
    * @param[in] mode The mode of working with file(read/write).
    *
    * @pre currentDir != NULL
*/
void handle_read_write(const struct FileNode* currentDir, char mode);

#endif //UI_H
