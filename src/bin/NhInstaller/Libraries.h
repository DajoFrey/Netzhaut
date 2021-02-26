#ifndef NH_INSTALLER_LIBRARIES_H
#define NH_INSTALLER_LIBRARIES_H

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

    NH_INSTALLER_RESULT Nh_Installer_installIncludes(
    );

    NH_INSTALLER_RESULT Nh_Installer_prepareLibraryBuild(
    );
    
    NH_INSTALLER_RESULT Nh_Installer_buildLibrary(
        char *name_p, NH_BOOL installLibraries
    ); 

/** @} */

#endif
