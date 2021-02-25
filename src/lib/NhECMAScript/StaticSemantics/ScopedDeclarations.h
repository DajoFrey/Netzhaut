#ifndef NH_ECMASCRIPT_SCOPED_DECLARATIONS_H
#define NH_ECMASCRIPT_SCOPED_DECLARATIONS_H

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

    Nh_List Nh_ECMAScript_getLexicallyScopedDeclarations(
        Nh_ECMAScript_ParseNode *Node_p
    );
    
    Nh_List Nh_ECMAScript_getTopLevelLexicallyScopedDeclarations(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_List Nh_ECMAScript_getVarScopedDeclarations(
        Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_List Nh_ECMAScript_getTopLevelVarScopedDeclarations(
        Nh_ECMAScript_ParseNode *Node_p
    );

/** @} */

#endif
