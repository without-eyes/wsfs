/**
    * @file: file_node_funcs_test.c
    * @author: without eyes
    *
    * This file contains tests for functions related
    * to files and file nodes.
*/

#include "../include/file_node_funcs.h"

#include <stdio.h>
#include <time.h>

#include "../include/wsfs_macros.h"
#include "criterion/criterion.h"

Test(create_file_node, basic_creation) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    char name[5] = "node";
    enum FileType type = FILE_TYPE_FILE;

    struct FileNode* child = create_file_node(parent, name, type);

    struct Timestamp timestamp = get_current_time();
    cr_assert_str_eq(child->info.metadata.name, name);
    cr_assert_eq(child->info.metadata.creationTime.hour, timestamp.hour);
    cr_assert_eq(child->info.metadata.creationTime.minute, timestamp.minute);
    cr_assert_eq(child->info.properties.type, type);
    cr_assert_eq(child->info.properties.permissions, PERM_DEFAULT - PERMISSION_MASK);
    cr_assert_eq(child->parent, parent);
    cr_assert_null(child->info.data.directoryContent);
    cr_assert_null(child->info.data.fileContent);
    cr_assert_null(child->info.data.symlinkTarget);
    cr_assert_null(child->next);

    free(child->info.metadata.name);
    free(child);
    free(parent->info.metadata.name);
    free(parent);
}

Test(create_file_node, root_creation) {
    char name[2] = "\\";
    enum FileType type = FILE_TYPE_DIR;

    struct FileNode* root = create_file_node(NULL, name, type);

    struct Timestamp timestamp = get_current_time();
    cr_assert_str_eq(root->info.metadata.name, name);
    cr_assert_eq(root->info.metadata.creationTime.hour, timestamp.hour);
    cr_assert_eq(root->info.metadata.creationTime.minute, timestamp.minute);
    cr_assert_eq(root->info.properties.type, type);
    cr_assert_eq(root->info.properties.permissions, PERM_DEFAULT - PERMISSION_MASK);
    cr_assert_eq(root->parent, root);
    cr_assert_null(root->info.data.directoryContent);
    cr_assert_null(root->info.data.fileContent);
    cr_assert_null(root->info.data.symlinkTarget);
    cr_assert_null(root->next);

    free(root->info.metadata.name);
    free(root);
}

Test(change_permissions, change_valid_node_permissions) {
    struct FileNode node;
    node.info.properties.permissions = PERM_NONE;

    change_permissions(&node, PERM_READ);

    cr_assert_eq(node.info.properties.permissions, PERM_READ);
}

Test(change_permissions, null_node_no_change) {
    change_permissions(NULL, PERM_NONE);

    // No assertion needed; just ensuring it does not crash
}

Test(change_permissions, change_multiple_perms) {
    struct FileNode node;
    node.info.properties.permissions = PERM_NONE;

    change_permissions(&node, PERM_READ | PERM_WRITE);

    cr_assert_eq(node.info.properties.permissions, PERM_READ | PERM_WRITE);
}

Test(is_permissions_equal, all) {
    cr_assert_eq(is_permissions_equal(PERM_NONE, PERM_NONE), 1);
    cr_assert_eq(is_permissions_equal(PERM_READ | PERM_WRITE, PERM_WRITE | PERM_READ), 1);
    cr_assert_eq(is_permissions_equal(PERM_READ, PERM_WRITE), 0);
}

Test(root_node, set_and_get) {
    struct FileNode* testNode = create_file_node(NULL, "\\", FILE_TYPE_DIR);

    set_root_node(testNode);
    const struct FileNode* retrievedNode = get_root_node();

    cr_assert_eq(retrievedNode, testNode);
    cr_assert_str_eq(retrievedNode->info.metadata.name, "\\");
}

Test(root_node, set_null_does_not_change_root) {
    struct FileNode* testNode = create_file_node(NULL, "\\", FILE_TYPE_DIR);

    set_root_node(testNode);
    set_root_node(NULL); // This should not change root

    const struct FileNode* retrieved_node = get_root_node();

    cr_assert_eq(retrieved_node, testNode);
}

Test(get_file_node_size, null_node) {
    cr_assert_eq(get_file_node_size(NULL), 0);
}

