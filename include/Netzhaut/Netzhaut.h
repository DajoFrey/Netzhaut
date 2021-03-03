#ifndef NETZHAUT_H
#define NETZHAUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "NhCore/Common/API.h"
#include "NhCore/Common/Result.h"
#include "NhLoader/Common/API.h"
#include "NhLoader/Common/Result.h"

#endif

/** @addtogroup NetzhautMacros 
 *  @{
 */

    /**
     * 2021-02-23 | 0
     * - Initial version.
     */
    #define NETZHAUT_MAJOR_VERSION 0

    /**
     * 2021-02-23 | 0
     * - Initial version.
     */
    #define NETZHAUT_MINOR_VERSION 0

    /**
     * 2021-02-23 | 0
     * - Initial version.
     */
    #define NETZHAUT_PATCH_VERSION 0

/** @} */

/** @addtogroup NetzhautVars 
 *  @{
 */

    extern Nh_Loader *NH_LOADER_P;

/** @} */

/** @addtogroup NetzhautFunctions
 *  @{
 */

    NH_LOADER_RESULT Nh_initialize(
         NH_LOADER_SCOPE loaderScope, NH_BYTE *loaderPath_p, NH_BOOL install
    );

    unsigned int Nh_run(
    );

    NH_BOOL Nh_keepRunning(
    );

    NH_LOADER_RESULT Nh_terminate(
    );

/** @} */

#endif // NETZHAUT_H
