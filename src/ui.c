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
#include "../include/utils.h"
#include "../include/file_structs.h"

void run_ui(struct FileNode* currentDir) {
    int isRunning = 1;
    while (isRunning) {
        puts("");
        print_dir_content(currentDir);

        printf("Options: (q)uit, create (f)ile or (d)irectory, (w)rite text to file, (g)o into directory, go (b)ack: ");
        const char input = getchar();
        while (getchar() != '\n'); // Consume newline character left in buffer
        switch (input) {
        case 'g': // go into directory
            printf("Enter directory name: ");
            char dirName[MAX_NAME_SIZE];
            fgets(dirName, MAX_NAME_SIZE, stdin);
            dirName[strcspn(dirName, "\n")] = 0;
            currentDir = find_file_node(currentDir, dirName);
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

        case 'w': // write into file
            printf("Enter directory name: ");
            char wantedFileName[MAX_NAME_SIZE];
            fgets(wantedFileName, MAX_NAME_SIZE, stdin);
            wantedFileName[strcspn(wantedFileName, "\n")] = 0;
            struct FileNode* wantedFile = find_file_node(currentDir, wantedFileName);
            char* text = read_user_input();
            write_to_file(wantedFile, text);
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