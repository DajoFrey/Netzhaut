#ifndef NH_DOM_NODE_H
#define NH_DOM_NODE_H

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

    NH_DOM_RESULT Nh_DOM_appendChildToNode(
        Nh_Web_Object *Node_p, Nh_Web_Object *Child_p
    );

/** @} */

#endif
