/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef  NH_CHECK
#define NH_CHECK(checkReturn, checkable)                 \
{                                                        \
    NH_RESULT checkResult = checkable;                   \
    if (checkResult != NH_SUCCESS) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_NULL
#define NH_CHECK_NULL(checkReturn, checkable)        \
{                                                    \
    void *checkResult_p = checkable;                 \
    if (checkResult_p == NULL) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_MEM
#define NH_CHECK_MEM(checkReturn, checkable)         \
{                                                    \
    void *checkResult_p = checkable;                 \
    if (checkResult_p == NULL) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_VULKAN
#define NH_CHECK_VULKAN(checkReturn, checkable)          \
{                                                        \
    VkResult checkResult = checkable;                    \
    if (checkResult != VK_SUCCESS) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_TRUE
#define NH_CHECK_TRUE(checkReturn, checkable)         \
{                                                     \
    NH_BOOL checkResult = checkable;                  \
    if (checkResult != NH_TRUE) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_FALSE
#define NH_CHECK_FALSE(checkReturn, checkable)         \
{                                                      \
    NH_BOOL checkResult = checkable;                   \
    if (checkResult != NH_FALSE) {NH_END(checkReturn)} \
}

#undef  NH_CHECK_TYPE
#define NH_CHECK_TYPE(checkReturn, type1, type2) if (type1 != type2) {NH_END(checkReturn)}

