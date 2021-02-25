// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Tool.h"
#include "Util.h"

#include "Common/Macro.h"

#include NH_INSTALLER_FLOW
#include NH_INSTALLER_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// BUILD ===========================================================================================

NH_INSTALLER_RESULT Nh_Installer_buildTool(
    const char *name_p)
{
NH_INSTALLER_BEGIN()

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_ERROR_BAD_STATE)
}

// RUN =============================================================================================

NH_INSTALLER_RESULT Nh_Installer_runTool(
    const char *name_p, const char *arguments_p)
{
NH_INSTALLER_BEGIN()

NH_INSTALLER_DIAGNOSTIC_END(NH_INSTALLER_SUCCESS)
}

