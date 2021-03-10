#ifndef NH_WEBIDL_API_H
#define NH_WEBIDL_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Common/API.h"

#endif

/** @addtogroup NhWebIDLEnums
 *  @{
 */

    typedef enum NH_WEBIDL_TYPE {
        NH_WEBIDL_TYPE_WILDCARD,
        NH_WEBIDL_TYPE_ANY,
        NH_WEBIDL_TYPE_UNDEFINED,
        NH_WEBIDL_TYPE_BOOLEAN,
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
        NH_WEBIDL_TYPE_DOMSTRING,
        NH_WEBIDL_TYPE_BYTESTRING,
        NH_WEBIDL_TYPE_USVSTRING,
        NH_WEBIDL_TYPE_OBJECT,
        NH_WEBIDL_TYPE_SYMBOL,
    } NH_WEBIDL_TYPE;

/** @} */

/** @addtogroup NhWebIDLTypedefs
 *  @{
 */

    typedef struct Nh_WebIDL_ParseNode Nh_WebIDL_ParseNode;

    typedef char NH_WEBIDL_BYTE;
    typedef unsigned char NH_WEBIDL_OCTET;
    typedef short NH_WEBIDL_SHORT;
    typedef unsigned short NH_WEBIDL_UNSIGNED_SHORT;
    typedef long NH_WEBIDL_LONG;
    typedef unsigned long NH_WEBIDL_UNSIGNED_LONG;
    typedef long long NH_WEBIDL_LONG_LONG;
    typedef unsigned long long NH_WEBIDL_UNSIGNED_LONG_LONG;
    typedef float NH_WEBIDL_FLOAT;
    typedef float NH_WEBIDL_UNRESTRICTED_FLOAT;
    typedef double NH_WEBIDL_DOUBLE;
    typedef double NH_WEBIDL_UNRESTRICTED_DOUBLE;
    typedef unsigned long NH_WEBIDL_UNSIGNED_LONG;
    typedef long long NH_WEBIDL_LONG_LONG;
    typedef unsigned long long NH_WEBIDL_UNSIGNED_LONG_LONG;

    typedef Nh_UTF8String Nh_WebIDL_DOMString;
    typedef Nh_String Nh_WebIDL_ByteString;
    typedef Nh_UnicodeString Nh_WebIDL_USVString;
    typedef Nh_BigInt Nh_WebIDL_BigInt;

/** @} */

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Specification {
        NH_BYTE name_p[255];
        Nh_Array Fragments;
    } Nh_WebIDL_Specification;

    typedef struct Nh_WebIDL_Interface {
        NH_BOOL partial;
        NH_BYTE *name_p;
        Nh_Array Members;
        Nh_WebIDL_Specification *Specification_p;
        Nh_WebIDL_ParseNode *Child_p;
    } Nh_WebIDL_Interface;

    typedef struct Nh_WebIDL_Object {
        Nh_WebIDL_Interface *Interface_p;
        struct Nh_WebIDL_Object *Parent_p;
        struct Nh_WebIDL_Object *Child_p;
        Nh_List Attributes;
        Nh_List Parts;
        void *internal_p;
    } Nh_WebIDL_Object;

    typedef struct Nh_WebIDL_Any {
        NH_WEBIDL_TYPE type;
        void *value_p;
    } Nh_WebIDL_Any;

/** @} */

#endif
