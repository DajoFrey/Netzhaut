#ifndef NH_ECMASCRIPT_MISCELLANEOUS_STATIC_SEMANTICS_H
#define NH_ECMASCRIPT_MISCELLANEOUS_STATIC_SEMANTICS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

#include "../Engine/Parser.h"

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    NH_BOOL Nh_ECMAScript_isAnonymousFunctionDefinition(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_UTF8String *Nh_ECMAScript_getStringValue(
        Nh_ECMAScript_ParseNode *Node_p
    );

    NH_BOOL Nh_ECMAScript_isConstantDeclaration(
        Nh_ECMAScript_ParseNode *Node_p
    );

/** @} */

#endif
