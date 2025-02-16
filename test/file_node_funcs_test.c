/**
    * @file: file_node_funcs_test.c
    * @author: without eyes
    *
    * This file contains tests for functions related
    * to files and file nodes.
*/

#include "../include/file_node_funcs.h"

#include <stdio.h>

#include "criterion/criterion.h"
#include "../include/utils.h"

Test(create_file_node, basic_creation) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    char name[5] = "node";
    enum FileType type = FILE_TYPE_FILE;
    enum Permissions permissions = PERM_NONE;

    struct FileNode* child = create_file_node(parent, name, type, permissions);

    struct Timestamp timestamp = get_current_time();
    cr_assert_str_eq(child->attributes.name,            name);
    cr_assert_eq(child->attributes.createdAt.hour,      timestamp.hour);
    cr_assert_eq(child->attributes.createdAt.minute,    timestamp.minute);
    cr_assert_eq(child->attributes.type,                type);
    cr_assert_eq(child->attributes.permissions,         permissions);
    cr_assert_eq(child->parent,                         parent);
    cr_assert_null(child->attributes.directoryContent);
    cr_assert_null(child->attributes.fileContent);
    cr_assert_null(child->attributes.symlinkTarget);
    cr_assert_null(child->next);

    free(child->attributes.name);
    free(child);
    free(parent->attributes.name);
    free(parent);
}

Test(create_file_node, root_creation) {
    char name[2] = "\\";
    enum FileType type = FILE_TYPE_DIR;
    enum Permissions permissions = PERM_NONE;

    struct FileNode* root = create_file_node(NULL, name, type, permissions);

    struct Timestamp timestamp = get_current_time();
    cr_assert_str_eq(root->attributes.name,             name);
    cr_assert_eq(root->attributes.createdAt.hour,       timestamp.hour);
    cr_assert_eq(root->attributes.createdAt.minute,     timestamp.minute);
    cr_assert_eq(root->attributes.type,                 type);
    cr_assert_eq(root->attributes.permissions,          permissions);
    cr_assert_eq(root->parent,                          root);
    cr_assert_null(root->attributes.directoryContent);
    cr_assert_null(root->attributes.fileContent);
    cr_assert_null(root->attributes.symlinkTarget);
    cr_assert_null(root->next);

    free(root->attributes.name);
    free(root);
}

Test(change_permissions, change_valid_node_permissions) {
    struct FileNode node;
    node.attributes.permissions = PERM_NONE;

    change_permissions(&node, PERM_READ);

    cr_assert_eq(node.attributes.permissions, PERM_READ);
}

Test(change_permissions, null_node_no_change) {
    change_permissions(NULL, PERM_NONE);

    // No assertion needed; just ensuring it does not crash
}

Test(change_permissions, change_multiple_perms) {
    struct FileNode node;
    node.attributes.permissions = PERM_NONE;

    change_permissions(&node, PERM_READ | PERM_WRITE);

    cr_assert_eq(node.attributes.permissions, PERM_READ | PERM_WRITE);
}

Test(is_permissions_equal, all) {
    cr_assert_eq(is_permissions_equal(PERM_NONE, PERM_NONE), 1);
    cr_assert_eq(is_permissions_equal(PERM_READ | PERM_WRITE, PERM_WRITE | PERM_READ), 1);
    cr_assert_eq(is_permissions_equal(PERM_READ, PERM_WRITE), 0);
}

Test(get_file_node_size, null_node) {
    cr_assert_eq(get_file_node_size(NULL), 0);
}

Test(get_file_node_size, empty_file_node) {
    struct FileNode* node = create_file_node(NULL, "", FILE_TYPE_FILE, PERM_READ);
    const size_t expected_size = sizeof(struct FileNode) + strlen(node->attributes.name) + 1;
    cr_assert_eq(get_file_node_size(node), expected_size);
    free(node);
}

