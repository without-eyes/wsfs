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

#include "../include/file_structs.h"

/**
    * Retrieves the current local time (hour and minute).
    *
    * @return Returns a struct Timestamp containing the current
    * hour and minute.
*/
struct Timestamp get_current_time(void);

/**
    * Read user input until he input "EOF".
    *
    * @return Returns inputted text.
*/
char* read_user_input(void);

/**
    * Read user input.
    *
    * @param[in, out] buffer The buffer where user input will be placed.
    * @param[in] size The size of user input.
    *
    * @pre size > 0
*/
void read_input(char* buffer, size_t size);

#endif //UTILS_H
