#ifndef NH_LOADER_LIBRARY_H
#define NH_LOADER_LIBRARY_H

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

    NH_LOADER_RESULT Nh_Loader_installLibrary(
        NH_BYTE *libName_p
    );
    
    void *Nh_Loader_loadLibrary(
        NH_BYTE *libName_p, int major
    );
    
    NH_LOADER_RESULT Nh_Loader_unloadLibrary(
        void *lib_p
    );

/** @} */

#endif
