#ifndef WSFS_H
#define WSFS_H

int wsfs_init();

struct FileNode* create_root_dir();

struct FileNode* create_file(struct FileNode* parent);

void add_file_to_dir(const struct FileNode* parent, struct FileNode* child);

void print_file_info(struct FileNode* file);

#endif //WSFS_H