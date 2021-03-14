/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_ENCODING_CHECK
#define NH_ENCODING_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_ENCODING_RESULT checkResult = checkable;                          \
    if (checkResult != NH_ENCODING_SUCCESS) {NH_ENCODING_END(checkReturn)} \
}

#undef  NH_ENCODING_CHECK_NULL
#define NH_ENCODING_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_ENCODING_END(checkReturn)} \
}

#undef  NH_ENCODING_CHECK_MEM
#define NH_ENCODING_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_ENCODING_END(checkReturn)} \
}

