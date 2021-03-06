#ifndef NH_WEB_TYPE_H
#define NH_WEB_TYPE_H

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

/** @addtogroup NhWebFunctions Functions
 *  @{
 */

    NH_WEB_TYPE Nh_Web_getType(
        Nh_Web_ParseNode *Type_p
    );

    size_t Nh_Web_getAllocationSize(
        NH_WEB_TYPE type
    );

/** @} */

#endif
