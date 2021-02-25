#ifndef NH_INSTALLER_MESSAGE_H
#define NH_INSTALLER_MESSAGE_H

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

    NH_INSTALLER_RESULT Nh_Installer_printVersion(
    );

    NH_INSTALLER_RESULT Nh_Installer_printHelp(
    );

    NH_INSTALLER_RESULT Nh_Installer_operationf(
        char *format_p, ...
    );
    
    NH_INSTALLER_RESULT Nh_Installer_messagef(
        char *format_p, ...
    );

    NH_INSTALLER_RESULT Nh_Installer_noticef(
        char *format_p, ...
    );

    NH_INSTALLER_RESULT Nh_Installer_externalMessage(
        char *prepend_p, char *message_p
    );

    NH_INSTALLER_RESULT Nh_Installer_exitMessage(
        NH_INSTALLER_RESULT result
    );

/** @} */

#endif
