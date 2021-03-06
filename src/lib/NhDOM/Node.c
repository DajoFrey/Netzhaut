// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Node.h"
#include "NodeList.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include "../NhWeb/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define CHILD_NODES Node_p->Attributes.pp[9]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeNode(
    Nh_Web_Object *Node_p)
{
NH_DOM_BEGIN()

    CHILD_NODES = Nh_Web_createObject("DOM", "NodeList");
    NH_DOM_CHECK_MEM(CHILD_NODES)

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_appendChildToNode(
    Nh_Web_Object *Node_p, Nh_Web_Object *Child_p)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(Nh_DOM_appendToNodeList(CHILD_NODES, Child_p))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

