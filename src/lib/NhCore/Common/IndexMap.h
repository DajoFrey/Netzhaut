#ifndef NH_INDEXMAP_H
#define NH_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"
#include "../HashMap.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_IndexMap {
        Nh_HashMap MediaTypes;
    } Nh_IndexMap;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */

    extern Nh_IndexMap NH_INDEXMAP;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_createIndexMap(
    );

    void Nh_freeIndexMap(
    );

/** @} */

#endif 
