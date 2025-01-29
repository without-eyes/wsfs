/**
    * @file: utils.h
    * @author: without eyes
    *
    * This file contains functions that is not related to other
    * files.
*/

#ifndef UTILS_H
#define UTILS_H

#include "../include/file_structs.h"

/**
    * Retrieves the current local time (hour and minute).
    *
    * @return Returns a struct Timestamp containing the current
    * hour and minute.
*/
struct Timestamp get_current_time(void);

#endif //UTILS_H
