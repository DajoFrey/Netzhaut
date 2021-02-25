// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Object.h"

#include "../Engine/OrdinaryObject.h"
#include "../Engine/BuiltinFunctionObject.h"
#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>

// INTERNAL METHODS ================================================================================

// https://tc39.es/ecma262/#sec-immutable-prototype-exotic-objects-setprototypeof-v
static NH_ECMASCRIPT_BOOLEAN _Nh_ECMAScript_setPrototypeOf(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(NH_FALSE)
}

// DATA ============================================================================================

static Nh_ECMAScript_InternalMethods ObjectPrototypeInternalMethods = {
    .getPrototypeOf_f    = Nh_ECMAScript_ordinaryGetPrototypeOf,
    .setPrototypeOf_f    = _Nh_ECMAScript_setPrototypeOf,
    .isExtensible_f      = Nh_ECMAScript_ordinaryIsExtensible,
    .preventExtensions_f = Nh_ECMAScript_ordinaryPreventExtensions,
    .getOwnProperty_f    = Nh_ECMAScript_ordinaryGetOwnProperty,
    .defineOwnProperty_f = Nh_ECMAScript_ordinaryDefineOwnProperty,
    .hasProperty_f       = Nh_ECMAScript_ordinaryHasProperty,
    .get_f               = Nh_ECMAScript_ordinaryGet,
    .set_f               = Nh_ECMAScript_ordinarySet,
    .delete_f            = Nh_ECMAScript_ordinaryDelete,
    .ownPropertyKeys_f   = Nh_ECMAScript_ordinaryOwnPropertyKeys,
    .call_f              = NULL,
    .construct_f         = NULL,
};

static const NH_BYTE *objectPropertyNames_pp[] = {
// constructor properties
    "length",
    "assign",
    "create",
    "defineProperties",
    "defineProperty",
    "entries",
    "freeze",
    "fromEntries",
    "getOwnPropertyDescriptor",
    "getOwnPropertyDescriptors",
    "getOwnPropertyNames",
    "getOwnPropertySymbols",
    "getPrototypeOf",
    "is",
    "isExtensible",
    "isFrozen",
    "isSealed",
    "keys",
    "preventExtensions",
    "seal",
    "setPrototypeOf",
    "values",
// prototype properties
    "constructor",
    "hasOwnProperty",
    "isPrototypeOf",
    "propertyIsEnumerable",
    "toLocaleString",
    "toString",
    "valueOf",
};

// CONSTRUCTOR =====================================================================================

