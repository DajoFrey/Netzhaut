#ifndef NH_WEBIDL_PARSER_H
#define NH_WEBIDL_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Lexer.h"
#include "../Common/API.h"

#include "../../NhCore/List.h"
#include "../../NhCore/Array.h"

#endif

/** @addtogroup WebIDLEnums Enums
 *  \ingroup WebIDL
 *  @{
 */

    typedef enum NH_WEBIDL_TYPE {
        NH_WEBIDL_TYPE_BYTE, 
        NH_WEBIDL_TYPE_OCTET, 
        NH_WEBIDL_TYPE_SHORT, 
        NH_WEBIDL_TYPE_UNSIGNED_SHORT, 
        NH_WEBIDL_TYPE_LONG, 
        NH_WEBIDL_TYPE_UNSIGNED_LONG, 
        NH_WEBIDL_TYPE_LONG_LONG,
        NH_WEBIDL_TYPE_UNSIGNED_LONG_LONG,
        NH_WEBIDL_TYPE_FLOAT, 
        NH_WEBIDL_TYPE_UNRESTRICTED_FLOAT, 
        NH_WEBIDL_TYPE_DOUBLE,
        NH_WEBIDL_TYPE_UNRESTRICTED_DOUBLE,
        NH_WEBIDL_TYPE_BIGINT,
        NH_WEBIDL_TYPE_BOOLEAN,
        NH_WEBIDL_TYPE_DOMSTRING,
        NH_WEBIDL_TYPE_BYTESTRING,
        NH_WEBIDL_TYPE_USVSTRING,
        NH_WEBIDL_TYPE_INT8_ARRAY, 
        NH_WEBIDL_TYPE_INT16_ARRAY, 
        NH_WEBIDL_TYPE_INT32_ARRAY, 
        NH_WEBIDL_TYPE_UINT8_ARRAY, 
        NH_WEBIDL_TYPE_UINT16_ARRAY, 
        NH_WEBIDL_TYPE_UINT32_ARRAY, 
        NH_WEBIDL_TYPE_UINT8_CLAMPED_ARRAY, 
        NH_WEBIDL_TYPE_FLOAT32_ARRAY,
        NH_WEBIDL_TYPE_FLOAT64_ARRAY,
        NH_WEBIDL_TYPE_ARRAYBUFFER,
        NH_WEBIDL_TYPE_DATAVIEW,
        NH_WEBIDL_TYPE_OBJECT,
    } NH_WEBIDL_TYPE;

    typedef enum NH_WEBIDL_EXTENDED_ATTRIBUTE {
        NH_WEBIDL_EXTENDED_ATTRIBUTE_ALLOW_SHARED,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_CLAMP,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_CROSS_ORIGIN_ISOLATED,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_DEFAULT,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_ENFORCE_RANGE,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_EXPOSED,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_GLOBAL,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_NEW_OBJECT,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_PUT_FORWARDS,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_REPLACEABLE,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_SAME_OBJECT,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_SECURE_CONTEXT,
        NH_WEBIDL_EXTENDED_ATTRIBUTE_UNSCOPABLE,
    } NH_WEBIDL_EXTENDED_ATTRIBUTE;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    typedef struct Nh_WebIDL_ExtendedAttribute {
        NH_WEBIDL_EXTENDED_ATTRIBUTE type;
        Nh_List Values;
    } Nh_WebIDL_ExtendedAttribute;

    typedef struct Nh_WebIDL_Type {
        NH_WEBIDL_TYPE type;        	
        NH_BOOL nullable;
        void *type_p;
    } Nh_WebIDL_Type;

    typedef struct Nh_WebIDL_Attribute {
        Nh_WebIDL_Type Type;
        NH_BOOL _static;
        NH_BOOL readonly;
        NH_BOOL stringifier;
        char *name_p;
    } Nh_WebIDL_Attribute;

    typedef struct Nh_WebIDL_Operation {
        Nh_Array ExtendedAttributes;
        char *name_p;
    } Nh_WebIDL_Operation;

    typedef struct Nh_WebIDL_Interface {
        char *name_p;
        void *inherit_p;
        Nh_Array Attributes;
        Nh_Array Operations;
    } Nh_WebIDL_Interface;

    typedef struct Nh_WebIDL_Definition {
        NH_WEBIDL_DEFINITION type;
        void *data_p;
    } Nh_WebIDL_Definition;

    typedef struct Nh_WebIDL_Fragment {
        int definitionCount;
        Nh_WebIDL_Definition *Definitions_p;
    } Nh_WebIDL_Fragment;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

    Nh_WebIDL_Fragment Nh_WebIDL_parseFragmentToken(
        Nh_WebIDL_FragmentToken Fragment
    );

    void Nh_WebIDL_unparseFragment(
        Nh_WebIDL_Fragment Fragment
    );

    void Nh_WebIDL_stringifyFragment(
        Nh_WebIDL_Fragment Fragment, char *str_p, int *length_p
    );

/** @} */

#endif
