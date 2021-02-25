#ifndef NH_ECMASCRIPT_ORDINARY_FUNCTION_OBJECT_H
#define NH_ECMASCRIPT_ORDINARY_FUNCTION_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"
#include "ObjectType.h"
#include "Parser.h"
#include "Environment.h"

#include "../../NhCore/List.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_THIS_MODE {
        NH_ECMASCRIPT_THIS_MODE_LEXICAL,
        NH_ECMASCRIPT_THIS_MODE_NON_LEXICAL,
        NH_ECMASCRIPT_THIS_MODE_STRICT,
        NH_ECMASCRIPT_THIS_MODE_GLOBAL,
    } NH_ECMASCRIPT_THIS_MODE;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    // https://tc39.es/ecma262/#sec-ordinaryfunctioncreate
    Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryFunctionCreate(
        Nh_ECMAScript_Object *Prototype_p, void *SourceText_p, Nh_ECMAScript_ParseNode *ParameterList_p, 
        Nh_ECMAScript_ParseNode *Body_p, NH_ECMASCRIPT_THIS_MODE thisMode, Nh_ECMAScript_Environment *Scope_p 
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_makeConstructor(
        Nh_ECMAScript_Object *Function_p, NH_BOOL writablePrototype, Nh_ECMAScript_Object *Prototype_p
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_setFunctionLength(
        Nh_ECMAScript_Object *Function_p, NH_ECMASCRIPT_NUMBER len
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_setFunctionName(
        Nh_ECMAScript_Object *Function_p, Nh_ECMAScript_Any Name, NH_BYTE *prefix_p
    );

/** @} */

#endif
