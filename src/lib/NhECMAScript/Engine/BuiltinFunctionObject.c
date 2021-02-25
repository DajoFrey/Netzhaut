// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "BuiltinFunctionObject.h"
#include "OrdinaryObject.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// BUILTIN FUNCTION ================================================================================

static Nh_ECMAScript_Completion Nh_ECMAScript_builtinCall(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any ThisArgument, Nh_List ArgumentsList)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_normalEmptyCompletion())
}

static Nh_ECMAScript_Object *Nh_ECMAScript_builtinConstruct(
    Nh_List Arguments, Nh_ECMAScript_Object *Target_p)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(NULL)
}

// DATA ============================================================================================

static Nh_ECMAScript_InternalMethods BuiltinFunctionObjectInternalMethods = {
    .getPrototypeOf_f    = Nh_ECMAScript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = Nh_ECMAScript_ordinarySetPrototypeOf,
    .isExtensible_f      = Nh_ECMAScript_ordinaryIsExtensible,
    .preventExtensions_f = Nh_ECMAScript_ordinaryPreventExtensions,
    .getOwnProperty_f    = Nh_ECMAScript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = Nh_ECMAScript_ordinaryDefineOwnProperty,
    .hasProperty_f       = Nh_ECMAScript_ordinaryHasProperty,
    .get_f               = Nh_ECMAScript_ordinaryGet,
    .set_f               = Nh_ECMAScript_ordinarySet,
    .delete_f            = Nh_ECMAScript_ordinaryDelete,
    .ownPropertyKeys_f   = Nh_ECMAScript_ordinaryOwnPropertyKeys,
    .call_f              = Nh_ECMAScript_builtinCall,
    .construct_f         = Nh_ECMAScript_builtinConstruct,
};

static int builtinFunctionObjectLookup_p[] = {
     0,  1,  2,  3,  4
};

// CREATE ==========================================================================================

// https://tc39.es/ecma262/#sec-createbuiltinfunction
Nh_ECMAScript_Object *Nh_ECMAScript_createBuiltinFunctionObject(
    Nh_ECMAScript_Object *FunctionObject_p, Nh_ECMAScript_Any (*call_f)(Nh_ECMAScript_Object *This_p, Nh_List Arguments),
    int *lookup_p, int lookupLength, Nh_ECMAScript_Realm *Realm_p, Nh_ECMAScript_Object *Prototype_p)
{
NH_ECMASCRIPT_BEGIN()

    if (Prototype_p == NULL) {
        Prototype_p = &Realm_p->Intrinsics.Function.Prototype;
    }

    if (FunctionObject_p == NULL) {
        FunctionObject_p = Nh_allocate(sizeof(Nh_ECMAScript_Object));
        NH_ECMASCRIPT_CHECK_MEM(NULL, FunctionObject_p)
    }

    if (lookup_p == NULL || lookupLength < 5) {
        lookup_p = builtinFunctionObjectLookup_p;
        lookupLength = 5;
    }

    if (lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_REALM] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN] < 0
    ||  lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME] < 0) {
        NH_ECMASCRIPT_END(NULL)
    }

    FunctionObject_p->InternalSlots     = Nh_ECMAScript_initInternalSlots(lookup_p, lookupLength);
    FunctionObject_p->Properties        = Nh_initList(8);
    FunctionObject_p->InternalMethods_p = &BuiltinFunctionObjectInternalMethods;

    Nh_ECMAScript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_REALM, Realm_p);
    Nh_ECMAScript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, Prototype_p);
    Nh_ECMAScript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*)NH_TRUE);
    Nh_ECMAScript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_INITIAL_NAME, NULL);
    Nh_ECMAScript_setInternalSlot(&FunctionObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_BUILTIN, call_f);

NH_ECMASCRIPT_END(FunctionObject_p)
}