Test(get_file_node_size, empty_file_node) {
    struct FileNode* node = create_file_node(NULL, "", FILE_TYPE_FILE);
    const size_t expected_size = sizeof(struct FileNode) + strlen(node->info.metadata.name) + 1;
    cr_assert_eq(get_file_node_size(node), expected_size);
    free(node);
}

Test(get_file_node_size, file_with_content) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE);
    write_to_file(node, "Hello");
    const size_t expected_size = sizeof(struct FileNode) + strlen("file") + 1 + strlen("Hello") + 1;

    cr_assert_eq(get_file_node_size(node), expected_size);

    free(node->info.metadata.name);
    free(node->info.data.fileContent);
    free(node);
}

Test(get_file_node_size, file_without_permissions) {
    struct FileNode* node = create_file_node(NULL, "", FILE_TYPE_FILE);
    change_permissions(node, PERM_NONE);
    cr_assert_eq(get_file_node_size(node), 0);
    free(node);
}

Test(get_file_node_size, directory_with_files) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR);
    struct FileNode* file1 = create_file_node(dir, "file1", FILE_TYPE_FILE);
    write_to_file(file1, "Hello");
    struct FileNode* file2 = create_file_node(dir, "file2", FILE_TYPE_FILE);
    write_to_file(file2, "World");

    size_t expected_size = sizeof(struct FileNode) + strlen("dir") + 1;
    expected_size += sizeof(struct FileNode) + strlen("file1") + 1 + strlen("Hello") + 1;
    expected_size += sizeof(struct FileNode) + strlen("file2") + 1 + strlen("World") + 1;

    cr_assert_eq(get_file_node_size(dir), expected_size);

    free(file1->info.metadata.name);
    free(file1->info.data.fileContent);
    free(file1);
    free(file2->info.metadata.name);
    free(file2->info.data.fileContent);
    free(file2);
    free(dir->info.metadata.name);
    free(dir);
}

Test(change_current_dir, dir_exists) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR);
    change_permissions(newCurrentDir, PERM_DEFAULT);

    change_current_dir(&currentDir, newCurrentDir);

    cr_assert_eq(currentDir, newCurrentDir);
    cr_assert_str_eq(currentDir->info.metadata.name, newCurrentDir->info.metadata.name);
    cr_assert_eq(currentDir->info.metadata.creationTime.hour, newCurrentDir->info.metadata.creationTime.hour);
    cr_assert_eq(currentDir->info.metadata.creationTime.minute, newCurrentDir->info.metadata.creationTime.minute);
    cr_assert_eq(currentDir->info.properties.type, newCurrentDir->info.properties.type);
    cr_assert_eq(currentDir->info.data.directoryContent, newCurrentDir->info.data.directoryContent);
    cr_assert_eq(currentDir->info.data.fileContent, newCurrentDir->info.data.fileContent);
    cr_assert_eq(currentDir->info.data.symlinkTarget, newCurrentDir->info.data.symlinkTarget);
    cr_assert_eq(currentDir->next, newCurrentDir->next);
    cr_assert_eq(currentDir->parent, newCurrentDir->parent);

    free(currentDir->parent->info.metadata.name);
    free(currentDir->parent);
    free(currentDir->info.metadata.name);
    free(currentDir);
}

Test(change_current_dir, dir_not_exists) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR);

    change_current_dir(&currentDir, NULL);

    cr_assert_str_eq(currentDir->info.metadata.name, currentDir->info.metadata.name);
    cr_assert_eq(currentDir->info.metadata.creationTime.hour, currentDir->info.metadata.creationTime.hour);
    cr_assert_eq(currentDir->info.metadata.creationTime.minute, currentDir->info.metadata.creationTime.minute);
    cr_assert_eq(currentDir->info.properties.type, currentDir->info.properties.type);
    cr_assert_eq(currentDir->info.data.directoryContent, currentDir->info.data.directoryContent);
    cr_assert_eq(currentDir->info.data.fileContent, currentDir->info.data.fileContent);
    cr_assert_eq(currentDir->info.data.symlinkTarget, currentDir->info.data.symlinkTarget);
    cr_assert_eq(currentDir->next, currentDir->next);
    cr_assert_eq(currentDir->parent, currentDir->parent);

    free(currentDir->info.metadata.name);
    free(currentDir);
}

