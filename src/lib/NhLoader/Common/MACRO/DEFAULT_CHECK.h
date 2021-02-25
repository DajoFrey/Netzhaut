/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_LOADER_CHECK
#define NH_LOADER_CHECK(checkable)                                                \
{                                                                                 \
    NH_LOADER_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_LOADER_SUCCESS) {NH_LOADER_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_LOADER_SILENT_CHECK
#define NH_LOADER_SILENT_CHECK(checkable)                          \
{                                                                  \
    NH_LOADER_RESULT checkResult = checkable;                      \
    if (checkResult != NH_LOADER_SUCCESS) {NH_LOADER_SILENT_END()} \
}

#undef  NH_LOADER_CHECK_NULL
#define NH_LOADER_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_NULL_POINTER)} \
}

#undef  NH_LOADER_CHECK_MEM
#define NH_LOADER_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_LOADER_DIAGNOSTIC_END(NH_LOADER_ERROR_MEMORY_ALLOCATION)} \
}