Test(get_file_node_size, file_with_content) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_READ | PERM_WRITE);
    write_to_file(node, "Hello");
    const size_t expected_size = sizeof(struct FileNode) + strlen("file") + 1 + strlen("Hello") + 1;

    cr_assert_eq(get_file_node_size(node), expected_size);

    free(node->attributes.name);
    free(node->attributes.fileContent);
    free(node);
}

Test(get_file_node_size, file_without_permissions) {
    struct FileNode* node = create_file_node(NULL, "", FILE_TYPE_FILE, PERM_NONE);
    cr_assert_eq(get_file_node_size(node), 0);
    free(node);
}

Test(get_file_node_size, directory_with_files) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* file1 = create_file_node(dir, "file1", FILE_TYPE_FILE, PERM_READ | PERM_WRITE);
    write_to_file(file1, "Hello");
    struct FileNode* file2 = create_file_node(dir, "file2", FILE_TYPE_FILE, PERM_READ | PERM_WRITE);
    write_to_file(file2, "World");

    size_t expected_size = sizeof(struct FileNode) + strlen("dir") + 1;
    expected_size += sizeof(struct FileNode) + strlen("file1") + 1 + strlen("Hello") + 1;
    expected_size += sizeof(struct FileNode) + strlen("file2") + 1 + strlen("World") + 1;

    cr_assert_eq(get_file_node_size(dir), expected_size);

    free(file1->attributes.name);
    free(file1->attributes.fileContent);
    free(file1);
    free(file2->attributes.name);
    free(file2->attributes.fileContent);
    free(file2);
    free(dir->attributes.name);
    free(dir);
}

Test(change_current_dir, dir_exists) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR, PERM_READ | PERM_EXEC);

    change_current_dir(&currentDir, newCurrentDir);

    cr_assert_eq(currentDir, newCurrentDir);
    cr_assert_str_eq(currentDir->attributes.name,           newCurrentDir->attributes.name);
    cr_assert_eq(currentDir->attributes.createdAt.hour,     newCurrentDir->attributes.createdAt.hour);
    cr_assert_eq(currentDir->attributes.createdAt.minute,   newCurrentDir->attributes.createdAt.minute);
    cr_assert_eq(currentDir->attributes.type,               newCurrentDir->attributes.type);
    cr_assert_eq(currentDir->attributes.directoryContent,   newCurrentDir->attributes.directoryContent);
    cr_assert_eq(currentDir->attributes.fileContent,        newCurrentDir->attributes.fileContent);
    cr_assert_eq(currentDir->attributes.symlinkTarget,      newCurrentDir->attributes.symlinkTarget);
    cr_assert_eq(currentDir->next,                          newCurrentDir->next);
    cr_assert_eq(currentDir->parent,                        newCurrentDir->parent);

    free(currentDir->parent->attributes.name);
    free(currentDir->parent);
    free(currentDir->attributes.name);
    free(currentDir);
}

Test(change_current_dir, dir_not_exists) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_NONE);

    change_current_dir(&currentDir, NULL);

    cr_assert_str_eq(currentDir->attributes.name,           currentDir->attributes.name);
    cr_assert_eq(currentDir->attributes.createdAt.hour,     currentDir->attributes.createdAt.hour);
    cr_assert_eq(currentDir->attributes.createdAt.minute,   currentDir->attributes.createdAt.minute);
    cr_assert_eq(currentDir->attributes.type,               currentDir->attributes.type);
    cr_assert_eq(currentDir->attributes.directoryContent,   currentDir->attributes.directoryContent);
    cr_assert_eq(currentDir->attributes.fileContent,        currentDir->attributes.fileContent);
    cr_assert_eq(currentDir->attributes.symlinkTarget,      currentDir->attributes.symlinkTarget);
    cr_assert_eq(currentDir->next,                          currentDir->next);
    cr_assert_eq(currentDir->parent,                        currentDir->parent);

    free(currentDir->attributes.name);
    free(currentDir);
}

