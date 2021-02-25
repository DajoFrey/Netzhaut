#ifndef NH_ECMASCRIPT_INDEXMAP_H
#define NH_ECMASCRIPT_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"
#include "../../NhCore/HashMap.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_IndexMap {
        Nh_HashMap ParseNodeNames;
        Nh_HashMap ReservedWords;
    } Nh_ECMAScript_IndexMap;

/** @} */

/** @addtogroup NhECMAScriptVars
 *  @{
 */

    extern Nh_ECMAScript_IndexMap NH_ECMASCRIPT_INDEXMAP;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIndexMap(
    );

    void Nh_ECMAScript_freeIndexMap(
    );

/** @} */

#endif 
