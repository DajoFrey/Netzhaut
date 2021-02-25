#ifndef NH_INSTALLER_EXAMPLE_H
#define NH_INSTALLER_EXAMPLE_H

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

    NH_INSTALLER_RESULT Nh_Installer_buildWebBrowser(
    );

    NH_INSTALLER_RESULT Nh_Installer_installWebBrowser(
    );

    NH_INSTALLER_RESULT Nh_Installer_buildTerminal(
    );

    NH_INSTALLER_RESULT Nh_Installer_installTerminal(
    );

    NH_INSTALLER_RESULT Nh_Installer_installLogo(
    );

/** @} */

#endif
