#ifndef NH_LOADER_RELOAD_H
#define NH_LOADER_RELOAD_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhLoaderFunctions
 *  @{
 */

    NH_BYTE *Nh_Loader_lastModified(
        void *lib_p
    );
    
    NH_BYTE *Nh_Loader_getModifiedModuleName(
    );

/** @} */

#endif
