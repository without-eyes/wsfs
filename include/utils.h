/**
    * @file: utils.h
    * @author: without eyes
    *
    * This file contains functions that is not related to other
    * files.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include "../include/file_node_funcs.h"
#define BUFFER_SIZE 1024
#define END_OF_FILE_LINE "EOF"

/**
    * Retrieves the current local time (hour and minute).
    *
    * @return Returns the current hour and minute.
*/
struct Timestamp get_current_time(void);

/**
    * Reads user input until he input "EOF".
    *
    * @return Returns inputted text.
*/
char* read_all_user_input(void);

/**
    * Reads inputted line.
    *
    * @param[in, out] buffer The buffer where user input will
    * be placed.
    * @param[in] size The size of user input.
    *
    * @pre size > 0
*/
void read_line(char* buffer, size_t size);

#endif //UTILS_H
