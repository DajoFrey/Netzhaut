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

#include "../NhWebIDL/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

#define DOCUMENT ((Nh_DOM_Node*)Node_p->internal_p)->Document_p
#define REGISTERED_OBSERVERS ((Nh_DOM_Node*)Node_p->internal_p)->RegisteredObservers

#define NODE_TYPE Node_p->Attributes.pp[0]
#define NODE_NAME Node_p->Attributes.pp[1] 
#define CHILD_NODES Node_p->Attributes.pp[7]

typedef struct Nh_DOM_Node {
    Nh_WebIDL_Object *Document_p;
    Nh_List RegisteredObservers;
} Nh_DOM_Node;

// INITIALIZE ======================================================================================

static NH_DOM_RESULT Nh_DOM_setNodeType(
    Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()

    NODE_TYPE = (void*)0;

    Nh_WebIDL_Object *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL)
    { 
        if (strcmp(Parent_p->Interface_p->Specification_p->name_p, "DOM")) {
            Parent_p = Parent_p->Parent_p;
            continue;
        }
    
        if (!strcmp(Parent_p->Interface_p->name_p, "Element")) {
            NODE_TYPE = (void*)1;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Attr")) {
            NODE_TYPE = (void*)2;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Text")) {
            NODE_TYPE = (void*)3;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "CDATASection")) {
            NODE_TYPE = (void*)4;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "ProcessingInstruction")) {
            NODE_TYPE = (void*)7;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Comment")) {
            NODE_TYPE = (void*)8;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Document")) {
            NODE_TYPE = (void*)9;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "DocumentType")) {
            NODE_TYPE = (void*)10;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "DocumentFragment")) {
            NODE_TYPE = (void*)11;
        }

        if (NODE_TYPE) {NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)}

        Parent_p = Parent_p->Parent_p;
    }

NH_DOM_DIAGNOSTIC_END(NH_DOM_ERROR_BAD_STATE)
}

NH_DOM_RESULT Nh_DOM_initializeNode(
    Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()

    Node_p->internal_p = Nh_allocate(sizeof(Nh_DOM_Node));
    NH_DOM_CHECK_MEM(Node_p->internal_p)

    DOCUMENT = NULL;
    REGISTERED_OBSERVERS = Nh_initList(8);

    NH_DOM_CHECK(Nh_DOM_setNodeType(Node_p))

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

NH_DOM_RESULT Nh_DOM_insertIntoNode(
    Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(Nh_DOM_insertIntoNodeList(CHILD_NODES, Child_p, index))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_setNodeDocument(
    Nh_WebIDL_Object *Node_p, Nh_WebIDL_Object *NodeDocument_p)
{
NH_DOM_BEGIN()

    DOCUMENT = NodeDocument_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

Nh_WebIDL_Object *Nh_DOM_getNodeDocument(
    Nh_WebIDL_Object *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(DOCUMENT)
}

// API =============================================================================================

