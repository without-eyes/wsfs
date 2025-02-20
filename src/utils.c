/**
    * @file: utils.c
    * @author: without eyes
    *
    * This file contains implementation of functions that
    * is not related to other files.
*/

#include "../include/utils.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_node_structs.h"

struct Timestamp get_current_time(void) {
    time_t rawTime;
    time(&rawTime);
    const struct tm* timeInfo = localtime(&rawTime);

    struct Timestamp currentTime;
    currentTime.year = timeInfo->tm_year + 1900;
    currentTime.month = timeInfo->tm_mon + 1;
    currentTime.day = timeInfo->tm_mday;
    currentTime.hour = timeInfo->tm_hour;
    currentTime.minute = timeInfo->tm_min;

    return currentTime;
}

char* read_all_user_input(void) {
    size_t buffSize = BUFFER_SIZE;
    size_t len = 0;
    char *buff = malloc(buffSize);

    puts("Enter text (Type 'EOF' on a new line to finish):");

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, END_OF_FILE_LINE) == 0) break;

        if (len + strlen(line) + 2 > buffSize) {
            buffSize *= 2;
            char *newBuff = realloc(buff, buffSize);
            buff = newBuff;
        }

        strcpy(buff + len, line);
        len += strlen(line);
        buff[len++] = '\n';
    }

    buff[len] = '\0';
    return buff;
}

void read_line(char* buffer, const size_t size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}