Test(change_current_dir, symlink) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* symlink = create_file_node(currentDir, "symlink", FILE_TYPE_SYMLINK);
    change_permissions(symlink, PERM_DEFAULT);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR);
    change_permissions(newCurrentDir, PERM_DEFAULT);
    set_symlink_target(symlink, newCurrentDir);

    change_current_dir(&currentDir, symlink);

    cr_assert_str_eq(currentDir->info.metadata.name, newCurrentDir->info.metadata.name);
    cr_assert_eq(currentDir->info.metadata.creationTime.hour, newCurrentDir->info.metadata.creationTime.hour);
    cr_assert_eq(currentDir->info.metadata.creationTime.minute, newCurrentDir->info.metadata.creationTime.minute);
    cr_assert_eq(currentDir->info.properties.type, newCurrentDir->info.properties.type);
    cr_assert_eq(currentDir->info.data.directoryContent, newCurrentDir->info.data.directoryContent);
    cr_assert_eq(currentDir->info.data.fileContent, newCurrentDir->info.data.fileContent);
    cr_assert_eq(currentDir->info.data.symlinkTarget, newCurrentDir->info.data.symlinkTarget);
    cr_assert_eq(currentDir->next, newCurrentDir->next);
    cr_assert_eq(currentDir->parent, newCurrentDir->parent);

    free(currentDir->parent->info.metadata.name);
    free(currentDir->parent);
    free(currentDir->info.metadata.name);
    free(currentDir);
}

Test(change_current_dir, dir_without_permissions) {
    struct FileNode* currentDir = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* newCurrentDir = create_file_node(currentDir, "dir", FILE_TYPE_DIR);
    change_permissions(newCurrentDir, PERM_NONE);
    const struct FileNode* temp = currentDir;

    change_current_dir(&currentDir, newCurrentDir);

    cr_assert_neq(currentDir, newCurrentDir);

    free(temp->parent->info.metadata.name);
    free(temp->parent);
    free(newCurrentDir->info.metadata.name);
    free(newCurrentDir);
}

Test(add_to_dir, add_first_child_to_empty_directory) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR);
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE);

    add_to_dir(parent, child);

    cr_assert_eq(parent->info.data.directoryContent, child);
    cr_assert_null(child->next);

    free(parent->info.metadata.name);
    free(parent);
    free(child->info.metadata.name);
    free(child);
}

Test(add_to_dir, add_multiple_children) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR);
    struct FileNode* child1 = create_file_node(NULL, "child1", FILE_TYPE_FILE);
    struct FileNode* child2 = create_file_node(NULL, "child2", FILE_TYPE_FILE);

    add_to_dir(parent, child1);
    add_to_dir(parent, child2);

    cr_assert_eq(parent->info.data.directoryContent, child1);
    cr_assert_eq(child1->next, child2);
    cr_assert_null(child2->next);

    free(parent->info.metadata.name);
    free(parent);
    free(child1->info.metadata.name);
    free(child1);
    free(child2->info.metadata.name);
    free(child2);
}

Test(add_to_dir, add_null_child_does_nothing) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR);

    add_to_dir(parent, NULL);

    cr_assert_null(parent->info.data.directoryContent);

    free(parent->info.metadata.name);
    free(parent);
}

Test(add_to_dir, add_child_to_null_parent_does_nothing) {
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE);

    add_to_dir(NULL, child);

    cr_assert_null(child->next);

    free(child->info.metadata.name);
    free(child);
}

Test(add_to_dir, add_to_directory_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "parent", FILE_TYPE_DIR);
    change_permissions(parent, PERM_NONE);
    struct FileNode* child = create_file_node(NULL, "child", FILE_TYPE_FILE);

    add_to_dir(parent, child);

    cr_assert_null(parent->info.data.directoryContent);

    free(parent->info.metadata.name);
    free(parent);
    free(child->info.metadata.name);
    free(child);
}

Test(get_file_type_letter, all_types) {
    cr_assert_eq(get_file_type_letter(FILE_TYPE_DIR),       'd');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_FILE),      'f');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_SYMLINK),   's');
    cr_assert_eq(get_file_type_letter(FILE_TYPE_UNKNOWN),   '-');
    cr_assert_eq(get_file_type_letter(-1),                  '?');
}

