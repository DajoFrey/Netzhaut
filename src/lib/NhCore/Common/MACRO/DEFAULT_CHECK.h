/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#undef  NH_CHECK
#define NH_CHECK(checkable)                                         \
{                                                                   \
    NH_RESULT checkResult = checkable;                              \
    if (checkResult != NH_SUCCESS) {NH_DIAGNOSTIC_END(checkResult)} \
}

#undef  NH_CHECK_NULL
#define NH_CHECK_NULL(checkable)                                          \
{                                                                         \
    void *checkResult_p = checkable;                                      \
    if (checkResult_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_NULL_POINTER)} \
}

#undef  NH_CHECK_MEM
#define NH_CHECK_MEM(checkable)                                                \
{                                                                              \
    void *checkResult_p = checkable;                                           \
    if (checkResult_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_MEMORY_ALLOCATION)} \
}

#undef  NH_CHECK_VULKAN
#define NH_CHECK_VULKAN(checkable)                                                   \
{                                                                                    \
    VkResult checkResult = checkable;                                                \
    if (checkResult != VK_SUCCESS) {NH_DIAGNOSTIC_END(Nh_Vk_getResult(checkResult))} \
}

#undef  NH_CHECK_TRUE
#define NH_CHECK_TRUE(checkable)                                            \
{                                                                           \
    NH_BOOL checkResult = checkable;                                        \
    if (checkResult != NH_TRUE) {NH_DIAGNOSTIC_END(NH_ERROR_EXPECTED_TRUE)} \
}

#undef  NH_CHECK_FALSE
#define NH_CHECK_FALSE(checkable)                                             \
{                                                                             \
    NH_BOOL checkResult = checkable;                                          \
    if (checkResult != NH_FALSE) {NH_DIAGNOSTIC_END(NH_ERROR_EXPECTED_FALSE)} \
}

#undef  NH_CHECK_TYPE
#define NH_CHECK_TYPE(type1, type2) if (type1 != type2) {NH_DIAGNOSTIC_END(NH_ERROR_TYPE_MISMATCH)}

