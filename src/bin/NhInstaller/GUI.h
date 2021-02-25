#ifndef NH_INSTALLER_GUI_H
#define NH_INSTALLER_GUI_H

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

    NH_INSTALLER_RESULT Nh_Installer_runGUI(
    );

    NH_INSTALLER_RESULT Nh_Installer_handleGUIExpose(
    );

    void Nh_Installer_handleGUIExit(
    );

/** @} */

#endif
