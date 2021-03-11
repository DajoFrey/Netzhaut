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

    NH_DOM_RESULT Nh_DOM_appendToNode(
        Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *Child_p
    );

    NH_DOM_RESULT Nh_DOM_insertIntoNode(
        Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    NH_DOM_RESULT Nh_DOM_setNodeDocument(
        Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *NodeDocument_p
    );

    Nh_WebIDL_Object *Nh_DOM_getNodeDocument(
        Nh_WebIDL_Object *Node_p
    );

/** @} */

#endif
