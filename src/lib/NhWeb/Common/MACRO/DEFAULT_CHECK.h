/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WEB_CHECK
#define NH_WEB_CHECK(checkable)                                                \
{                                                                                 \
    NH_WEB_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_WEB_SUCCESS) {NH_WEB_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_WEB_SILENT_CHECK
#define NH_WEB_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_WEB_RESULT checkResult = checkable;                      \
    if (checkResult != NH_WEB_SUCCESS) {NH_WEB_SILENT_END()} \
}

#undef  NH_WEB_CHECK_NULL
#define NH_WEB_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_NULL_POINTER)} \
}

#undef  NH_WEB_CHECK_MEM
#define NH_WEB_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_MEMORY_ALLOCATION)} \
}

