/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_NET_CHECK
#define NH_NET_CHECK(checkReturn, checkable)                     \
{                                                                \
    NH_NET_RESULT checkResult = checkable;                       \
    if (checkResult != NH_NET_SUCCESS) {NH_NET_END(checkReturn)} \
}

#undef  NH_NET_CHECK_NULL
#define NH_NET_CHECK_NULL(checkReturn, checkable)        \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_NET_END(checkReturn)} \
}

#undef  NH_NET_CHECK_MEM
#define NH_NET_CHECK_MEM(checkReturn, checkable)         \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_NET_END(checkReturn)} \
}

