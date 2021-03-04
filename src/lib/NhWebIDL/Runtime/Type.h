#ifndef NH_WEBIDL_TYPE_H
#define NH_WEBIDL_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Object.h"

#include "../../NhCore/Unicode.h"

#endif

/** @addtogroup NhWebIDLEnums
 *  @{
 */

    typedef enum NH_WEBIDL_TYPE {
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

/** @} */

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_BigInteger {
        int TODO;
    } Nh_WebIDL_BigInteger;

    typedef struct Nh_WebIDL_Any {
        NH_WEBIDL_TYPE type;
        void *value_p;
    } Nh_WebIDL_Any;

/** @} */

/** @addtogroup NhWebIDLFunctions Functions
 *  @{
 */

/** @} */

#endif
