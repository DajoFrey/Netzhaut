#ifndef NH_WEB_API_H
#define NH_WEB_API_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Common/API.h"

#endif

/** @addtogroup NhWebEnums
 *  @{
 */

    typedef enum NH_WEB_TYPE {
        NH_WEB_TYPE_WILDCARD,
        NH_WEB_TYPE_ANY,
        NH_WEB_TYPE_UNDEFINED,
        NH_WEB_TYPE_BOOLEAN,
        NH_WEB_TYPE_BYTE,
        NH_WEB_TYPE_OCTET,
        NH_WEB_TYPE_SHORT,
        NH_WEB_TYPE_UNSIGNED_SHORT,
        NH_WEB_TYPE_LONG,
        NH_WEB_TYPE_UNSIGNED_LONG,
        NH_WEB_TYPE_LONG_LONG,
        NH_WEB_TYPE_UNSIGNED_LONG_LONG,
        NH_WEB_TYPE_FLOAT,
        NH_WEB_TYPE_UNRESTRICTED_FLOAT,
        NH_WEB_TYPE_DOUBLE,
        NH_WEB_TYPE_UNRESTRICTED_DOUBLE,
        NH_WEB_TYPE_BIGINT,
        NH_WEB_TYPE_DOMSTRING,
        NH_WEB_TYPE_BYTESTRING,
        NH_WEB_TYPE_USVSTRING,
        NH_WEB_TYPE_OBJECT,
        NH_WEB_TYPE_SYMBOL,
    } NH_WEB_TYPE;

/** @} */

/** @addtogroup NhWebTypedefs
 *  @{
 */

    typedef struct Nh_Web_Class Nh_Web_Class;

    typedef char NH_WEB_BYTE;
    typedef unsigned char NH_WEB_OCTET;
    typedef short NH_WEB_SHORT;
    typedef unsigned short NH_WEB_UNSIGNED_SHORT;
    typedef long NH_WEB_LONG;
    typedef unsigned long NH_WEB_UNSIGNED_LONG;
    typedef long long NH_WEB_LONG_LONG;
    typedef unsigned long long NH_WEB_UNSIGNED_LONG_LONG;
    typedef float NH_WEB_FLOAT;
    typedef float NH_WEB_UNRESTRICTED_FLOAT;
    typedef double NH_WEB_DOUBLE;
    typedef double NH_WEB_UNRESTRICTED_DOUBLE;
    typedef unsigned long NH_WEB_UNSIGNED_LONG;
    typedef long long NH_WEB_LONG_LONG;
    typedef unsigned long long NH_WEB_UNSIGNED_LONG_LONG;

    typedef Nh_UTF8String Nh_Web_DOMString;
    typedef Nh_String Nh_Web_ByteString;
    typedef Nh_UnicodeString Nh_Web_USVString;
    typedef Nh_BigInt Nh_Web_BigInt;

/** @} */

/** @addtogroup NhWebStructs
 *  @{
 */

    typedef struct Nh_Web_Object {
        Nh_Web_Class *Class_p;
        struct Nh_Web_Object *Parent_p;
        struct Nh_Web_Object *Inherit_p;
        Nh_List Attributes;
    } Nh_Web_Object;

    typedef struct Nh_Web_CompositeObject {
        Nh_List PartialObjects;
    } Nh_Web_CompositeObject;

    typedef struct Nh_Web_Any {
        NH_WEB_TYPE type;
        void *value_p;
    } Nh_Web_Any;

/** @} */

#endif
