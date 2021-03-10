#ifndef NH_HTML_INDEXMAP_H
#define NH_HTML_INDEXMAP_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"
#include "../../NhCore/HashMap.h"

#endif

/** @addtogroup NhHTMLStructs
 *  @{
 */

    typedef struct Nh_HTML_IndexMap {
        Nh_HashMap Tags;
    } Nh_HTML_IndexMap;

/** @} */

/** @addtogroup NhHTMLVars
 *  @{
 */

    extern Nh_HTML_IndexMap NH_HTML_INDEXMAP;

/** @} */

/** @addtogroup NhHTMLFunctions
 *  @{
 */

    NH_HTML_RESULT Nh_HTML_createIndexMap(
    );

    void Nh_HTML_freeIndexMap(
    );

/** @} */

#endif 
