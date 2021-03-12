#ifndef NH_DOM_TEXT_H
#define NH_DOM_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhDOMFunctions
 *  @{
 */

    Nh_WebIDL_Object *Nh_DOM_createText(
        Nh_WebIDL_DOMString Data, Nh_WebIDL_Object *NodeDocument_p
    );

    NH_DOM_RESULT Nh_DOM_appendToText(
        Nh_WebIDL_Object *Text_p, Nh_WebIDL_DOMString DOMString
    );

/** @} */

#endif