static Nh_ECMAScript_Any Nh_ECMAScript_construct(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

// BUILTIN OBJECT CONSTRUCTOR FUNCTIONS ============================================================

static Nh_ECMAScript_Any Nh_ECMAScript_assign(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_create(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_defineProperties(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_defineProperty(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_entries(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_freeze(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_fromEntries(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyDescriptor(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyDescriptors(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyNames(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertySymbols(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_getPrototypeOf(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_is(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_isExtensible(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_isFrozen(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_isSealed(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_keys(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_preventExtensions(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_seal(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_setPrototypeOf(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_values(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

// BUILTIN OBJECT PROTOTYPE FUNCTIONS ==============================================================

static Nh_ECMAScript_Any Nh_ECMAScript_hasOwnProperty(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_isPrototypeOf(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_propertyIsEnumerable(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_toLocaleString(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any _Nh_ECMAScript_toString(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

static Nh_ECMAScript_Any Nh_ECMAScript_valueOf(
    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_wrapUndefined())
}

// CREATE ==========================================================================================

typedef Nh_ECMAScript_Any (*builtin_f)(Nh_ECMAScript_Object *This_p, Nh_List Arguments);

static Nh_ECMAScript_Property *Nh_ECMAScript_getObjectProperty(
    Nh_ECMAScript_IntrinsicObject *Object_p, int i, builtin_f *function_pp)
{
NH_ECMASCRIPT_BEGIN()

    Object_p->Properties_p[i].type = NH_ECMASCRIPT_PROPERTY_DATA;
    Object_p->Properties_p[i].enumerable   = NH_FALSE;
    Object_p->Properties_p[i].configurable = NH_FALSE;
    Object_p->Properties_p[i].Fields.Data.writable = NH_FALSE;

    Nh_ECMAScript_Any Value;
    Value.type = NH_ECMASCRIPT_TYPE_OBJECT;
    Value.handle_p = NULL;

    switch (i)
    {
        case  0 : break;
        case  1 : Value.handle_p = &Object_p->Assign; break;
        case  2 : Value.handle_p = &Object_p->Create; break;
        case  3 : Value.handle_p = &Object_p->DefineProperties; break;
        case  4 : Value.handle_p = &Object_p->DefineProperty; break;
        case  5 : Value.handle_p = &Object_p->Entries; break;
        case  6 : Value.handle_p = &Object_p->Freeze; break;
        case  7 : Value.handle_p = &Object_p->FromEntries; break;
        case  8 : Value.handle_p = &Object_p->GetOwnPropertyDescriptor; break;
        case  9 : Value.handle_p = &Object_p->GetOwnPropertyDescriptors; break;
        case 10 : Value.handle_p = &Object_p->GetOwnPropertyNames; break;
        case 11 : Value.handle_p = &Object_p->GetOwnPropertySymbols; break;
        case 12 : Value.handle_p = &Object_p->GetPrototypeOf; break;
        case 13 : Value.handle_p = &Object_p->Is; break;
        case 14 : Value.handle_p = &Object_p->IsExtensible; break;
        case 15 : Value.handle_p = &Object_p->IsFrozen; break;
        case 16 : Value.handle_p = &Object_p->IsSealed; break;
        case 17 : Value.handle_p = &Object_p->Keys; break;
        case 18 : Value.handle_p = &Object_p->PreventExtensions; break;
        case 19 : Value.handle_p = &Object_p->Seal; break;
        case 20 : Value.handle_p = &Object_p->SetPrototypeOf; break;
        case 21 : Value.handle_p = &Object_p->Values; break;
        case 22 : Value.handle_p = &Object_p->Constructor; break;
        case 23 : Value.handle_p = &Object_p->HasOwnProperty; break;
        case 24 : Value.handle_p = &Object_p->IsPrototypeOf; break;
        case 25 : Value.handle_p = &Object_p->PropertyIsEnumerable; break;
        case 26 : Value.handle_p = &Object_p->ToLocaleString; break;
        case 27 : Value.handle_p = &Object_p->ToString; break;
        case 28 : Value.handle_p = &Object_p->ValueOf; break;
    }

    Object_p->Properties_p[i].Fields.Data.Value = Value;
    *function_pp = NULL;

    switch (i)
    {
        case  0 : break;
        case  1 : *function_pp = Nh_ECMAScript_assign; break;
        case  2 : *function_pp = Nh_ECMAScript_create; break;
        case  3 : *function_pp = Nh_ECMAScript_defineProperties; break;
        case  4 : *function_pp = Nh_ECMAScript_defineProperty; break;
        case  5 : *function_pp = Nh_ECMAScript_entries; break;
        case  6 : *function_pp = Nh_ECMAScript_freeze; break;
        case  7 : *function_pp = Nh_ECMAScript_fromEntries; break;
        case  8 : *function_pp = Nh_ECMAScript_getOwnPropertyDescriptor; break;
        case  9 : *function_pp = Nh_ECMAScript_getOwnPropertyDescriptors; break;
        case 10 : *function_pp = Nh_ECMAScript_getOwnPropertyNames; break;
        case 11 : *function_pp = Nh_ECMAScript_getOwnPropertySymbols; break;
        case 12 : *function_pp = Nh_ECMAScript_getPrototypeOf; break;
        case 13 : *function_pp = Nh_ECMAScript_is; break;
        case 14 : *function_pp = Nh_ECMAScript_isExtensible; break;
        case 15 : *function_pp = Nh_ECMAScript_isFrozen; break;
        case 16 : *function_pp = Nh_ECMAScript_isSealed; break;
        case 17 : *function_pp = Nh_ECMAScript_keys; break;
        case 18 : *function_pp = Nh_ECMAScript_preventExtensions; break;
        case 19 : *function_pp = Nh_ECMAScript_seal; break;
        case 20 : *function_pp = Nh_ECMAScript_setPrototypeOf; break;
        case 21 : *function_pp = Nh_ECMAScript_values; break;
        case 22 : break;
        case 23 : *function_pp = Nh_ECMAScript_hasOwnProperty; break;
        case 24 : *function_pp = Nh_ECMAScript_isPrototypeOf; break;
        case 25 : *function_pp = Nh_ECMAScript_propertyIsEnumerable; break;
        case 26 : *function_pp = Nh_ECMAScript_toLocaleString; break;
        case 27 : *function_pp = _Nh_ECMAScript_toString; break;
        case 28 : *function_pp = Nh_ECMAScript_valueOf; break;
    }

NH_ECMASCRIPT_END(&Object_p->Properties_p[i])
}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsicObject(
    Nh_ECMAScript_IntrinsicObject *Object_p, Nh_ECMAScript_Realm *Realm_p)
{
NH_ECMASCRIPT_BEGIN()

    Object_p->Prototype.Properties = Nh_initList(8);
    Object_p->Prototype.InternalSlots = Nh_ECMAScript_initInternalSlots(NULL, 0);
    Object_p->Prototype.InternalMethods_p = &ObjectPrototypeInternalMethods;

    Nh_ECMAScript_setInternalSlot(&Object_p->Prototype.InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*)NH_TRUE);
    Nh_ECMAScript_setInternalSlot(&Object_p->Prototype.InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, NULL);

    NH_ECMASCRIPT_CHECK_NULL(Nh_ECMAScript_createBuiltinFunctionObject(
        &Object_p->Constructor, Nh_ECMAScript_construct, NULL, 0, Realm_p, &Realm_p->Intrinsics.Function.Prototype 
    ))

    for (int i = 0; i < sizeof(objectPropertyNames_pp) / sizeof(objectPropertyNames_pp[0]); ++i) 
    {
        Nh_ECMAScript_Object *ConstructorOrPrototype_p = i < 22 ? &Object_p->Constructor : &Object_p->Prototype;

        Object_p->Keys_p[i].bytes_p = (NH_BYTE*)objectPropertyNames_pp[i];
        Object_p->Keys_p[i].length  = strlen(objectPropertyNames_pp[i]);

        builtin_f function_p = NULL;
        Nh_ECMAScript_Property *Property_p = Nh_ECMAScript_getObjectProperty(Object_p, i, &function_p);
        Property_p->Key = Nh_ECMAScript_wrapString(&Object_p->Keys_p[i]);

        if (function_p != NULL) {
            NH_ECMASCRIPT_CHECK_NULL(Nh_ECMAScript_createBuiltinFunctionObject(
                Property_p->Fields.Data.Value.handle_p, function_p, NULL, 0, Realm_p, NULL 
            ))
        }

        Nh_appendToList(&ConstructorOrPrototype_p->Properties, Property_p);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

