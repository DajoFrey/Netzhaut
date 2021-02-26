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

#endif

/** @addtogroup CFunctions Functions
 *  \ingroup CCore 
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

/** @addtogroup CFunctions Functions
 *  \ingroup CCore 
 *  @{
 */

    extern Nh_Loader *NH_LOADER_P;

/** @} */

/** @addtogroup CFunctions Functions
 *  \ingroup CCore 
 *  @{
 */

    NH_RESULT Nh_initLoader(
         NH_LOADER_SCOPE loaderScope, NH_BYTE *loaderPath_p, NH_BOOL install
    );

    NH_RESULT Nh_init(
    );

    unsigned int Nh_run(
    );

    NH_BOOL Nh_keepRunning(
    );

    NH_RESULT Nh_release(
    );

/** @} */

#endif // NETZHAUT_H
