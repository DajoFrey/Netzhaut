#ifndef NH_WEBIDL_TYPE_H
#define NH_WEBIDL_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/Result.h"

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

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Any {
        NH_WEBIDL_TYPE type;
        void *value_p;
    } Nh_WebIDL_Any;

/** @} */

/** @addtogroup WebIDLStructs Structs
 *  \ingroup WebIDL
 *  @{
 */

/** @} */

#endif
