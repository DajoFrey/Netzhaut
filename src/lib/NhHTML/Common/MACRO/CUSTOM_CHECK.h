/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_HTML_CHECK
#define NH_HTML_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_HTML_RESULT checkResult = checkable;                          \
    if (checkResult != NH_HTML_SUCCESS) {NH_HTML_END(checkReturn)} \
}

#undef  NH_HTML_CHECK_NULL
#define NH_HTML_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_HTML_END(checkReturn)} \
}

#undef  NH_HTML_CHECK_MEM
#define NH_HTML_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_HTML_END(checkReturn)} \
}

