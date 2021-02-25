/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WEBIDL_CHECK
#define NH_WEBIDL_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_WEBIDL_RESULT checkResult = checkable;                          \
    if (checkResult != NH_WEBIDL_SUCCESS) {NH_WEBIDL_END(checkReturn)} \
}

#undef  NH_WEBIDL_CHECK_NULL
#define NH_WEBIDL_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEBIDL_END(checkReturn)} \
}

#undef  NH_WEBIDL_CHECK_MEM
#define NH_WEBIDL_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEBIDL_END(checkReturn)} \
}

