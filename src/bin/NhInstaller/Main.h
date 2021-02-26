#ifndef NH_INSTALLER_MAIN_H
#define NH_INSTALLER_MAIN_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/Result.h"
#include "Common/API.h"

#endif

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    extern NH_BOOL NH_INSTALLER_QUIET;
    extern NH_BOOL NH_INSTALLER_FLOW_LOGGING;
    extern NH_BOOL NH_INSTALLER_GUI;

/** @} */

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    NH_INSTALLER_RESULT Nh_Installer_main(
        int argc, char **argv_pp
    );

/** @} */

#endif
