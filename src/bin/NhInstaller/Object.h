#ifndef NH_INSTALLER_OBJECT_H
#define NH_INSTALLER_OBJECT_H

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

    NH_INSTALLER_RESULT Nh_Installer_createObjectsDir(
        char *wrkDir_p, char *projDir_p, char *name_p
    ); 

/** @} */

#endif
