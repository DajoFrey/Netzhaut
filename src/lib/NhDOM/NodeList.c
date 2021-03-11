// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "NodeList.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define NODES NodeList_p->internal_p
#define LENGTH NodeList_p->Attributes.pp[0]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeNodeList(
    Nh_WebIDL_Object *NodeList_p)
{
NH_DOM_BEGIN()

    NODES = Nh_allocate(sizeof(Nh_List));
    NH_DOM_CHECK_MEM(NODES)

    *((Nh_List*)NODES) = Nh_initList(16);
    LENGTH = &((Nh_List*)NODES)->size;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_appendToNodeList(
    Nh_WebIDL_Object *NodeList_p, Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()

    Nh_appendToList(NODES, Node_p);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_insertIntoNodeList(
    Nh_WebIDL_Object *NodeList_p, Nh_WebIDL_Object *Node_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()

    Nh_insertIntoList(NODES, Node_p, index);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

Nh_WebIDL_Object *Nh_DOM_getFromNodeList(
    Nh_WebIDL_Object *NodeList_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()
NH_DOM_END(((Nh_List*)NODES)->pp[index])
}

NH_WEBIDL_UNSIGNED_LONG Nh_DOM_getNodeListLength(
    Nh_WebIDL_Object *NodeList_p)
{
NH_DOM_BEGIN()
NH_DOM_END(*((NH_WEBIDL_UNSIGNED_LONG*)LENGTH))
}

