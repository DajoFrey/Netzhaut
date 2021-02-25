/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Message.h"
#include "../Result.h"

#undef  NH_INSTALLER_CHECK
#define NH_INSTALLER_CHECK(checkable)                                                            \
{                                                                                      \
    NH_INSTALLER_RESULT checkResult = checkable;                                            \
    if (checkResult != NH_INSTALLER_SUCCESS) {                                              \
        Nh_Installer_messagef("-> \e[1;31mERROR\e[0m %s", NH_INSTALLER_RESULTS_PP[checkResult]); \
        return checkResult;                                                            \
    }                                                                                  \
}

#undef  NH_INSTALLER_CHECK_NULL
#define NH_INSTALLER_CHECK_NULL(checkable)                                                                      \
{                                                                                                     \
    void *checkResult_p = checkable;                                                                  \
    if (checkResult_p == NULL) {                                                                      \
        Nh_Installer_messagef("-> \e[1;31mERROR\e[0m %s", NH_INSTALLER_RESULTS_PP[NH_INSTALLER_ERROR_NULL_POINTER]); \
        return NH_INSTALLER_ERROR_NULL_POINTER;                                                            \
    }                                                                                                 \
}

