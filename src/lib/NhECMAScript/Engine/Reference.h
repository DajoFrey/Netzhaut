#ifndef NH_ECMASCRIPT_REFERENCE_H
#define NH_ECMASCRIPT_REFERENCE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Realm.h"
#include "Parser.h"
#include "Environment.h"

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Array.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_ReferenceBase {
        NH_BOOL unresolvable;
        Nh_ECMAScript_Any Value;
        Nh_ECMAScript_Environment *Environment_p;
    } Nh_ECMAScript_ReferenceBase;

    typedef struct Nh_ECMAScript_ReferenceValue {
        NH_BOOL empty;
        Nh_ECMAScript_Any Payload;
    } Nh_ECMAScript_ReferenceValue;

    typedef struct Nh_ECMAScript_Reference {
        Nh_ECMAScript_ReferenceBase Base;
        Nh_ECMAScript_Any ReferencedName;
        NH_ECMASCRIPT_BOOLEAN strict;
        Nh_ECMAScript_ReferenceValue ThisValue;
    } Nh_ECMAScript_Reference;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Reference Nh_ECMAScript_initReference(
    );

    Nh_ECMAScript_Reference Nh_ECMAScript_getIdentifierReference(
        Nh_ECMAScript_Environment *Environment_p, Nh_UTF8String *Name_p, NH_ECMASCRIPT_BOOLEAN strict
    );

    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isPropertyReference(
        Nh_ECMAScript_Reference Reference
    );
    
    Nh_ECMAScript_Completion Nh_ECMAScript_getValue(
        Nh_ECMAScript_Completion Completion, Nh_ECMAScript_Reference Reference
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_putValue(
        Nh_ECMAScript_Reference Reference, Nh_ECMAScript_Any Value
    );

/** @} */

#endif
