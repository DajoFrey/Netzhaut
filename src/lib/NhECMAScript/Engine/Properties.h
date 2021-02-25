#ifndef NH_ECMASCRIPT_PROPERTIES_H
#define NH_ECMASCRIPT_PROPERTIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "PropertyDescriptor.h"
#include "Type.h"

#include "../../NhCore/List.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_PROPERTY {
        NH_ECMASCRIPT_PROPERTY_DATA,
        NH_ECMASCRIPT_PROPERTY_ACCESSOR,
    } NH_ECMASCRIPT_PROPERTY;

/** @} */

/** @addtogroup NhECMAScriptTypedefs
 *  @{
 */

    #define Nh_ECMAScript_Properties Nh_List

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    // https://www.262.ecma-international.org/11.0/index.html#table-2
    typedef struct Nh_ECMAScript_DataProperty {
        Nh_ECMAScript_Any Value;
        NH_ECMASCRIPT_BOOLEAN writable;
    } Nh_ECMAScript_DataProperty;

    // https://www.262.ecma-international.org/11.0/index.html#table-3
    typedef struct Nh_ECMAScript_AccessorProperty {
        Nh_ECMAScript_Any Get;
        Nh_ECMAScript_Any Set;
    } Nh_ECMAScript_AccessorProperty;

    typedef union Nh_ECMAScript_PropertyFields {
        Nh_ECMAScript_DataProperty Data;
        Nh_ECMAScript_AccessorProperty Accessor;
    } Nh_ECMAScript_PropertyFields;

    typedef struct Nh_ECMAScript_Property {
        NH_ECMASCRIPT_PROPERTY type;
        Nh_ECMAScript_Any Key;
        Nh_ECMAScript_PropertyFields Fields;
        NH_ECMASCRIPT_BOOLEAN enumerable;
        NH_ECMASCRIPT_BOOLEAN configurable;
    } Nh_ECMAScript_Property;

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_ECMAScript_Properties Nh_ECMAScript_initProperties(
        int chunkSize 
    );

    NH_BOOL Nh_ECMAScript_equalPropertyKeys(
        Nh_ECMAScript_Any Key1, Nh_ECMAScript_Any Key2
    );

    Nh_ECMAScript_Property *Nh_ECMAScript_newProperty(
        Nh_ECMAScript_Properties *Properties_p, Nh_ECMAScript_Any Key, Nh_ECMAScript_PropertyDescriptor Descriptor 
    );

/** @} */

#endif
