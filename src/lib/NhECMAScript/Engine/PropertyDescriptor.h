#ifndef NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H
#define NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Type.h"

#endif

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_DataPropertyDescriptor {
        Nh_ECMAScript_Any Value;
        NH_ECMASCRIPT_BOOLEAN writable;
    } Nh_ECMAScript_DataPropertyDescriptor;

    typedef struct Nh_ECMAScript_AccessorPropertyDescriptor {
        Nh_ECMAScript_Any Get;
        Nh_ECMAScript_Any Set;
    } Nh_ECMAScript_AccessorPropertyDescriptor;

    typedef union Nh_ECMAScript_PropertyDescriptorFields {
        Nh_ECMAScript_DataPropertyDescriptor Data;
        Nh_ECMAScript_AccessorPropertyDescriptor Accessor;
    } Nh_ECMAScript_PropertyDescriptorFields;

    typedef struct Nh_ECMAScript_PropertyDescriptor {
        int type;
        Nh_ECMAScript_PropertyDescriptorFields Fields;
        NH_ECMASCRIPT_BOOLEAN enumerable;
        NH_ECMASCRIPT_BOOLEAN configurable;
    } Nh_ECMAScript_PropertyDescriptor;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_PropertyDescriptor Nh_ECMAScript_undefinedPropertyDescriptor(
    );

//    Nh_ECMAScript_PropertyDescriptorWrapper Nh_ECMAScript_wrapPropertyDescriptor(
//        Nh_ECMAScript_PropertyDescriptor Descriptor
//    );
//    
//    Nh_ECMAScript_PropertyDescriptorWrapper Nh_ECMAScript_undefinedPropertyDescriptorWrapper(
//    );

    // https://tc39.es/ecma262/#sec-isaccessordescriptor
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isAccessorDescriptor(
        Nh_ECMAScript_PropertyDescriptor Descriptor 
    );

    // https://tc39.es/ecma262/#sec-isdatadescriptor
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isDataDescriptor (
        Nh_ECMAScript_PropertyDescriptor Descriptor
    );

    // https://tc39.es/ecma262/#sec-isgenericdescriptor
    NH_ECMASCRIPT_BOOLEAN Nh_ECMAScript_isGenericDescriptor (
        Nh_ECMAScript_PropertyDescriptor Descriptor
    );

//    Nh_ECMAScript_fromPropertyDescriptor(
//        Desc
//    );
//
//    Nh_ECMAScript_toPropertyDescriptor(
//        Obj
//    );
//
//    Nh_ECMAScript_completePropertyDescriptor(
//        Desc
//    );

/** @} */

#endif