Test(change_current_dir, symlink) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* symlink = create_file_node(currentDir, "symlink", FILE_TYPE_SYMLINK, PERM_READ | PERM_WRITE | PERM_EXEC);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR, PERM_READ | PERM_EXEC);
    set_symlink_target(symlink, newCurrentDir);

    change_current_dir(&currentDir, symlink);

    cr_assert_str_eq(currentDir->attributes.name,           newCurrentDir->attributes.name);
    cr_assert_eq(currentDir->attributes.createdAt.hour,     newCurrentDir->attributes.createdAt.hour);
    cr_assert_eq(currentDir->attributes.createdAt.minute,   newCurrentDir->attributes.createdAt.minute);
    cr_assert_eq(currentDir->attributes.type,               newCurrentDir->attributes.type);
    cr_assert_eq(currentDir->attributes.directoryContent,   newCurrentDir->attributes.directoryContent);
    cr_assert_eq(currentDir->attributes.fileContent,        newCurrentDir->attributes.fileContent);
    cr_assert_eq(currentDir->attributes.symlinkTarget,      newCurrentDir->attributes.symlinkTarget);
    cr_assert_eq(currentDir->next,                          newCurrentDir->next);
    cr_assert_eq(currentDir->parent,                        newCurrentDir->parent);

    free(currentDir->parent->attributes.name);
    free(currentDir->parent);
    free(currentDir->attributes.name);
    free(currentDir);
}

Test(change_current_dir, dir_without_permissions) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR, PERM_NONE);
    struct FileNode* temp = currentDir;

    change_current_dir(&currentDir, newCurrentDir);

    cr_assert_neq(currentDir, newCurrentDir);

    free(temp->parent->attributes.name);
    free(temp->parent);
    free(newCurrentDir->attributes.name);
    free(newCurrentDir);
}

Test(add_to_dir, add_first_child_to_empty_directory) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE, PERM_NONE);

    add_to_dir(parent, child);

    cr_assert_eq(parent->attributes.directoryContent, child);
    cr_assert_null(child->next);

    free(parent->attributes.name);
    free(parent);
    free(child->attributes.name);
    free(child);
}

Test(add_to_dir, add_multiple_children) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* child1 = create_file_node(NULL, "child1", FILE_TYPE_FILE, PERM_NONE);
    struct FileNode* child2 = create_file_node(NULL, "child2", FILE_TYPE_FILE, PERM_NONE);

    add_to_dir(parent, child1);
    add_to_dir(parent, child2);

    cr_assert_eq(parent->attributes.directoryContent, child1);
    cr_assert_eq(child1->next, child2);
    cr_assert_null(child2->next);

    free(parent->attributes.name);
    free(parent);
    free(child1->attributes.name);
    free(child1);
    free(child2->attributes.name);
    free(child2);
}

Test(add_to_dir, add_null_child_does_nothing) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);

    add_to_dir(parent, NULL);

    cr_assert_null(parent->attributes.directoryContent);

    free(parent->attributes.name);
    free(parent);
}

Test(add_to_dir, add_child_to_null_parent_does_nothing) {
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE, PERM_NONE);

    add_to_dir(NULL, child);

    cr_assert_null(child->next);

    free(child->attributes.name);
    free(child);
}

Test(add_to_dir, add_to_directory_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR, PERM_NONE);
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE, PERM_NONE);

    add_to_dir(parent, child);

    cr_assert_null(parent->attributes.directoryContent);

    free(parent->attributes.name);
    free(parent);
    free(child->attributes.name);
    free(child);
}

Test(get_file_type_letter, all_types) {
    cr_assert_eq(get_file_type_letter(FILE_TYPE_DIR),       'd');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_FILE),      'f');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_SYMLINK),   's');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_UNKNOWN),   '-');
    cr_assert_eq(get_file_type_letter(999),                 '?');
}

Test(get_permission_letter, all_permissions) {
    cr_assert_eq(get_permission_letter(PERM_NONE),   '-');
    cr_assert_eq(get_permission_letter(PERM_READ),   'r');
    cr_assert_eq(get_permission_letter(PERM_WRITE),  'w');
    cr_assert_eq(get_permission_letter(PERM_EXEC),   'x');
    cr_assert_eq(get_permission_letter(999),         '?');
}

