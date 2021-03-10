/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_DOM_CHECK
#define NH_DOM_CHECK(checkReturn, checkable)                     \
{                                                                \
    NH_DOM_RESULT checkResult = checkable;                       \
    if (checkResult != NH_DOM_SUCCESS) {NH_DOM_END(checkReturn)} \
}

#undef  NH_DOM_CHECK_NULL
#define NH_DOM_CHECK_NULL(checkReturn, checkable)        \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_DOM_END(checkReturn)} \
}

#undef  NH_DOM_CHECK_MEM
#define NH_DOM_CHECK_MEM(checkReturn, checkable)         \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_DOM_END(checkReturn)} \
}

