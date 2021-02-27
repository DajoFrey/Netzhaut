#ifndef NH_INSTALLER_BINARIES_H
#define NH_INSTALLER_BINARIES_H

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

    NH_INSTALLER_RESULT Nh_Installer_buildNhWebBrowser(
    );

    NH_INSTALLER_RESULT Nh_Installer_installNhWebBrowser(
    );

    NH_INSTALLER_RESULT Nh_Installer_buildNhTerminal(
    );

    NH_INSTALLER_RESULT Nh_Installer_installNhTerminal(
    );

    NH_INSTALLER_RESULT Nh_Installer_buildNhWebIDLSerializer(
    );

    NH_INSTALLER_RESULT Nh_Installer_installLogo(
    );

/** @} */

#endif
