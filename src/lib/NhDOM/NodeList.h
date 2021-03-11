#ifndef NH_DOM_NODE_LIST_H
#define NH_DOM_NODE_LIST_H

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

    NH_DOM_RESULT Nh_DOM_appendToNodeList(
        Nh_WebIDL_Object *NodeList_p, Nh_WebIDL_Object *Node_p
    );

    NH_DOM_RESULT Nh_DOM_insertIntoNodeList(
        Nh_WebIDL_Object *NodeList_p, Nh_WebIDL_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index
    );

    Nh_WebIDL_Object *Nh_DOM_getFromNodeList(
        Nh_WebIDL_Object *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index
    );
    
    NH_WEBIDL_UNSIGNED_LONG Nh_DOM_getNodeListLength(
        Nh_WebIDL_Object *NodeList_p
    );

/** @} */

#endif
