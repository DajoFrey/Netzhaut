#ifndef NH_WEBIDL_TYPE_H
#define NH_WEBIDL_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/API.h"

#include "Object.h"
#include "Parser.h"

#endif

/** @addtogroup NhWebIDLFunctions Functions
 *  @{
 */

    NH_WEBIDL_TYPE Nh_WebIDL_getType(
        Nh_WebIDL_ParseNode *Type_p
    );

    size_t Nh_WebIDL_getAllocationSize(
        NH_WEBIDL_TYPE type
    );

/** @} */

#endif
