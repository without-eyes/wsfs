/**
* @file: wsfs.c
    * @author: without eyes
    *
    * This file contains tests for functions related
    * to Without eyeS's File System(WSFS).
*/

#include "../include/wsfs.h"

#include "criterion/criterion.h"

Test(wsfs_init, basic) {
    struct FileNode* result = wsfs_init();

    cr_assert_str_eq(result->info.metadata.name, "\\");
    cr_assert_eq(result->info.metadata.creationTime.hour, get_current_time().hour);
    cr_assert_eq(result->info.metadata.creationTime.minute, get_current_time().minute);
    cr_assert_eq(result->info.properties.type, FILE_TYPE_DIR);
    cr_assert_eq(result->parent, result);
    cr_assert_null(result->info.data.directoryContent);
    cr_assert_null(result->info.data.fileContent);
    cr_assert_null(result->info.data.symlinkTarget);
    cr_assert_null(result->next);

    free(result->info.metadata.name);
    free(result);
}