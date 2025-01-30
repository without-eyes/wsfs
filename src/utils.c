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

struct Timestamp get_current_time(void) {
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);

    struct Timestamp currentTime;
    currentTime.hour = timeinfo->tm_hour;
    currentTime.minute = timeinfo->tm_min;

    return currentTime;
}

char* read_user_input(void) {
    size_t buffSize = 1024;
    size_t len = 0;
    char *buff = malloc(buffSize);

    puts("Enter text (Type 'EOF' on a new line to finish):");

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "EOF") == 0) break;

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