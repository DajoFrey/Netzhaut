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

/** @addtogroup NhLoaderEnums
 *  @{
 */

    typedef enum NH_LOADER_BEHAVIOR {
        NH_LOADER_BEHAVIOR_NO_INSTALL,
        NH_LOADER_BEHAVIOR_LOCAL_INSTALL,
        NH_LOADER_BEHAVIOR_SYSTEM_INSTALL,
    } NH_LOADER_BEHAVIOR;

/** @} */

#endif
