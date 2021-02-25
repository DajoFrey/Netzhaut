#ifndef NH_ECMASCRIPT_INSTANTIATION_H
#define NH_ECMASCRIPT_INSTANTIATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Environment.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-globaldeclarationinstantiation
    Nh_ECMAScript_Completion Nh_ECMAScript_globalDeclarationInstantiation(
        Nh_ECMAScript_ParseNode *ScriptBody_p, Nh_ECMAScript_Environment *GlobalEnvironment_p
    );

    Nh_ECMAScript_Object *Nh_ECMAScript_instantiateFunctionObject(
        Nh_ECMAScript_Environment *Scope_p, Nh_ECMAScript_ParseNode *Node_p
    );

    Nh_ECMAScript_Object *Nh_ECMAScript_instantiateOrdinaryFunctionObject(
        Nh_ECMAScript_Environment *Scope_p, Nh_ECMAScript_ParseNode *Node_p
    ); 

/** @} */

#endif
