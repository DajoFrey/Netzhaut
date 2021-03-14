#ifndef NH_INSTALLER_LIBRARIES_H
#define NH_INSTALLER_LIBRARIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Main.h"
#include "Common/Result.h"
#include "Common/API.h"

#endif

/** @addtogroup InstallerFunctions Functions
 *  \ingroup Installer 
 *  @{
 */

    NH_INSTALLER_RESULT Nh_Installer_installIncludes(
    );

    NH_INSTALLER_RESULT Nh_Installer_prepareLibraryBuild(
    );
    
    NH_INSTALLER_RESULT Nh_Installer_buildLibrary(
        Nh_Installer_Library *Library_p, NH_BOOL installLibraries
    ); 

    NH_INSTALLER_RESULT Nh_Installer_buildNhExternal(
        NH_BOOL install
    );

/** @} */

#endif
