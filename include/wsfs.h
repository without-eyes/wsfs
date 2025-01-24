#ifndef WSFS_H
#define WSFS_H

int wsfs_init();

struct FileNode* create_root_dir();

struct FileNode* create_file(struct FileNode* parent);

void add_file_to_dir(struct FileNode* parent, struct FileNode* child);

void print_file_info(const struct FileNode* file);

void print_dir_content(const struct FileNode* directory);

#endif //WSFS_H