/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_LOADER_CHECK
#define NH_LOADER_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_LOADER_RESULT checkResult = checkable;                          \
    if (checkResult != NH_LOADER_SUCCESS) {NH_LOADER_END(checkReturn)} \
}

#undef  NH_LOADER_CHECK_NULL
#define NH_LOADER_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_LOADER_END(checkReturn)} \
}

#undef  NH_LOADER_CHECK_MEM
#define NH_LOADER_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_LOADER_END(checkReturn)} \
}

