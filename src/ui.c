/**
* @file: ui.c
    * @author: without eyes
    *
    * This file contains implementation of functions related
    * to user interface.
*/

#include "../include/ui.h"

#include <stdio.h>
#include <string.h>

void run_ui(struct FileNode* currentDir) {
    int isRunning = 1;
    while (isRunning) {
        puts("");
        print_dir_content(currentDir);

        printf("Options: (q)uit, create (f)ile or (d)irectory: ");
        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer
        switch (input) {
        case 'q': // quit
            isRunning = 0;
            break;

        case 'g': // go into directory
            printf("Enter directory name: ");
            char dirName[MAX_NAME_SIZE];
            fgets(dirName, MAX_NAME_SIZE, stdin);
            dirName[strcspn(dirName, "\n")] = 0;
            change_current_dir(&currentDir, dirName);
            break;

        case 'b' : // go back
            currentDir = currentDir->parent;
            break;

        case 'f': // create file
            struct FileNode* file = create_file(currentDir);
            add_to_dir(currentDir, file);
            break;

        case 'd': // create directory
            struct FileNode* dir = create_dir(currentDir);
            add_to_dir(currentDir, dir);
            break;

        default: // other
            printf("Invalid input.\n");
            break;
        }
    }
}

void print_file_info(const struct FileNode* file) {
    printf("%c %2u:%02u %s\n", get_file_type_letter(file->attributes.type), file->attributes.createdAt.hour, file->attributes.createdAt.minute, file->attributes.name);
}

void print_dir_content(const struct FileNode* directory) {
    print_file_info(directory);
    const struct FileNode* currentFile = directory->attributes.directoryContent;
    while (currentFile != NULL) {
        print_file_info(currentFile);
        currentFile = currentFile->next;
    }
}