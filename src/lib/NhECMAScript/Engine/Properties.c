// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Properties.h"

#include "../Common/Macro.h" 
#include NH_ECMASCRIPT_CUSTOM_CHECK
#include NH_ECMASCRIPT_FLOW

#include "../../NhCore/Memory.h"

#include <string.h>

// PROPERTIES ======================================================================================

Nh_ECMAScript_Properties Nh_ECMAScript_initProperties(
    int chunkSize)
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_Properties Properties = Nh_initList(chunkSize);

NH_ECMASCRIPT_END(Properties)
}

NH_BOOL Nh_ECMAScript_equalPropertyKeys(
    Nh_ECMAScript_Any Key1, Nh_ECMAScript_Any Key2)
{
NH_ECMASCRIPT_BEGIN()

    if (Key1.type == NH_ECMASCRIPT_TYPE_STRING && Key2.type == NH_ECMASCRIPT_TYPE_STRING) {
        if (Key1.handle_p != NULL && Key2.handle_p != NULL && !strcmp(Key1.handle_p, Key2.handle_p)) {
            NH_ECMASCRIPT_END(NH_TRUE)
        }
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

Nh_ECMAScript_Property *Nh_ECMAScript_newProperty(
    Nh_ECMAScript_Properties *Properties_p, Nh_ECMAScript_Any Key, Nh_ECMAScript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()

    if (Descriptor.type == -1) {NH_ECMASCRIPT_END(NULL)}

    Nh_ECMAScript_Property *Property_p = Nh_allocate(sizeof(Nh_ECMAScript_Property));
    NH_ECMASCRIPT_CHECK_MEM(NULL, Property_p)

    Property_p->type = Descriptor.type;
    Property_p->Key  = Key;
    Property_p->enumerable   = Descriptor.enumerable;
    Property_p->configurable = Descriptor.configurable;

    if (Property_p->type == NH_ECMASCRIPT_PROPERTY_DATA) {
        Property_p->Fields.Data.Value    = Descriptor.Fields.Data.Value;
        Property_p->Fields.Data.writable = Descriptor.Fields.Data.writable;
    }
    else {
        Property_p->Fields.Accessor.Get = Descriptor.Fields.Accessor.Get;
        Property_p->Fields.Accessor.Set = Descriptor.Fields.Accessor.Set;
    }

NH_ECMASCRIPT_END(Property_p)
}

