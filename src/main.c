/**
    * @file: main.c
    * @author: without eyes
    *
    * This file contains main function which starts WSFS.
*/

#include <stdlib.h>

#include "../include/wsfs.h"

int main(void) {
    if (wsfs_init() == EXIT_SUCCESS) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}