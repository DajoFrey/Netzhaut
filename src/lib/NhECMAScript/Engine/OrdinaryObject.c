// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "OrdinaryObject.h"
#include "TestAndCompare.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// ORDINARY INTERNAL METHODS =======================================================================

Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryGetPrototypeOf(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Nh_ECMAScript_getInternalSlot(&This_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE))
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinarySetPrototypeOf(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(NH_FALSE)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryIsExtensible(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END((NH_BOOL)This_p->InternalSlots.values_pp[This_p->InternalSlots.lookup_p[NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE]])
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryPreventExtensions(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(NH_FALSE)
}

// https://tc39.es/ecma262/#sec-ordinarygetownproperty
Nh_ECMAScript_PropertyDescriptor Nh_ECMAScript_ordinaryGetOwnProperty(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Property *Property_p = NULL;

    for (int i = 0; i < This_p->Properties.size; ++i) 
    {
        Nh_ECMAScript_Property *CheckProperty_p = This_p->Properties.handles_pp[i];
        if (Nh_ECMAScript_equalPropertyKeys(CheckProperty_p->Key, PropertyKey)) {
            Property_p = CheckProperty_p;
            break;
        }
    }

    if (Property_p == NULL) {
        NH_ECMASCRIPT_END(Nh_ECMAScript_undefinedPropertyDescriptor())
    }

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = Property_p->type; 

    if (Property_p->type == NH_ECMASCRIPT_PROPERTY_DATA) {
        Descriptor.Fields.Data.Value = Property_p->Fields.Data.Value;
        Descriptor.Fields.Data.writable = Property_p->Fields.Data.writable;
    }
    else {
        Descriptor.Fields.Accessor.Get = Property_p->Fields.Accessor.Get;
        Descriptor.Fields.Accessor.Set = Property_p->Fields.Accessor.Set;
    }

    Descriptor.enumerable = Property_p->enumerable;
    Descriptor.configurable = Property_p->configurable;

NH_ECMASCRIPT_END(Descriptor)
}

// https://tc39.es/ecma262/#sec-validateandapplypropertydescriptor
static NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_validateAndApplyPropertyDescriptor(
    Nh_ECMAScript_Object *O_p, Nh_ECMAScript_Any P, NH_ECMASCRIPT_BOOLEAN extensible, 
    Nh_ECMAScript_PropertyDescriptor Desc, Nh_ECMAScript_PropertyDescriptor Current)
{
NH_ECMASCRIPT_BEGIN()

    if (Current.type == -1) 
    {
        if (extensible == NH_FALSE) {NH_ECMASCRIPT_END(NH_FALSE)}
        if (Nh_ECMAScript_isGenericDescriptor(Desc) || Nh_ECMAScript_isDataDescriptor(Desc)) {
            if (O_p != NULL) {
                if (Nh_ECMAScript_newProperty(&O_p->Properties, P, Desc) == NULL) {
                    NH_ECMASCRIPT_END(NH_FALSE)
                }
            }
        } 
        else {
            if (O_p != NULL) {
                if (Nh_ECMAScript_newProperty(&O_p->Properties, P, Desc) == NULL) {
                    NH_ECMASCRIPT_END(NH_FALSE)
                }
            }
        }
        NH_ECMASCRIPT_END(NH_TRUE)
    }

    if (Current.configurable == NH_FALSE) {

    }

    // TODO validation

NH_ECMASCRIPT_END(NH_FALSE)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryDefineOwnProperty(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_PropertyDescriptor Desc)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Current = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);
    Nh_ECMAScript_Completion IsExtensibleCompletion = Nh_ECMAScript_isExtensible(This_p);

NH_ECMASCRIPT_END(Nh_ECMAScript_validateAndApplyPropertyDescriptor(This_p, PropertyKey, (NH_ECMASCRIPT_BOOLEAN)IsExtensibleCompletion.Value.Payload.handle_p, Desc, Current))
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryHasProperty(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Descriptor = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);
    if (Descriptor.type != -1) {NH_ECMASCRIPT_END(NH_TRUE)}

    Nh_ECMAScript_Object *Parent_p = This_p->InternalMethods_p->getPrototypeOf_f(This_p);
    if (Parent_p != NULL) {
        NH_ECMASCRIPT_END(Parent_p->InternalMethods_p->hasProperty_f(Parent_p, PropertyKey))
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

Nh_ECMAScript_Any Nh_ECMAScript_ordinaryGet(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Receiver)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_ECMAScript_initAny(NH_ECMASCRIPT_TYPE_UNDEFINED, NULL))
}

static NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinarySetWithOwnDescriptor(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Value, Nh_ECMAScript_Any Receiver,
    Nh_ECMAScript_PropertyDescriptor OwnDescriptor)
{
NH_ECMASCRIPT_BEGIN()

    if (OwnDescriptor.type == -1) {
        Nh_ECMAScript_Object *Parent_p = Object_p->InternalMethods_p->getPrototypeOf_f(Object_p);
        if (Parent_p != NULL) {
            NH_ECMASCRIPT_END(Parent_p->InternalMethods_p->set_f(Parent_p, PropertyKey, Value, Receiver))
        }
        else {
            OwnDescriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
            OwnDescriptor.enumerable = NH_TRUE;
            OwnDescriptor.configurable = NH_TRUE;
            OwnDescriptor.Fields.Data.writable = NH_TRUE;
            OwnDescriptor.Fields.Data.Value = Nh_ECMAScript_wrapUndefined();
        }
    }

    if (Nh_ECMAScript_isDataDescriptor(OwnDescriptor)) 
    {
        if (!OwnDescriptor.Fields.Data.writable) {NH_ECMASCRIPT_END(NH_FALSE)}
        if (Receiver.type != NH_ECMASCRIPT_TYPE_OBJECT) {NH_ECMASCRIPT_END(NH_FALSE)}

        Nh_ECMAScript_PropertyDescriptor ExistingDescriptor = 
            ((Nh_ECMAScript_Object*)Receiver.handle_p)->InternalMethods_p->getOwnProperty_f(Receiver.handle_p, PropertyKey);

        if (ExistingDescriptor.type != -1) 
        {
            if (Nh_ECMAScript_isAccessorDescriptor(ExistingDescriptor)) {NH_ECMASCRIPT_END(NH_FALSE)}
            if (!ExistingDescriptor.Fields.Data.writable) {NH_ECMASCRIPT_END(NH_FALSE)}

            Nh_ECMAScript_PropertyDescriptor ValueDescriptor;
            ValueDescriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
            ValueDescriptor.Fields.Data.Value = Value;

            NH_ECMASCRIPT_END(((Nh_ECMAScript_Object*)Receiver.handle_p)->InternalMethods_p->defineOwnProperty_f(Receiver.handle_p, PropertyKey, ValueDescriptor))
        }
        else {
            NH_ECMASCRIPT_END(Nh_ECMAScript_abstractCreateDataProperty(Receiver.handle_p, PropertyKey, Value))
        }
    }

    Nh_ECMAScript_Any Setter = OwnDescriptor.Fields.Accessor.Set;
    if (Setter.type == NH_ECMASCRIPT_TYPE_UNDEFINED) {NH_ECMASCRIPT_END(NH_FALSE)}

    Nh_List Arguments = Nh_initList(1);
    Nh_appendToList(&Arguments, &Value);

    Nh_ECMAScript_abstractCall(Setter, Receiver, Arguments);

    Nh_freeList(&Arguments, NH_FALSE);

NH_ECMASCRIPT_END(NH_TRUE)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinarySet(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any V, Nh_ECMAScript_Any Receiver)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Descriptor = This_p->InternalMethods_p->getOwnProperty_f(This_p, PropertyKey);

NH_ECMASCRIPT_END(Nh_ECMAScript_ordinarySetWithOwnDescriptor(This_p, PropertyKey, V, Receiver, Descriptor))
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_ordinaryDelete(
    Nh_ECMAScript_Object *This_p, Nh_ECMAScript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(NH_FALSE)
}

Nh_List Nh_ECMAScript_ordinaryOwnPropertyKeys(
    Nh_ECMAScript_Object *This_p)
{
NH_ECMASCRIPT_BEGIN()

NH_ECMASCRIPT_END(Nh_initList(8))
}

// DATA ============================================================================================

const int NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P[] = {0, 1};

Nh_ECMAScript_InternalMethods NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS = {
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
    .call_f              = NULL,
    .construct_f         = NULL,
};

// OTHER ===========================================================================================

// https://tc39.es/ecma262/#sec-ordinaryobjectcreate
Nh_ECMAScript_Object *Nh_ECMAScript_ordinaryObjectCreate(
    Nh_ECMAScript_Object *Proto_p, const int *lookup_p, int lookupLength)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Object *NewObject_p = Nh_ECMAScript_abstractMakeBasicObject(
        lookup_p == NULL ? NH_ECMASCRIPT_ORDINARY_OBJECT_LOOKUP_P : lookup_p, lookup_p == NULL ? 2 : lookupLength
    );

    Nh_ECMAScript_setInternalSlot(&NewObject_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_PROTOTYPE, Proto_p);

NH_ECMASCRIPT_END(NewObject_p)
}

