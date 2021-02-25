/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_IO_CHECK
#define NH_IO_CHECK(checkable)                                            \
{                                                                         \
    NH_IO_RESULT checkResult = checkable;                                 \
    if (checkResult != NH_IO_SUCCESS) {NH_IO_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_IO_CHECK_NULL
#define NH_IO_CHECK_NULL(checkable)                                             \
{                                                                               \
    void *checkResult_p = checkable;                                            \
    if (checkResult_p == NULL) {NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_NULL_POINTER)} \
}

#undef  NH_IO_CHECK_MEM
#define NH_IO_CHECK_MEM(checkable)                                                      \
{                                                                                    \
    void *checkResult_p = checkable;                                                 \
    if (checkResult_p == NULL) {NH_IO_DIAGNOSTIC_END(NH_IO_ERROR_MEMORY_ALLOCATION)} \
}