Test(get_permission_letter, all_permissions) {
    cr_assert_eq(get_permission_letter(PERM_NONE),   '-');
    cr_assert_eq(get_permission_letter(PERM_READ),   'r');
    cr_assert_eq(get_permission_letter(PERM_WRITE),  'w');
    cr_assert_eq(get_permission_letter(PERM_EXEC),   'x');
    cr_assert_eq(get_permission_letter(-1),          '?');
}

Test(set_symlink_target, valid_symlink_target) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);
    struct FileNode* target = create_file_node(NULL, "target_file", FILE_TYPE_FILE);

    set_symlink_target(symlink, target);

    cr_assert_eq(symlink->info.data.symlinkTarget, target);
}

Test(set_symlink_target, set_symlink_target_to_null) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);

    set_symlink_target(symlink, NULL);

    cr_assert_null(symlink->info.data.symlinkTarget);
}

Test(set_symlink_target, null_symlink_does_nothing) {
    struct FileNode* target = create_file_node(NULL, "target_file", FILE_TYPE_FILE);

    set_symlink_target(NULL, target);

    // No assertion needed; just ensuring it does not crash
}

Test(get_symlink_target, basic) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE);
    set_symlink_target(symlink, target);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_eq(result, target);

    free(target->info.metadata.name);
    free(target);
    free(symlink->info.metadata.name);
    free(symlink);
}

Test(get_symlink_target, symlink_is_null) {
    const struct FileNode* result = get_symlink_target(NULL);

    cr_assert_null(result);
}

Test(get_symlink_target, target_is_null) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_null(result);

    free(symlink->info.metadata.name);
    free(symlink);
}

Test(get_symlink_target, symlink_on_symlink) {
    struct FileNode* firstSymlink = create_file_node(NULL, "symlink1", FILE_TYPE_SYMLINK);
    struct FileNode* secondSymlink = create_file_node(NULL, "symlink2", FILE_TYPE_SYMLINK);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE);
    set_symlink_target(firstSymlink, secondSymlink);
    set_symlink_target(secondSymlink, target);

    const struct FileNode* result = get_symlink_target(firstSymlink);

    cr_assert_eq(result, target);

    free(target->info.metadata.name);
    free(target);
    free(secondSymlink->info.metadata.name);
    free(secondSymlink);
    free(firstSymlink->info.metadata.name);
    free(firstSymlink);
}

Test(get_symlink_target, symlink_without_permissions) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);
    change_permissions(symlink, PERM_NONE);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE);
    set_symlink_target(symlink, target);

    const struct FileNode* result = get_symlink_target(symlink);

    cr_assert_null(result);

    free(target->info.metadata.name);
    free(target);
    free(symlink->info.metadata.name);
    free(symlink);
}

Test(write_to_file, basic) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE);
    const char content[] = "content";

    write_to_file(node, content);

    cr_assert_str_eq(node->info.data.fileContent, content);

    free(node->info.metadata.name);
    free(node);
}

Test(write_to_file, node_or_content_is_null) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE);

    write_to_file(node, NULL);
    cr_assert_eq(node->info.data.fileContent, NULL);

    write_to_file(NULL, "content");

    free(node->info.metadata.name);
    free(node);
}

Test(write_to_file, symlink) {
    struct FileNode* symlink = create_file_node(NULL, "symlink", FILE_TYPE_SYMLINK);
    struct FileNode* target = create_file_node(NULL, "file", FILE_TYPE_FILE);
    set_symlink_target(symlink, target);
    const char content[] = "content";

    write_to_file(symlink, content);

    cr_assert_str_eq(target->info.data.fileContent, content);

    free(target->info.metadata.name);
    free(target);
    free(symlink->info.metadata.name);
    free(symlink);
}

Test(write_to_file, file_without_permissions) {
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE);
    change_permissions(node, PERM_NONE);
    const char content[] = "content";

    write_to_file(node, content);

    cr_assert_null(node->info.data.fileContent);

    free(node->info.metadata.name);
    free(node);
}

Test(read_file_content, basic) {
    struct FileNode* file = create_file_node(NULL, "file", FILE_TYPE_FILE);
    const char content[] = "content";
    write_to_file(file, content);

    cr_assert_str_eq(read_file_content(file), content);

    free(file->info.metadata.name);
    free(file);
}

