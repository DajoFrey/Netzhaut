// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "TestAndCompare.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>

// ABSTRACT ========================================================================================

// https://tc39.es/ecma262/#sec-isextensible-o
Nh_ECMAScript_Completion Nh_ECMAScript_isExtensible(
    Nh_ECMAScript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_BOOLEAN boolean = Object_p->InternalMethods_p->isExtensible_f(Object_p);
    Nh_ECMAScript_Completion Result = Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapBoolean(boolean));

NH_ECMASCRIPT_END(Result)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isCallable(
    Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    if (Value.type != NH_ECMASCRIPT_TYPE_OBJECT) {NH_ECMASCRIPT_END(NH_FALSE)}

    if (Value.handle_p != NULL && ((Nh_ECMAScript_Object*)Value.handle_p)->InternalMethods_p->call_f != NULL) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

