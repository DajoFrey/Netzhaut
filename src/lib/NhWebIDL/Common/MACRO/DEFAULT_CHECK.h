/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WEBIDL_CHECK
#define NH_WEBIDL_CHECK(checkable)                                                \
{                                                                                 \
    NH_WEBIDL_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_WEBIDL_SUCCESS) {NH_WEBIDL_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_WEBIDL_SILENT_CHECK
#define NH_WEBIDL_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_WEBIDL_RESULT checkResult = checkable;                      \
    if (checkResult != NH_WEBIDL_SUCCESS) {NH_WEBIDL_SILENT_END()} \
}

#undef  NH_WEBIDL_CHECK_NULL
#define NH_WEBIDL_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_NULL_POINTER)} \
}

#undef  NH_WEBIDL_CHECK_MEM
#define NH_WEBIDL_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_MEMORY_ALLOCATION)} \
}

