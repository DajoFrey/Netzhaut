/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_TTY_CHECK
#define NH_TTY_CHECK(checkReturn, checkable)                     \
{                                                                \
    NH_TTY_RESULT checkResult = checkable;                       \
    if (checkResult != NH_TTY_SUCCESS) {NH_TTY_END(checkReturn)} \
}

#undef  NH_TTY_CHECK_NULL
#define NH_TTY_CHECK_NULL(checkReturn, checkable)        \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_TTY_END(checkReturn)} \
}

#undef  NH_TTY_CHECK_MEM
#define NH_TTY_CHECK_MEM(checkReturn, checkable)         \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_TTY_END(checkReturn)} \
}

