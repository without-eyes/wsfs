#include "../include/utils.h"

#include <stdio.h>
#include <time.h>

struct Timestamp get_current_time() {
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);

    struct Timestamp currentTime;
    currentTime.hour = timeinfo->tm_hour;
    currentTime.minute = timeinfo->tm_min;

    return currentTime;
}