Test(read_file_content, node_is_null) {
    cr_assert_null(read_file_content(NULL));
}

Test(read_file_content, file_without_permissions) {
    struct FileNode* file = create_file_node(NULL, "file", FILE_TYPE_FILE);
    change_permissions(file, PERM_NONE);
    const char content[] = "content";
    write_to_file(file, content);

    cr_assert_null(read_file_content(file));

    free(file->info.metadata.name);
    free(file);
}

Test(find_file_node_in_curr_dir, basic) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR);
    change_permissions(dir, PERM_DEFAULT);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE);

    cr_assert_eq(find_file_node_in_curr_dir(dir, file->info.metadata.name), file);

    free(file->info.metadata.name);
    free(file);
    free(dir->info.metadata.name);
    free(dir);
}

Test(find_file_node_in_curr_dir, dir_or_name_is_null) {
    cr_assert_null(find_file_node_in_curr_dir(NULL, "test"));
    cr_assert_null(find_file_node_in_curr_dir((struct FileNode*)1, NULL));
}

Test(find_file_node_in_curr_dir, find_non_existing_file) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR);
    cr_assert_null(find_file_node_in_curr_dir(dir, "not exist"));

    free(dir->info.metadata.name);
    free(dir);
}

Test(find_file_node_in_curr_dir, dir_without_permissions) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR);
    change_permissions(dir, PERM_NONE);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE);

    cr_assert_null(find_file_node_in_curr_dir(dir, file->info.metadata.name));

    free(file->info.metadata.name);
    free(file);
    free(dir->info.metadata.name);
    free(dir);
}

Test(find_file_node_in_fs, root_or_name_is_null) {
    cr_assert_null(find_file_node_in_fs(NULL, "test"));
    cr_assert_null(find_file_node_in_fs((struct FileNode*)1, NULL));
}

Test(get_file_node_path, basic) {
    struct FileNode* root = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* file = create_file_node(root, "file", FILE_TYPE_FILE);

    char* path = get_file_node_path(file);
    cr_assert_str_eq(path, "\\file");

    free(path);
    free(file->info.metadata.name);
    free(file);
    free(root->info.metadata.name);
    free(root);
}

Test(get_file_node_path, dir_or_name_is_null) {
    cr_assert_null(get_file_node_path(NULL));
}

Test(change_file_node_location, move_valid_node) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE);

    change_file_node_location(location, node);

    cr_assert_null(parent->info.data.directoryContent);
    cr_assert_eq(node->parent, location);
    cr_assert_eq(location->info.data.directoryContent, node);
}

Test(change_file_node_location, move_null_node) {
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);

    change_file_node_location(location, NULL);
    // Expect no crash
}

Test(change_file_node_location, move_node_from_empty_parent) {
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);
    struct FileNode* node = create_file_node(NULL, "file", FILE_TYPE_FILE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, location);
}

Test(change_file_node_location, move_middle_node) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);

    struct FileNode* node1 = create_file_node(parent, "file1", FILE_TYPE_FILE);
    struct FileNode* node2 = create_file_node(parent, "file2", FILE_TYPE_FILE);
    struct FileNode* node3 = create_file_node(parent, "file3", FILE_TYPE_FILE);

    change_file_node_location(location, node2);

    cr_assert_eq(node1->next, node3);
    cr_assert_eq(node2->parent, location);
    cr_assert_eq(location->info.data.directoryContent, node2);
}

Test(change_file_node_location, new_location_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);
    change_permissions(location, PERM_NONE);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, parent);
    cr_assert_null(location->info.data.directoryContent);
}

Test(change_file_node_location, node_without_permissions) {
    struct FileNode* parent = create_file_node(NULL, "\\", FILE_TYPE_DIR);
    struct FileNode* location = create_file_node(NULL, "new location", FILE_TYPE_DIR);
    struct FileNode* node = create_file_node(parent, "file", FILE_TYPE_FILE);
    change_permissions(node, PERM_NONE);

    change_file_node_location(location, node);

    cr_assert_eq(node->parent, parent);
    cr_assert_null(location->info.data.directoryContent);
}

