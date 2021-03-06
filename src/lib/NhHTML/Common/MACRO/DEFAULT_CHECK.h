/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_HTML_CHECK
#define NH_HTML_CHECK(checkable)                                                \
{                                                                                 \
    NH_HTML_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_HTML_SUCCESS) {NH_HTML_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_HTML_SILENT_CHECK
#define NH_HTML_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_HTML_RESULT checkResult = checkable;                      \
    if (checkResult != NH_HTML_SUCCESS) {NH_HTML_SILENT_END()} \
}

#undef  NH_HTML_CHECK_NULL
#define NH_HTML_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_NULL_POINTER)} \
}

#undef  NH_HTML_CHECK_MEM
#define NH_HTML_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_MEMORY_ALLOCATION)} \
}

