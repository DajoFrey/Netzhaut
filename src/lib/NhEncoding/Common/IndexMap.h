#ifndef NH_ENCODING_INDEXMAP_H
#define NH_ENCODING_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"
#include "../../NhCore/HashMap.h"

#endif

/** @addtogroup NhEncodingStructs
 *  @{
 */

    typedef struct Nh_Encoding_IndexMap {
        Nh_HashMap Labels;
    } Nh_Encoding_IndexMap;

/** @} */

/** @addtogroup NhEncodingVars
 *  @{
 */

    extern Nh_Encoding_IndexMap NH_ENCODING_INDEXMAP;

/** @} */

/** @addtogroup NhEncodingFunctions
 *  @{
 */

    NH_ENCODING_RESULT Nh_Encoding_createIndexMap(
    );

    void Nh_Encoding_freeIndexMap(
    );

/** @} */

#endif 
