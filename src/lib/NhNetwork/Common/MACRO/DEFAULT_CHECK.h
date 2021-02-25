/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_NET_CHECK
#define NH_NET_CHECK(checkable)                                             \
{                                                                           \
    NH_NET_RESULT checkResult = checkable;                                  \
    if (checkResult != NH_NET_SUCCESS) {NH_NET_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_NET_CHECK_NULL
#define NH_NET_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_NET_DIAGNOSTIC_END(NH_NET_ERROR_NULL_POINTER)} \
}

#undef  NH_NET_CHECK_MEM
#define NH_NET_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_NET_DIAGNOSTIC_END(NH_NET_ERROR_MEMORY_ALLOCATION)} \
}

