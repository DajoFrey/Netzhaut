// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "PropertyDescriptor.h"
#include "Properties.h"

#include "../Common/Macro.h"
#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>

// INIT ============================================================================================

Nh_ECMAScript_PropertyDescriptor Nh_ECMAScript_undefinedPropertyDescriptor()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_PropertyDescriptor Descriptor;
    Descriptor.type = -1;

NH_ECMASCRIPT_END(Descriptor)
}

// ABSTRACT OPERATIONS =============================================================================

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isAccessorDescriptor(
    Nh_ECMAScript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Descriptor.type == NH_ECMASCRIPT_PROPERTY_ACCESSOR)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isDataDescriptor (
    Nh_ECMAScript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(Descriptor.type == NH_ECMASCRIPT_PROPERTY_DATA)
}

NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isGenericDescriptor(
    Nh_ECMAScript_PropertyDescriptor Descriptor)
{
NH_ECMASCRIPT_BEGIN()

    if (Descriptor.type == -1) {
        NH_ECMASCRIPT_END(NH_FALSE)
    }

    if (!Nh_ECMAScript_isAccessorDescriptor(Descriptor) && !Nh_ECMAScript_isDataDescriptor(Descriptor)) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

NH_ECMASCRIPT_END(NH_FALSE)
}

