#ifndef NH_LOADER_REPOSITORY_H
#define NH_LOADER_REPOSITORY_H

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

    NH_LOADER_RESULT Nh_Loader_getExeDir(
        NH_BYTE *buffer_p, unsigned int size
    );

    NH_LOADER_RESULT Nh_Loader_downloadNetzhaut(
    );
    
    NH_LOADER_RESULT Nh_Loader_buildNhInstaller(
    );

/** @} */

#endif
