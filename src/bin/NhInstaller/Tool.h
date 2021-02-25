#ifndef NH_INSTALLER_TOOL_H
#define NH_INSTALLER_TOOL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"

#endif

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    NH_INSTALLER_RESULT Nh_Installer_buildTool(
        const char *name_p
    );

    NH_INSTALLER_RESULT Nh_Installer_runTool(
        const char *name_p, const char *arguments_p
    );

/** @} */

#endif
