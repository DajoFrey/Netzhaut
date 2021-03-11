// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Attr.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include "../NhWebIDL/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define NAMESPACE  Attr_p->Attributes.pp[0]
#define PREFIX     Attr_p->Attributes.pp[1]
#define LOCAL_NAME Attr_p->Attributes.pp[2]
#define NAME       Attr_p->Attributes.pp[3]
#define VALUE      Attr_p->Attributes.pp[4]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeNode(
    Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()

    CHILD_NODES = Nh_WebIDL_createObject("DOM", "NodeList");
    NH_DOM_CHECK_MEM(CHILD_NODES)

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_appendToNode(
    Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *Child_p)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(Nh_DOM_appendToNodeList(CHILD_NODES, Child_p))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_setNodeDocument(
    Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *NodeDocument_p)
{
NH_DOM_BEGIN()

    NODE_DOCUMENT = NodeDocument_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

