/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_ENCODING_CHECK
#define NH_ENCODING_CHECK(checkable)                                                \
{                                                                                 \
    NH_ENCODING_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_ENCODING_SUCCESS) {NH_ENCODING_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_ENCODING_SILENT_CHECK
#define NH_ENCODING_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_ENCODING_RESULT checkResult = checkable;                      \
    if (checkResult != NH_ENCODING_SUCCESS) {NH_ENCODING_SILENT_END()} \
}

#undef  NH_ENCODING_CHECK_NULL
#define NH_ENCODING_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_ERROR_NULL_POINTER)} \
}

#undef  NH_ENCODING_CHECK_MEM
#define NH_ENCODING_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_ERROR_MEMORY_ALLOCATION)} \
}

