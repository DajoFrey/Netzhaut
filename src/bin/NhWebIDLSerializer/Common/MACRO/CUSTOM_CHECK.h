/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Result.h"

#undef  NH_WEBIDL_SERIALIZER_CHECK
#define NH_WEBIDL_SERIALIZER_CHECK(checkReturn, checkable)                        \
{                                                                      \
    NH_WEBIDL_SERIALIZER_RESULT checkResult = checkable;                          \
    if (checkResult != NH_WEBIDL_SERIALIZER_SUCCESS) {NH_WEBIDL_SERIALIZER_END(checkReturn)} \
}

#undef  NH_WEBIDL_SERIALIZER_CHECK_NULL
#define NH_WEBIDL_SERIALIZER_CHECK_NULL(checkReturn, checkable)        \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEBIDL_SERIALIZER_END(checkReturn)} \
}

#undef  NH_WEBIDL_SERIALIZER_CHECK_MEM
#define NH_WEBIDL_SERIALIZER_CHECK_MEM(checkReturn, checkable)         \
{                                                           \
    void *checkResult_p = checkable;                        \
    if (checkResult_p == NULL) {NH_WEBIDL_SERIALIZER_END(checkReturn)} \
}

