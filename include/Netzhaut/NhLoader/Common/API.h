#ifndef NH_LOADER_API_H
#define NH_LOADER_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Common/API.h"

#endif

/** @addtogroup NhLoaderStructs
 *  @{
 */

    typedef struct Nh_Loader Nh_Loader;

/** @} */

/** @addtogroup NhLoaderEnums
 *  @{
 */

    typedef enum NH_LOADER_SCOPE {
        NH_LOADER_SCOPE_LOCAL,
        NH_LOADER_SCOPE_LOCAL_SYSTEM,
        NH_LOADER_SCOPE_SYSTEM,
    } NH_LOADER_SCOPE;

/** @} */

#endif
