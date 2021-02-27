/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_ECMASCRIPT_CHECK
#define NH_ECMASCRIPT_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_ECMASCRIPT_RESULT checkResult = checkable;                          \
    if (checkResult != NH_ECMASCRIPT_SUCCESS) {NH_ECMASCRIPT_END(checkReturn)} \
}

#undef  NH_ECMASCRIPT_CHECK_NULL
#define NH_ECMASCRIPT_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_ECMASCRIPT_END(checkReturn)} \
}

#undef  NH_ECMASCRIPT_CHECK_MEM
#define NH_ECMASCRIPT_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_ECMASCRIPT_END(checkReturn)} \
}

