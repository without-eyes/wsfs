/**
* @file: ui.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to user interface.
*/

#include "../include/ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/file_node_funcs.h"

void run_ui(struct FileNode* currentDir) {
    while (1) {
        puts("");
        print_dir_content(currentDir);
        printf("Input command('h' for help): ");

        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer

        switch (input) {
        case 'h': // show help
            print_help();
            break;

        case 'g': // go into directory
            printf("Enter directory name: ");
            char dir[MAX_NAME_SIZE];
            read_line(dir, MAX_NAME_SIZE);
            change_current_dir(&currentDir, dir);
            break;

        case 'f': // create file
            handle_create(currentDir, FILE_TYPE_FILE);
            break;

        case 'd': // create directory
            handle_create(currentDir, FILE_TYPE_DIR);
            break;

        case 's': // create symlink
            handle_create(currentDir, FILE_TYPE_SYMLINK);
            break;

        case 'c': // change name
            printf("Enter file node name: ");
            char oldName[MAX_NAME_SIZE];
            read_line(oldName, MAX_NAME_SIZE);
            struct FileNode* current = find_file_node_in_curr_dir(currentDir, oldName);
            printf("Enter new name: ");
            char newName[MAX_NAME_SIZE];
            read_line(newName, MAX_NAME_SIZE);
            change_file_node_name(current, newName);
            break;

        case 'e': // erase file node
            printf("Enter file node name: ");
            char delName[MAX_NAME_SIZE];
            read_line(delName, MAX_NAME_SIZE);
            delete_file_node(currentDir, delName);
            break;

        case 'w': // write into file
        case 'r': // read from file
            handle_read_write(currentDir, input);
            break;

        case 'p': // get file node path
            printf("Enter file node name: ");
            char search[MAX_NAME_SIZE];
            read_line(search, MAX_NAME_SIZE);
            const struct FileNode* node = find_file_node_in_curr_dir(currentDir, search);
            char* path = get_file_node_path(node);
            printf("%s\n", path);
            free(path);
            break;

        case 'b' : // go back
            currentDir = currentDir->parent;
            break;

        case 'q': // quit
            return;

        default: // other
            printf("Invalid input.\n");
            break;
        }
    }
}

void print_file_info(const struct FileNode* node) {
    if (node == NULL) return;

    if (node->attributes.type == FILE_TYPE_SYMLINK) {
        printf("%c %2u:%02u %s -> %c %2u:%02u %s\n", get_file_type_letter(node->attributes.type),
                                        node->attributes.createdAt.hour,
                                        node->attributes.createdAt.minute,
                                        node->attributes.name,
                                        get_file_type_letter(node->attributes.symlinkTarget->attributes.type),
                                        node->attributes.symlinkTarget->attributes.createdAt.hour,
                                        node->attributes.symlinkTarget->attributes.createdAt.minute,
                                        node->attributes.symlinkTarget->attributes.name);
    } else {
        printf("%c %2u:%02u %s\n", get_file_type_letter(node->attributes.type),
                                        node->attributes.createdAt.hour,
                                        node->attributes.createdAt.minute,
                                        node->attributes.name);
    }
}

void print_dir_content(const struct FileNode* directory) {
    if (directory == NULL) return;

    print_file_info(directory);
    const struct FileNode* current = directory->attributes.directoryContent;
    while (current != NULL) {
        print_file_info(current);
        current = current->next;
    }
}

void print_help() {
    puts("Options:\n"
                 "(q)uit\n"
                 "create (f)ile\n"
                 "create (d)irectory\n"
                 "create (s)ymbolic link\n"
                 "(c)hange name"
                 "(e)rase file node,\n"
                 "(w)rite text to file\n"
                 "(r)ead content from file,\n"
                 "(g)o into directory,\n"
                 "get file node (p)ath,\n"
                 "go (b)ack");
}

void handle_create(struct FileNode* currentDir, const enum FileType type) {
    char name[MAX_NAME_SIZE];
    char* fileType;
    switch (type) {
        case FILE_TYPE_SYMLINK:     fileType = "symlink";   break;
        case FILE_TYPE_DIR:         fileType = "directory"; break;
        case FILE_TYPE_FILE:        fileType = "file";      break;
        default:                    fileType = "?";         break;
    }
    printf("Enter %s name: ", fileType);
    read_line(name, MAX_NAME_SIZE);

    struct FileNode* node = create_file_node(currentDir, name, type);
    add_to_dir(currentDir, node);

    if (type == FILE_TYPE_SYMLINK) {
        printf("Enter symlink target's name: ");
        char target[MAX_NAME_SIZE];
        read_line(target, MAX_NAME_SIZE);

        const struct FileNode* root = currentDir;
        while (strcmp(root->attributes.name, "\\") != 0) root = root->parent;

        node->attributes.symlinkTarget = find_file_node_in_fs(root, target);
    }
}

void handle_read_write(const struct FileNode* currentDir, const char mode) {
    char name[MAX_NAME_SIZE];
    printf("Enter file name: ");
    read_line(name, MAX_NAME_SIZE);

    struct FileNode* file = find_file_node_in_curr_dir(currentDir, name);
    if (!file) return;

    char* content;
    if (mode == 'w') {
        content = read_all_user_input();
        write_to_file(file, content);
        free(content);
    } else {
        content = read_file_content(file);
        printf("%s", content);
    }
}