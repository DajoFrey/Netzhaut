#ifndef NH_ECMASCRIPT_DECLARED_NAMES_H
#define NH_ECMASCRIPT_DECLARED_NAMES_H

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

    Nh_List Nh_ECMAScript_getLexicallyDeclaredNames(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_List Nh_ECMAScript_getTopLevelLexicallyDeclaredNames(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_List Nh_ECMAScript_getVarDeclaredNames(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_List Nh_ECMAScript_getTopLevelVarDeclaredNames(
        Nh_ECMAScript_ParseNode *Node_p
    );

/** @} */

#endif
