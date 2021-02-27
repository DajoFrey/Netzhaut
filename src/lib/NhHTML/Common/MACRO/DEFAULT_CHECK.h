/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_ECMASCRIPT_CHECK
#define NH_ECMASCRIPT_CHECK(checkable)                                                \
{                                                                                 \
    NH_ECMASCRIPT_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_ECMASCRIPT_SUCCESS) {NH_ECMASCRIPT_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_ECMASCRIPT_SILENT_CHECK
#define NH_ECMASCRIPT_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_ECMASCRIPT_RESULT checkResult = checkable;                      \
    if (checkResult != NH_ECMASCRIPT_SUCCESS) {NH_ECMASCRIPT_SILENT_END()} \
}

#undef  NH_ECMASCRIPT_CHECK_NULL
#define NH_ECMASCRIPT_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_NULL_POINTER)} \
}

#undef  NH_ECMASCRIPT_CHECK_MEM
#define NH_ECMASCRIPT_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_ERROR_MEMORY_ALLOCATION)} \
}

