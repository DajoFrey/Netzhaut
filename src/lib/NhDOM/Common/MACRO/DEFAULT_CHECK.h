/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_DOM_CHECK
#define NH_DOM_CHECK(checkable)                                                \
{                                                                                 \
    NH_DOM_RESULT checkResult = checkable;                                     \
    if (checkResult != NH_DOM_SUCCESS) {NH_DOM_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_DOM_SILENT_CHECK
#define NH_DOM_SILENT_CHECK(checkable)                                                \
{                                                                  \
    NH_DOM_RESULT checkResult = checkable;                      \
    if (checkResult != NH_DOM_SUCCESS) {NH_DOM_SILENT_END()} \
}

#undef  NH_DOM_CHECK_NULL
#define NH_DOM_CHECK_NULL(checkable)                                                 \
{                                                                                       \
    void *checkResult_p = checkable;                                                    \
    if (checkResult_p == NULL) {NH_DOM_DIAGNOSTIC_END(NH_DOM_ERROR_NULL_POINTER)} \
}

#undef  NH_DOM_CHECK_MEM
#define NH_DOM_CHECK_MEM(checkable)                                                       \
{                                                                                            \
    void *checkResult_p = checkable;                                                         \
    if (checkResult_p == NULL) {NH_DOM_DIAGNOSTIC_END(NH_DOM_ERROR_MEMORY_ALLOCATION)} \
}

