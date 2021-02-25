#ifndef NH_ECMASCRIPT_BOUND_NAMES_H
#define NH_ECMASCRIPT_BOUND_NAMES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Engine/Parser.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

// correponds to https://tc39.es/ecma262/#sec-syntax-directed-operations-scope-analysis

    Nh_List Nh_ECMAScript_getBoundNames(
        Nh_ECMAScript_ParseNode *Node_p
    );

    void Nh_ECMAScript_freeBoundNames(
        Nh_List *Names_p
    );

/** @} */

#endif
