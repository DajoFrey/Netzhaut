#ifndef NH_WEBIDL_INDEXMAP_H
#define NH_WEBIDL_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"
#include "../../NhCore/HashMap.h"

#endif

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    typedef struct Nh_WebIDL_IndexMap {
        Nh_HashMap FragmentNames;
        Nh_HashMap FragmentFunctionNames;
    } Nh_WebIDL_IndexMap;

/** @} */

/** @addtogroup WebIDLData Data
 *  \ingroup WebIDL
 *  @{
 */

    extern Nh_WebIDL_IndexMap NH_WEBIDL_INDEXMAP;

/** @} */

/** @addtogroup WebIDLFunctions Functions
 *  \ingroup WebIDL
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_createIndexMap(
    );

    void Nh_WebIDL_freeIndexMap(
    );

/** @} */

#endif 
