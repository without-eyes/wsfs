/**
* @file: wsfs.c
    * @author: without eyes
    *
    * This file contains tests for functions related
    * to Without eyeS's File System(WSFS).
*/

#include "../include/wsfs.h"

#include "../include/utils.h"
#include "criterion/criterion.h"

Test(wsfs_init, basic) {
    struct FileNode* result = wsfs_init();

    cr_assert_str_eq(result->attributes.name, "\\");
    cr_assert_eq(result->attributes.createdAt.hour, get_current_time().hour);
    cr_assert_eq(result->attributes.createdAt.minute, get_current_time().minute);
    cr_assert_eq(result->attributes.type, FILE_TYPE_DIR);
    cr_assert_eq(result->parent, result);
    cr_assert_null(result->attributes.directoryContent);
    cr_assert_null(result->attributes.fileContent);
    cr_assert_null(result->attributes.symlinkTarget);
    cr_assert_null(result->next);

    free(result->attributes.name);
    free(result);
}