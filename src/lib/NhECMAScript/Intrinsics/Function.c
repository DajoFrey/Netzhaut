// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Function.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>

// INTERNAL METHODS ================================================================================

//// https://tc39.es/ecma262/#sec-immutable-prototype-exotic-objects-setprototypeof-v
//static NH_ECMASCRIPT_BOOLEAN _Nh_ECMAScript_setPrototypeOf(
//    Nh_ECMAScript_Object *This_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//NH_ECMASCRIPT_END(NH_FALSE)
//}

//// DATA ============================================================================================
//
//static Nh_ECMAScript_InternalMethods FunctionPrototypeInternalMethods = {
//    .getPrototypeOf_f    = Nh_ECMAScript_ordinaryGetPrototypeOf,
////    .setPrototypeOf_f    = _Nh_ECMAScript_setPrototypeOf,
//    .isExtensible_f      = Nh_ECMAScript_ordinaryIsExtensible,
//    .preventExtensions_f = Nh_ECMAScript_ordinaryPreventExtensions,
//    .getOwnProperty_f    = Nh_ECMAScript_ordinaryGetOwnProperty,
//    .defineOwnProperty_f = Nh_ECMAScript_ordinaryDefineOwnProperty,
//    .hasProperty_f       = Nh_ECMAScript_ordinaryHasProperty,
//    .get_f               = Nh_ECMAScript_ordinaryGet,
//    .set_f               = Nh_ECMAScript_ordinarySet,
//    .delete_f            = Nh_ECMAScript_ordinaryDelete,
//    .ownPropertyKeys_f   = Nh_ECMAScript_ordinaryOwnPropertyKeys,
////    .call_f              = NULL,
////    .construct_f         = NULL,
//};
//
//static const char *cProps_pp[] = {
//    "length",
//    "assign",
//    "create",
//    "defineProperties",
//    "defineProperty",
//    "entries",
//    "freeze",
//    "fromEntries",
//    "getOwnPropertyDescriptor",
//    "getOwnPropertyDescriptors",
//    "getOwnPropertyNames",
//    "getOwnPropertySymbols",
//    "getPrototypeOf",
//    "is",
//    "isExtensible",
//    "isFrozen",
//    "isSealed",
//    "keys",
//    "preventExtensions",
//    "seal",
//    "setPrototypeOf",
//    "values",
//};
//
//static const char *pProps_pp[] = {
//    "constructor",
//    "hasOwnProperty",
//    "isPrototypeOf",
//    "propertyIsEnumerable",
//    "toLocaleString",
//    "toString",
//    "valueOf",
//};
//
//// BUILTIN OBJECT CONSTRUCTOR FUNCTIONS ============================================================
//
////static Nh_ECMAScript_Any Nh_ECMAScript_assign(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_create(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_defineProperties(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_defineProperty(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_entries(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_freeze(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_fromEntries(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyDescriptor(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyDescriptors(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertyNames(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_getOwnPropertySymbols(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_getPrototypeOf(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_is(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_isExtensible(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_isFrozen(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_isSealed(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_keys(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_preventExtensions(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_seal(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_setPrototypeOf(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_values(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
//
//// BUILTIN OBJECT PROTOTYPE FUNCTIONS ==============================================================
//
////static Nh_ECMAScript_Any Nh_ECMAScript_hasOwnProperty(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_isPrototypeOf(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_propertyIsEnumerable(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_toLocaleString(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any _Nh_ECMAScript_toString(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
////
////static Nh_ECMAScript_Any Nh_ECMAScript_valueOf(
////    Nh_ECMAScript_Object *This_p, Nh_List Arguments)
////{
////NH_ECMASCRIPT_BEGIN()
////
////NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
////}
//
//// CREATE ==========================================================================================
//
//static NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsicFunctionConstructor(
//    Nh_ECMAScript_IntrinsicFunction *Function_p, Nh_ECMAScript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
////    Object_p->Constructor.Properties = Nh_ECMAScript_initProperties(22);
//////    Object_p->Prototype.InternalSlots = Nh_initInternalSlots(ordinaryInternalSlotLookUp_p);
//////    Object_p->Prototype.InternalSlots.values_pp[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE] = NH_TRUE;
//////    Object_p->Prototype.InternalSlots.values_pp[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE] = NULL;
//////    Object_p->Prototype.InternalMethods_p = ObjectPrototypeInternalMethods;
////
////    Object_p->Constructor.InternalSlots.values_pp[
////        Object_p->Constructor.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]
////    ] = (void*)NH_FALSE;
////    Object_p->Constructor.InternalSlots.values_pp[
////        Object_p->Constructor.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE]
////    ] = &Realm_p->Intrinsics.Function.Prototype;
////
//////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createBuiltinFunction(
//////        &Object_p->Constructor, Nh_ECMAScript_construct_f, NULL, Realm_p, NULL 
//////    ))
////
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Assign, cProps_pp[1], Nh_ECMAScript_assign, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Create, cProps_pp[2], Nh_ECMAScript_create, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->DefineProperties, cProps_pp[3], Nh_ECMAScript_defineProperties, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->DefineProperty, cProps_pp[4], Nh_ECMAScript_defineProperty, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Entries, cProps_pp[5], Nh_ECMAScript_entries, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Freeze, cProps_pp[6], Nh_ECMAScript_freeze, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->FromEntries, cProps_pp[7], Nh_ECMAScript_fromEntries, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyDescriptor, cProps_pp[8], Nh_ECMAScript_getOwnPropertyDescriptor, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyDescriptors, cProps_pp[9], Nh_ECMAScript_getOwnPropertyDescriptors, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertyNames, cProps_pp[10], Nh_ECMAScript_getOwnPropertyNames, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetOwnPropertySymbols, cProps_pp[11], Nh_ECMAScript_getOwnPropertySymbols, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->GetPrototypeOf, cProps_pp[12], Nh_ECMAScript_getPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Is, cProps_pp[13], Nh_ECMAScript_is, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsExtensible, cProps_pp[14], Nh_ECMAScript_isExtensible, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsFrozen, cProps_pp[15], Nh_ECMAScript_isFrozen, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->IsSealed, cProps_pp[16], Nh_ECMAScript_isSealed, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Keys, cProps_pp[17], Nh_ECMAScript_keys, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->PreventExtensions, cProps_pp[18], Nh_ECMAScript_preventExtensions, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Seal, cProps_pp[19], Nh_ECMAScript_seal, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->SetPrototypeOf, cProps_pp[20], Nh_ECMAScript_setPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Constructor, &Object_p->Values, cProps_pp[21], Nh_ECMAScript_values, Realm_p
////    ))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//static NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsicFunctionPrototype(
//    Nh_ECMAScript_IntrinsicFunction *Function_p, Nh_ECMAScript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
////    Object_p->Prototype.Properties = Nh_ECMAScript_initProperties(7);
////    Object_p->Prototype.InternalSlots = Nh_ECMAScript_initInternalSlots(NULL, -1);
////    Object_p->Prototype.InternalMethods_p = &ObjectPrototypeInternalMethods;
////
////    Object_p->Prototype.InternalSlots.values_pp[
////        Object_p->Prototype.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]
////    ] = (void*)NH_TRUE;
////    Object_p->Prototype.InternalSlots.values_pp[
////        Object_p->Prototype.InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE]
////    ] = NULL;
////
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicDataProperty(
////        &Object_p->Prototype, pProps_pp[0], NH_FALSE, NH_FALSE, NH_FALSE, NH_ECMASCRIPT_TYPE_OBJECT,
////        &Realm_p->Intrinsics.Object.Constructor
////    ))
////
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->HasOwnProperty, pProps_pp[1], Nh_ECMAScript_hasOwnProperty, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->IsPrototypeOf, pProps_pp[2], Nh_ECMAScript_isPrototypeOf, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->PropertyIsEnumerable, pProps_pp[3], Nh_ECMAScript_propertyIsEnumerable, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ToLocaleString, pProps_pp[4], Nh_ECMAScript_toLocaleString, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ToString, pProps_pp[5], _Nh_ECMAScript_toString, Realm_p
////    ))
////    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicBuiltinFunctionObject(
////        &Object_p->Prototype, &Object_p->ValueOf, pProps_pp[6], Nh_ECMAScript_valueOf, Realm_p
////    ))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//NH_ECMASCRIPT_RESULT Nh_ECMAScript_createIntrinsicFunction(
//    Nh_ECMAScript_IntrinsicFunction *Function_p, Nh_ECMAScript_Realm *Realm_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicFunctionConstructor(Function_p, Realm_p))
//    NH_ECMASCRIPT_CHECK(Nh_ECMAScript_createIntrinsicFunctionPrototype(Function_p, Realm_p))
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
//NH_ECMASCRIPT_RESULT Nh_ECMAScript_freeIntrinsicFunction(
//    Nh_ECMAScript_IntrinsicFunction *Function_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//    // TODO
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}
//