Test(copy_file_node, copy_single_file) {
    struct FileNode* root = create_file_node(NULL, "root", FILE_TYPE_DIR);
    struct FileNode* file = create_file_node(root, "file", FILE_TYPE_FILE);
    write_to_file(file, "Hello");

    copy_file_node(root, file);

    cr_assert_not_null(root->info.data.directoryContent);
    cr_assert_not_null(root->info.data.directoryContent->next);
    cr_assert_str_eq(root->info.data.directoryContent->next->info.metadata.name, "file");
    cr_assert_str_eq(root->info.data.directoryContent->next->info.data.fileContent, "Hello");
}

Test(copy_file_node, copy_directory_structure) {
    struct FileNode* root = create_file_node(NULL, "root", FILE_TYPE_DIR);
    struct FileNode* subdir = create_file_node(root, "subdir", FILE_TYPE_DIR);
    struct FileNode* file = create_file_node(subdir, "file", FILE_TYPE_FILE);
    write_to_file(file, "World");

    copy_file_node(root, subdir);

    cr_assert_not_null(root->info.data.directoryContent->next);
    cr_assert_str_eq(root->info.data.directoryContent->next->info.metadata.name, "subdir");
    cr_assert_not_null(root->info.data.directoryContent->next->info.data.directoryContent);
    cr_assert_str_eq(root->info.data.directoryContent->next->info.data.directoryContent->info.metadata.name, "file");
    cr_assert_str_eq(root->info.data.directoryContent->next->info.data.directoryContent->info.data.fileContent, "World");
}

Test(copy_file_node, null_inputs) {
    struct FileNode* root = create_file_node(NULL, "root", FILE_TYPE_DIR);

    copy_file_node(NULL, root);
    copy_file_node(root, NULL);

    cr_assert_null(root->info.data.directoryContent);
}

Test(copy_file_node, non_writable_directory) {
    struct FileNode* root = create_file_node(NULL, "root", FILE_TYPE_DIR);
    change_permissions(root, PERM_NONE);
    struct FileNode* file = create_file_node(root, "file", FILE_TYPE_FILE);

    copy_file_node(root, file);

    cr_assert_null(root->info.data.directoryContent);
}

Test(change_file_node_name, rename) {
    struct FileNode* file = create_file_node(NULL, "old", FILE_TYPE_FILE);

    change_file_node_name(file, "new");

    cr_assert_str_eq(file->info.metadata.name, "new");

    free(file->info.metadata.name);
    free(file);
}

Test(delete_file_node, delete_existing) {
    struct FileNode* dir = create_file_node(NULL, "dir", FILE_TYPE_DIR);
    struct FileNode* file = create_file_node(dir, "file", FILE_TYPE_FILE);

    delete_file_node(dir, file);

    cr_assert_null(dir->info.data.directoryContent);

    free(dir->info.metadata.name);
    free(dir);
}

Test(get_current_time, basic) {
    time_t rawTime;
    time(&rawTime);
    const struct tm* timeInfo = localtime(&rawTime);

    const struct Timestamp currentTime1 = {
        .year=timeInfo->tm_year + 1900,
        .month=timeInfo->tm_mon + 1,
        .day=timeInfo->tm_mday,
        .hour=timeInfo->tm_hour,
        .minute=timeInfo->tm_min,
    };
    const struct Timestamp currentTime2 = get_current_time();

    cr_assert_eq(currentTime1.year, currentTime2.year);
    cr_assert_eq(currentTime1.month, currentTime2.month);
    cr_assert_eq(currentTime1.day, currentTime2.day);
    cr_assert_eq(currentTime1.hour, currentTime2.hour);
    cr_assert_eq(currentTime1.minute, currentTime2.minute);
}

Test(is_enough_memory, memory_under_limit) {
    struct FileNode* allocatedMemory = malloc(MAX_MEMORY_SIZE / 2);
    set_root_node(allocatedMemory);
    cr_assert_eq(is_enough_memory(MAX_MEMORY_SIZE / 4), 1);
    free(allocatedMemory);
}

Test(is_enough_memory, memory_over_limit) {
    struct FileNode* allocatedMemory = malloc(MAX_MEMORY_SIZE / 2);
    set_root_node(allocatedMemory);
    cr_assert_eq(is_enough_memory(MAX_MEMORY_SIZE), 0);
    free(allocatedMemory);
}