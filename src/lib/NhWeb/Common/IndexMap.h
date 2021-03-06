#ifndef NH_WEB_INDEXMAP_H
#define NH_WEB_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../../NhCore/HashMap.h"

#endif

/** @addtogroup NhWebStructs
 *  @{
 */

    typedef struct Nh_Web_IndexMap {
        Nh_HashMap FragmentNames;
        Nh_HashMap FragmentFunctionNames;
        Nh_HashMap ParseNodeNames;
    } Nh_Web_IndexMap;

/** @} */

/** @addtogroup NhWebVars
 *  @{
 */

    extern Nh_Web_IndexMap NH_WEB_INDEXMAP;

/** @} */

/** @addtogroup NhWebFunctions
 *  @{
 */

    NH_WEB_RESULT Nh_Web_createIndexMap(
    );

    void Nh_Web_freeIndexMap(
    );

/** @} */

#endif 
