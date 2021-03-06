/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WEB_CHECK
#define NH_WEB_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_WEB_RESULT checkResult = checkable;                          \
    if (checkResult != NH_WEB_SUCCESS) {NH_WEB_END(checkReturn)} \
}

#undef  NH_WEB_CHECK_NULL
#define NH_WEB_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEB_END(checkReturn)} \
}

#undef  NH_WEB_CHECK_MEM
#define NH_WEB_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEB_END(checkReturn)} \
}

