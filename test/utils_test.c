/**
    * @file: utils_test.c
    * @author: without eyes
    *
    * This file contains tests for functions that
    * is not related to other files.
*/

#include "../include/utils.h"

#include <time.h>
#include "../include/file_node_structs.h"
#include "criterion/criterion.h"

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