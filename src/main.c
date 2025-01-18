#include <stdlib.h>

#include "../include/ramfs.h"

int main(void) {
    if (ramfs_init() == EXIT_SUCCESS) return EXIT_SUCCESS;
    return EXIT_FAILURE;
}