Test(set_symlink_target, valid_symlink_target) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_WRITE);
    struct FileNode* target = create_file_node(NULL, "target_file", FILE_TYPE_FILE, PERM_NONE);

    set_symlink_target(symlink, target);

    cr_assert_eq(symlink->attributes.symlinkTarget, target);
}

Test(set_symlink_target, set_symlink_target_to_null) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_WRITE);

    set_symlink_target(symlink, NULL);

    cr_assert_null(symlink->attributes.symlinkTarget);
}

Test(set_symlink_target, null_symlink_does_nothing) {
    struct FileNode* target = create_file_node(NULL, "target_file", FILE_TYPE_FILE, PERM_NONE);

    set_symlink_target(NULL, target);

    // No assertion needed; just ensuring it does not crash
}

Test(get_symlink_target, basic) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_READ | PERM_WRITE);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_NONE);
    set_symlink_target(symlink, target);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_eq(result, target);

    free(target->attributes.name);
    free(target);
    free(symlink->attributes.name);
    free(symlink);
}

Test(get_symlink_target, symlink_is_null) {
    const struct FileNode* result = get_symlink_target(NULL);

    cr_assert_null(result);
}

Test(get_symlink_target, target_is_null) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_READ);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_null(result);

    free(symlink->attributes.name);
    free(symlink);
}

Test(get_symlink_target, symlink_on_symlink) {
    struct FileNode* firstSymlink = create_file_node(NULL, "symlink1", FILE_TYPE_SYMLINK, PERM_READ | PERM_WRITE);
    struct FileNode* secondSymlink = create_file_node(NULL, "symlink2", FILE_TYPE_SYMLINK, PERM_READ | PERM_WRITE);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_NONE);
    set_symlink_target(firstSymlink, secondSymlink);
    set_symlink_target(secondSymlink, target);

    const struct FileNode* result = get_symlink_target(firstSymlink);

    cr_assert_eq(result, target);

    free(target->attributes.name);
    free(target);
    free(secondSymlink->attributes.name);
    free(secondSymlink);
    free(firstSymlink->attributes.name);
    free(firstSymlink);
}

Test(get_symlink_target, symlink_without_permissions) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_NONE);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_NONE);
    set_symlink_target(symlink, target);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_null(result);

    free(target->attributes.name);
    free(target);
    free(symlink->attributes.name);
    free(symlink);
}

Test(write_to_file, basic) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_READ | PERM_WRITE);
    const char content[] = "content";

    write_to_file(node, content);

    cr_assert_str_eq(node->attributes.fileContent, content);

    free(node->attributes.name);
    free(node);
}

Test(write_to_file, node_or_content_is_null) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_READ | PERM_WRITE);

    write_to_file(node, NULL);
    cr_assert_eq(node->attributes.fileContent, NULL);

    write_to_file(NULL, "content");

    free(node->attributes.name);
    free(node);
}

Test(write_to_file, symlink) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK, PERM_READ | PERM_WRITE);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_WRITE);
    set_symlink_target(symlink, target);
    const char content[] = "content";

    write_to_file(symlink, content);

    cr_assert_str_eq(target->attributes.fileContent, content);

    free(target->attributes.name);
    free(target);
    free(symlink->attributes.name);
    free(symlink);
}

Test(write_to_file, file_without_permissions) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_NONE);
    const char content[] = "content";

    write_to_file(node, content);

    cr_assert_null(node->attributes.fileContent);

    free(node->attributes.name);
    free(node);
}

Test(read_file_content, basic) {
    struct FileNode* file = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_READ | PERM_WRITE | PERM_EXEC);
    const char content[] = "content";
    write_to_file(file, content);

    cr_assert_str_eq(read_file_content(file), content);

    free(file->attributes.name);
    free(file);
}

Test(read_file_content, node_is_null) {
    cr_assert_null(read_file_content(NULL));
}

Test(read_file_content, file_without_permissions) {
    struct FileNode* file = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_NONE);
    const char content[] = "content";
    write_to_file(file, content);

    cr_assert_null(read_file_content(file));

    free(file->attributes.name);
    free(file);
}

