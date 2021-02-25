#ifndef NH_ECMASCRIPT_OBJECT_TYPE_H
#define NH_ECMASCRIPT_OBJECT_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Completion.h"
#include "InternalSlots.h"
#include "Properties.h"

#include "../../NhCore/List.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_Object Nh_ECMAScript_Object;

    typedef struct Nh_ECMAScript_InternalMethods {
        Nh_ECMAScript_Object *(*getPrototypeOf_f) (
            Nh_ECMAScript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*setPrototypeOf_f) (
            Nh_ECMAScript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*isExtensible_f) (
            Nh_ECMAScript_Object *This_p
        );
        NH_ECMASCRIPT_BOOLEAN (*preventExtensions_f) (
            Nh_ECMAScript_Object *This_p
        );
        Nh_ECMAScript_PropertyDescriptor (*getOwnProperty_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
        );
        NH_ECMASCRIPT_BOOLEAN (*defineOwnProperty_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_PropertyDescriptor Desc
        );
        NH_ECMASCRIPT_BOOLEAN (*hasProperty_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
        );
        Nh_ECMAScript_Any (*get_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Receiver
        );
        NH_ECMASCRIPT_BOOLEAN (*set_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any V, Nh_ECMAScript_Any Receiver
        );
        NH_ECMASCRIPT_BOOLEAN (*delete_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey
        );
        Nh_List (*ownPropertyKeys_f) (
            Nh_ECMAScript_Object *This_p
        );
        Nh_ECMAScript_Completion (*call_f) (
            Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any ThisArgument, Nh_List ArgumentsList
        );
        Nh_ECMAScript_Object *(*construct_f) (
            Nh_List Arguments, Nh_ECMAScript_Object *Target_p
        );
    } Nh_ECMAScript_InternalMethods;

    typedef struct Nh_ECMAScript_Object {
        Nh_ECMAScript_Properties Properties;
        Nh_ECMAScript_InternalSlots InternalSlots;
        Nh_ECMAScript_InternalMethods *InternalMethods_p;
    } Nh_ECMAScript_Object;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

// corresponds to https://tc39.es/ecma262/#sec-operations-on-objects

    Nh_ECMAScript_Object *Nh_ECMAScript_abstractMakeBasicObject(
        const int *lookup_p, int lookupLength
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_abstractDefinePropertyOrThrow(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any ProportyKey, Nh_ECMAScript_PropertyDescriptor Descriptor
    );

    NH_BOOL Nh_ECMAScript_abstractHasProperty(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey
    );

    NH_BOOL Nh_ECMAScript_abstractHasOwnProperty(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey
    );

    NH_BOOL Nh_ECMAScript_abstractIsExtensible(
        Nh_ECMAScript_Object *Object_p
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_abstractSet(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any Key, Nh_ECMAScript_Any Value, NH_BOOL _throw
    );

    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_abstractCreateDataProperty(
        Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Value
    );

    Nh_ECMAScript_Completion Nh_ECMAScript_abstractCall(
        Nh_ECMAScript_Any F, Nh_ECMAScript_Any Value, Nh_List Arguments
    );

/** @} */

#endif
