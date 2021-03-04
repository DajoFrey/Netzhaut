// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "NodeList.h"

#include "../Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define CHUNK_SIZE  16
#define NODES       ((void**)NodeList_p->Attributes.pp[0])
#define CHUNK_COUNT *((NH_WEBIDL_UNSIGNED_LONG*)NodeList_p->Attributes.pp[1])
#define LENGTH      *((NH_WEBIDL_UNSIGNED_LONG*)NodeList_p->Attributes.pp[2])

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_initializeNodeList(
    Nh_WebIDL_Object *NodeList_p, va_list Arguments)
{
NH_DOM_BEGIN()

    CHUNK_COUNT = 0;
    LENGTH = 0;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_appendToNodeList(
    Nh_WebIDL_Object *NodeList_p, Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()

    if (LENGTH >= CHUNK_SIZE * CHUNK_COUNT) 
    {
        if (NODES == NULL) {
            NODES = Nh_allocate(sizeof(Nh_WebIDL_Object*) * CHUNK_SIZE);
            if (NODES == NULL) {return NH_DOM_ERROR_BAD_STATE;}
        }
        else {
            NODES = realloc(NODES, sizeof(Nh_WebIDL_Object*) * CHUNK_SIZE * (CHUNK_COUNT + 1));
            if (NODES == NULL) {return NH_DOM_ERROR_BAD_STATE;}
        }
        CHUNK_COUNT += 1;
    }

    NODES[LENGTH] = Node_p;
    LENGTH += 1;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================


