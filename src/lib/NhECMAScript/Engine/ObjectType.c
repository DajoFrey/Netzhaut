// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "ObjectType.h"
#include "TestAndCompare.h"
#include "OrdinaryObject.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// ABSTRACT ========================================================================================

Nh_ECMAScript_Object *Nh_ECMAScript_abstractMakeBasicObject(
    const int *lookup_p, int lookupLength)
{
NH_ECMASCRIPT_BEGIN()

#include NH_ECMASCRIPT_CUSTOM_CHECK

    Nh_ECMAScript_Object *Obj_p = Nh_allocate(sizeof(Nh_ECMAScript_Object));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Obj_p)

    Obj_p->Properties = Nh_ECMAScript_initProperties(8);
    Obj_p->InternalSlots = Nh_ECMAScript_initInternalSlots(lookup_p, lookupLength);
    Obj_p->InternalMethods_p = &NH_ECMASCRIPT_ORDINARY_OBJECT_INTERNAL_METHODS; 

    Nh_ECMAScript_setInternalSlot(&Obj_p->InternalSlots, NH_ECMASCRIPT_INTERNAL_SLOT_EXTENSIBLE, (void*) NH_TRUE);

#include NH_ECMASCRIPT_DEFAULT_CHECK

NH_ECMASCRIPT_END(Obj_p)
}

static NH_ECMASCRIPT_RESULT Nh_ECMAScript_allocatePropertyKeyAndValue(
    Nh_ECMAScript_Any *PropertyKey_p, Nh_ECMAScript_PropertyDescriptor *PropertyDescriptor_p)
{
NH_ECMASCRIPT_BEGIN()

    switch (PropertyKey_p->type)
    {
        case NH_ECMASCRIPT_TYPE_STRING : ;

            Nh_ECMAScript_String *Name_p = PropertyKey_p->handle_p;
            Nh_ECMAScript_String *AllocatedName_p = Nh_ECMAScript_allocateString(Name_p->length);
            NH_ECMASCRIPT_CHECK_MEM(AllocatedName_p);
            NH_ECMASCRIPT_CHECK(Nh_ECMAScript_appendFormatToString(AllocatedName_p, Name_p->bytes_p, Name_p->length))
            *PropertyKey_p = Nh_ECMAScript_wrapString(AllocatedName_p);
            break;

        default :
            NH_ECMASCRIPT_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
    }

    if (PropertyDescriptor_p->type == NH_ECMASCRIPT_PROPERTY_DATA)
    {
        switch (PropertyDescriptor_p->Fields.Data.Value.type)
        {
            case NH_ECMASCRIPT_TYPE_NUMBER : ;

                NH_ECMASCRIPT_NUMBER *number_p = Nh_ECMAScript_allocateNumber(
                    *((NH_ECMASCRIPT_NUMBER*)PropertyDescriptor_p->Fields.Data.Value.handle_p)
                );
                NH_ECMASCRIPT_CHECK_MEM(number_p)
                PropertyDescriptor_p->Fields.Data.Value.handle_p = number_p;    
                break;

            case NH_ECMASCRIPT_TYPE_OBJECT :
                break;

            default :
                NH_ECMASCRIPT_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
        }
    }
    else {
        NH_ECMASCRIPT_END(NH_ECMASCRIPT_ERROR_BAD_STATE)
    }

NH_ECMASCRIPT_END(NH_ECMASCRIPT_SUCCESS)
}

Nh_ECMAScript_Completion Nh_ECMAScript_abstractDefinePropertyOrThrow(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_PropertyDescriptor PropertyDescriptor)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_allocatePropertyKeyAndValue(&PropertyKey, &PropertyDescriptor);

    NH_ECMASCRIPT_BOOLEAN success = Object_p->InternalMethods_p->defineOwnProperty_f(
        Object_p, PropertyKey, PropertyDescriptor
    );

    if (success == NH_FALSE) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}

NH_ECMASCRIPT_END(Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapBoolean(success)))
}

NH_BOOL Nh_ECMAScript_abstractHasProperty(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Object_p->InternalMethods_p->hasProperty_f(Object_p, PropertyKey))
}

NH_BOOL Nh_ECMAScript_abstractHasOwnProperty(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Descriptor = Object_p->InternalMethods_p->getOwnProperty_f(Object_p, PropertyKey);
    if (Descriptor.type == -1) {NH_ECMASCRIPT_END(NH_FALSE)}

NH_ECMASCRIPT_END(NH_TRUE)
}

NH_BOOL Nh_ECMAScript_abstractIsExtensible(
    Nh_ECMAScript_Object *Object_p)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Object_p->InternalMethods_p->isExtensible_f(Object_p))
}

Nh_ECMAScript_Completion Nh_ECMAScript_abstractSet(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any Key, Nh_ECMAScript_Any Value, NH_BOOL _throw)
{
NH_ECMASCRIPT_BEGIN()

    NH_BOOL success = Object_p->InternalMethods_p->set_f(Object_p, Key, Value, Nh_ECMAScript_wrapObject(Object_p));
    if (!success && _throw) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}

NH_ECMASCRIPT_END(Nh_ECMAScript_normalCompletion(Nh_ECMAScript_wrapBoolean(success)))
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_abstractCreateDataProperty(
    Nh_ECMAScript_Object *Object_p, Nh_ECMAScript_Any PropertyKey, Nh_ECMAScript_Any Value)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = NH_ECMASCRIPT_PROPERTY_DATA;
    Descriptor.enumerable = NH_TRUE;
    Descriptor.configurable = NH_TRUE;
    Descriptor.Fields.Data.Value = Value;
    Descriptor.Fields.Data.writable = NH_TRUE; 

NH_ECMASCRIPT_END(Object_p->InternalMethods_p->defineOwnProperty_f(Object_p, PropertyKey, Descriptor))
}

Nh_ECMAScript_Completion Nh_ECMAScript_abstractCall(
    Nh_ECMAScript_Any F, Nh_ECMAScript_Any Value, Nh_List Arguments)
{
NH_ECMASCRIPT_BEGIN()

    if (!Nh_ECMAScript_isCallable(F)) {NH_ECMASCRIPT_END(Nh_ECMAScript_throwTypeError())}

NH_ECMASCRIPT_END(((Nh_ECMAScript_Object*)Value.handle_p)->InternalMethods_p->call_f(Value.handle_p, Value, Arguments))
}

