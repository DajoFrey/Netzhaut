/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WSI_CHECK
#define NH_WSI_CHECK(checkable)                                            \
{                                                                         \
    NH_WSI_RESULT checkResult = checkable;                                 \
    if (checkResult != NH_WSI_SUCCESS) {NH_WSI_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_WSI_CHECK_NULL
#define NH_WSI_CHECK_NULL(checkable)                                             \
{                                                                               \
    void *checkResult_p = checkable;                                            \
    if (checkResult_p == NULL) {NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_NULL_POINTER)} \
}

#undef  NH_WSI_CHECK_MEM
#define NH_WSI_CHECK_MEM(checkable)                                                      \
{                                                                                    \
    void *checkResult_p = checkable;                                                 \
    if (checkResult_p == NULL) {NH_WSI_DIAGNOSTIC_END(NH_WSI_ERROR_MEMORY_ALLOCATION)} \
}

