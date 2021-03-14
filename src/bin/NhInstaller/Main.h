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

    #define NH_INSTALLER_MAX_SOURCES   1024
    #define NH_INSTALLER_MAX_LIBRARIES 255

/** @} */

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    typedef struct Nh_Installer_Library {
        NH_BYTE *name_p;
        NH_BYTE *compileArgs_p;
        NH_BYTE *linkArgs_p;
        NH_BOOL build;
    } Nh_Installer_Library;

    typedef struct Nh_Installer_Source {
        Nh_Installer_Library *Library_p;
        NH_BYTE *path_p;
    } Nh_Installer_Source;

/** @} */

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    extern Nh_Installer_Library NH_INSTALLER_LIBRARIES_P[NH_INSTALLER_MAX_LIBRARIES];
    extern Nh_Installer_Source NH_INSTALLER_SOURCES_P[NH_INSTALLER_MAX_SOURCES];

    extern NH_BYTE *NH_INSTALLER_FILE_DATA_P;

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
