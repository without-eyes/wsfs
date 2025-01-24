#ifndef WSFS_H
#define WSFS_H

#include "file_structs.h"

int wsfs_init(void);

void print_file_info(const struct FileNode* file);

void print_dir_content(const struct FileNode* directory);

#endif //WSFS_H