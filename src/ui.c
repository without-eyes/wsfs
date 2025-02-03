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
#include "../include/file_structs.h"

void run_ui(struct FileNode* currentDir) {
    int isRunning = 1;
    while (isRunning) {
        puts("");
        print_dir_content(currentDir);

        printf("Input command('h' for help): ");
        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer
        switch (input) {
        case 'h':
            puts("Options:\n"
                 "(q)uit\n"
                 "create (f)ile\n"
                 "create (d)irectory\n"
                 "create (s)ymbolic link\n"
                 "(e)rase file node,\n"
                 "(w)rite text to file\n"
                 "(r)ead content from file,\n"
                 "(g)o into directory,\n"
                 "get file node (p)ath,\n"
                 "go (b)ack");
            break;

        case 'g': // go into directory
            printf("Enter directory name: ");
            char dirName[MAX_NAME_SIZE];
            fgets(dirName, MAX_NAME_SIZE, stdin);
            dirName[strcspn(dirName, "\n")] = 0;
            struct FileNode* fileNode = find_file_node_in_curr_dir(currentDir, dirName);
            if (fileNode->attributes.type == FILE_TYPE_SYMLINK) {
                currentDir = fileNode->attributes.symlinkTarget;
            } else if (fileNode->attributes.type == FILE_TYPE_DIR) {
                currentDir = fileNode;
            }
            break;

        case 'f': // create file
            printf("Enter file name: ");
            char newfileName[MAX_NAME_SIZE];
            fgets(newfileName, MAX_NAME_SIZE, stdin);
            newfileName[strcspn(newfileName, "\n")] = 0;
            struct FileNode* createdFile = create_file_node(currentDir, newfileName, FILE_TYPE_FILE);
            add_to_dir(currentDir, createdFile);
            break;

        case 'd': // create directory
            printf("Enter directory name: ");
            char directoryName[MAX_NAME_SIZE];
            fgets(directoryName, MAX_NAME_SIZE, stdin);
            directoryName[strcspn(directoryName, "\n")] = 0;
            struct FileNode* dir = create_file_node(currentDir, directoryName, FILE_TYPE_DIR);
            add_to_dir(currentDir, dir);
            break;

        case 's': // create symlink
            printf("Enter symlink name: ");
            char symlinkName[MAX_NAME_SIZE];
            fgets(symlinkName, MAX_NAME_SIZE, stdin);
            symlinkName[strcspn(symlinkName, "\n")] = 0;
            struct FileNode* symlink = create_file_node(currentDir, symlinkName, FILE_TYPE_SYMLINK);

            printf("Enter symlink target's name: ");
            char symlinkTargetName[MAX_NAME_SIZE];
            fgets(symlinkTargetName, MAX_NAME_SIZE, stdin);
            symlinkTargetName[strcspn(symlinkTargetName, "\n")] = 0;

            struct FileNode* rootDir = currentDir;
            while (strcmp(rootDir->attributes.name, "\\") != 0) {
                rootDir = rootDir->parent;
            }
            symlink->attributes.symlinkTarget = find_file_node_in_fs(rootDir, symlinkTargetName);

            add_to_dir(currentDir, symlink);
            break;

        case 'e': // erase file node
            printf("Enter file name: ");
            char wantedFileNameToDelete[MAX_NAME_SIZE];
            fgets(wantedFileNameToDelete, MAX_NAME_SIZE, stdin);
            wantedFileNameToDelete[strcspn(wantedFileNameToDelete, "\n")] = 0;
            delete_file_node(currentDir, wantedFileNameToDelete);
            break;

        case 'w': // write into file
            printf("Enter file name: ");
            char wantedFileNameToWrite[MAX_NAME_SIZE];
            fgets(wantedFileNameToWrite, MAX_NAME_SIZE, stdin);
            wantedFileNameToWrite[strcspn(wantedFileNameToWrite, "\n")] = 0;
            struct FileNode* wantedFileToWrite = find_file_node_in_curr_dir(currentDir, wantedFileNameToWrite);
            char* text = read_user_input();
            write_to_file(wantedFileToWrite, text);
            break;

        case 'r': // read from file
            printf("Enter file name: ");
            char wantedFileNameToRead[MAX_NAME_SIZE];
            fgets(wantedFileNameToRead, MAX_NAME_SIZE, stdin);
            wantedFileNameToRead[strcspn(wantedFileNameToRead, "\n")] = 0;
            struct FileNode* wantedFileToRead = find_file_node_in_curr_dir(currentDir, wantedFileNameToRead);
            print_file_content(wantedFileToRead);
            break;

        case 'p': // get file node path
            printf("Enter file name: ");
            char searchedFileNodeName[MAX_NAME_SIZE];
            fgets(searchedFileNodeName, MAX_NAME_SIZE, stdin);
            searchedFileNodeName[strcspn(searchedFileNodeName, "\n")] = 0;
            char* path = get_file_node_path(currentDir, searchedFileNodeName);
            printf("%s\n", path);
            free(path);
            break;

        case 'b' : // go back
            currentDir = currentDir->parent;
            break;

        case 'q': // quit
            isRunning = 0;
            break;

        default: // other
            printf("Invalid input.\n");
            break;
        }
    }
}

void print_file_info(const struct FileNode* fileNode) {
    if (fileNode->attributes.type == FILE_TYPE_SYMLINK) {
        printf("%c %2u:%02u %s -> %c %2u:%02u %s\n", get_file_type_letter(fileNode->attributes.type),
                                        fileNode->attributes.createdAt.hour,
                                        fileNode->attributes.createdAt.minute,
                                        fileNode->attributes.name,
                                        get_file_type_letter(fileNode->attributes.symlinkTarget->attributes.type),
                                        fileNode->attributes.symlinkTarget->attributes.createdAt.hour,
                                        fileNode->attributes.symlinkTarget->attributes.createdAt.minute,
                                        fileNode->attributes.symlinkTarget->attributes.name);
    } else {
        printf("%c %2u:%02u %s\n", get_file_type_letter(fileNode->attributes.type),
                                        fileNode->attributes.createdAt.hour,
                                        fileNode->attributes.createdAt.minute,
                                        fileNode->attributes.name);
    }
}

void print_dir_content(const struct FileNode* directory) {
    print_file_info(directory);
    const struct FileNode* currentFile = directory->attributes.directoryContent;
    while (currentFile != NULL) {
        print_file_info(currentFile);
        currentFile = currentFile->next;
    }
}