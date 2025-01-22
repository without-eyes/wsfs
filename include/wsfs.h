#ifndef WSFS_H
#define WSFS_H

int wsfs_init();

struct FileNode* create_root_dir();

void print_file_info(struct FileNode* file);

#endif //WSFS_H