Test(find_file_node_in_curr_dir, basic) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR, PERM_READ | PERM_WRITE | PERM_EXEC);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE, PERM_NONE);

    cr_assert_eq(find_file_node_in_curr_dir(dir, file->attributes.name), file);

    free(file->attributes.name);
    free(file);
    free(dir->attributes.name);
    free(dir);
}

Test(find_file_node_in_curr_dir, dir_or_name_is_null) {
    cr_assert_null(find_file_node_in_curr_dir(NULL, "test"));
    cr_assert_null(find_file_node_in_curr_dir((struct FileNode*)1, NULL));
}

Test(find_file_node_in_curr_dir, find_non_existing_file) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR, PERM_READ | PERM_EXEC);
    cr_assert_null(find_file_node_in_curr_dir(dir, "not exist"));

    free(dir->attributes.name);
    free(dir);
}

Test(find_file_node_in_curr_dir, dir_without_permissions) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR, PERM_NONE);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE, PERM_NONE);

    cr_assert_null(find_file_node_in_curr_dir(dir, file->attributes.name));

    free(file->attributes.name);
    free(file);
    free(dir->attributes.name);
    free(dir);
}

Test(find_file_node_in_fs, root_or_name_is_null) {
    cr_assert_null(find_file_node_in_fs(NULL, "test"));
    cr_assert_null(find_file_node_in_fs((struct FileNode*)1, NULL));
}

Test(get_file_node_path, basic) {
    struct FileNode* root = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE | PERM_EXEC);
    struct FileNode* file = create_file_node(root, "file", FILE_TYPE_FILE, PERM_NONE);

    char* path = get_file_node_path(file);
    cr_assert_str_eq(path, "\\file");

    free(path);
    free(file->attributes.name);
    free(file);
    free(root->attributes.name);
    free(root);
}

Test(get_file_node_path, dir_or_name_is_null) {
    cr_assert_null(get_file_node_path(NULL));
}

Test(change_file_node_location, move_valid_node) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE, PERM_WRITE);

    change_file_node_location(location, node);

    cr_assert_null(parent->attributes.directoryContent);
    cr_assert_eq(node->parent, location);
    cr_assert_eq(location->attributes.directoryContent, node);
}

Test(change_file_node_location, move_null_node) {
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);

    change_file_node_location(location, NULL);
    // Expect no crash
}

Test(change_file_node_location, move_node_from_empty_parent) {
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE, PERM_WRITE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, location);
}

Test(change_file_node_location, move_middle_node) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_READ | PERM_WRITE);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_WRITE);

    struct FileNode* node1 = create_file_node(parent, "file1", FILE_TYPE_FILE, PERM_WRITE);
    struct FileNode* node2 = create_file_node(parent, "file2", FILE_TYPE_FILE, PERM_WRITE);
    struct FileNode* node3 = create_file_node(parent, "file3", FILE_TYPE_FILE, PERM_WRITE);

    change_file_node_location(location, node2);

    cr_assert_eq(node1->next, node3);
    cr_assert_eq(node2->parent, location);
    cr_assert_eq(location->attributes.directoryContent, node2);
}

Test(change_file_node_location, new_location_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_NONE);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE, PERM_WRITE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, parent);
    cr_assert_null(location->attributes.directoryContent);
}

Test(change_file_node_location, node_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE, PERM_NONE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, parent);
    cr_assert_null(location->attributes.directoryContent);
}

Test(change_file_node_name, rename) {
    struct FileNode* file = create_file_node(NULL, "old", FILE_TYPE_FILE, PERM_WRITE);

    change_file_node_name(file, "new");

    cr_assert_str_eq(file->attributes.name, "new");

    free(file->attributes.name);
    free(file);
}

Test(delete_file_node, delete_existing) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR, PERM_WRITE);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE, PERM_NONE);

    delete_file_node(dir, file);

    cr_assert_null(dir->attributes.directoryContent);

    free(dir->attributes.name);
    free(dir);
}