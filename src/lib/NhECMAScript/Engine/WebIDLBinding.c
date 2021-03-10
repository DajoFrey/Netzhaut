// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Realm.h"

#include NH_DEFAULT_CHECK
#include NH_FLOW
#include NH_JS_UTILS

#include <string.h>

// WEB BINDING ==================================================================================

// https://heycam.github.io/webidl/#interface-object
Nh_ECMAScript_Object *Nh_ECMAScript_createInterfaceObject(
    Nh_WebIDL_Interface *Interface_p, Nh_ECMAScript_Realm *Realm_p)
{
NH_ECMASCRIPT_BEGIN()

    // step 1
    // TODO

    // step 2
    Nh_ECMAScript_Object *ConstructorProto_p = &Realm_p->Intrinsics.FunctionPrototype;

NH_ECMASCRIPT_END()
}


