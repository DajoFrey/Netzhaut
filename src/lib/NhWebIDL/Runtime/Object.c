// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Object.h"
#include "Runtime.h"
#include "Type.h"
#include "Definitions.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_CUSTOM_CHECK

#include "../../NhLoader/Loader.h"
#include "../../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OBJECT ==========================================================================================

static NH_BOOL Nh_WebIDL_isAttribute(
    Nh_WebIDL_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER     :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE   :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE   :
            NH_WEBIDL_END(NH_TRUE)
    }

NH_WEBIDL_END(NH_FALSE)
}

typedef int (*init_f)(Nh_WebIDL_Object *Object_p);

Nh_WebIDL_Object *Nh_WebIDL_createObjectFromInterface(
    Nh_WebIDL_Interface *Interface_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Object *Object_p = Nh_allocate(sizeof(Nh_WebIDL_Object));
    NH_WEBIDL_CHECK_MEM(NULL, Object_p)
 
    Object_p->Interface_p = Interface_p;
    Object_p->Child_p     = NULL;
    Object_p->Parent_p    = NULL;
    Object_p->internal_p  = NULL;
    Object_p->Parts       = Nh_initList(1);

    int attributes = 0;

    for (int i = 0; i < Interface_p->Members.length; ++i) {
        Nh_WebIDL_InterfaceMember *InterfaceMember_p = &((Nh_WebIDL_InterfaceMember*)Interface_p->Members.bytes_p)[i];
        if (Nh_WebIDL_isAttribute(InterfaceMember_p->Node_p)) {attributes++;}
    }

    Object_p->Attributes = Nh_initList(attributes);

    for (int i = 0; i < attributes; ++i) {Nh_appendToList(&Object_p->Attributes, NULL);}

    NH_BYTE funcName_p[255] = {'\0'};
    sprintf(funcName_p, "Nh_%s_initialize%s", Object_p->Interface_p->Specification_p->name_p, Object_p->Interface_p->name_p);

    NH_BYTE libName_p[255] = {'\0'};
    sprintf(libName_p, "Nh%s", Object_p->Interface_p->Specification_p->name_p);

    init_f initNewObject_f = NH_LOADER.loadFunction_f(libName_p, 0, funcName_p);
    if (initNewObject_f) {initNewObject_f(Object_p);} 

    // TODO cross specification inheritance
    if (Interface_p->Child_p != NULL && Interface_p->Child_p->Children.size > 0) 
    {
        Object_p->Child_p = Nh_WebIDL_createObject(Interface_p->Specification_p->name_p, ((Nh_WebIDL_ParseNode*)Interface_p->Child_p->Children.pp[1])->Token_p->String.bytes_p);
        NH_WEBIDL_CHECK_MEM(NULL, Object_p->Child_p)
        Object_p->Child_p->Parent_p = Object_p;
    }

NH_WEBIDL_END(Object_p)
}

Nh_WebIDL_Object *Nh_WebIDL_createObject(
    NH_BYTE *specification_p, NH_BYTE *interface_p)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL(NULL, specification_p)
    NH_WEBIDL_CHECK_NULL(NULL, interface_p)

    Nh_WebIDL_Interface *Interface_p = Nh_WebIDL_getInterface(specification_p, interface_p);
    NH_WEBIDL_CHECK_NULL(NULL, Interface_p)

    if (Interface_p->partial) {NH_WEBIDL_END(NULL)}

NH_WEBIDL_END(Nh_WebIDL_createObjectFromInterface(Interface_p))
}

// COMPOSITE OBJECT ================================================================================

Nh_WebIDL_Object *Nh_WebIDL_createCompositeObject(
    NH_BYTE *specification_p, NH_BYTE *interface_p, ...)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL(NULL, specification_p)
    NH_WEBIDL_CHECK_NULL(NULL, interface_p)

    Nh_WebIDL_Interface *Interface_p = Nh_WebIDL_getInterface(specification_p, interface_p);
    NH_WEBIDL_CHECK_NULL(NULL, Interface_p)

    if (!Interface_p->partial) {NH_WEBIDL_END(NULL)}

    Nh_List Interfaces = Nh_WebIDL_getCompositeInterfaces(Interface_p);
    if (Interfaces.size <= 1) {NH_WEBIDL_END(NULL)}

    Interface_p = NULL;
    for (int i = 0; i < Interfaces.size; ++i) {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {break;}
        Interface_p = NULL;
    }
    NH_WEBIDL_CHECK_NULL(NULL, Interface_p)

    Nh_WebIDL_Object *Object_p = Nh_WebIDL_createObjectFromInterface(Interface_p);
    NH_WEBIDL_CHECK_NULL(NULL, Object_p)

    for (int i = 0; i < Interfaces.size; ++i) 
    {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {continue;}

        Nh_WebIDL_Object *Part_p = Nh_WebIDL_createObjectFromInterface(Interface_p);
        NH_WEBIDL_CHECK_NULL(NULL, Part_p)
    
        Nh_appendToList(&Object_p->Parts, Part_p);
    }

    Nh_freeList(&Interfaces, NH_FALSE);

NH_WEBIDL_END(Object_p)
}

Nh_WebIDL_Object *Nh_WebIDL_getPart(
    Nh_WebIDL_Object *Object_p, NH_BYTE *specification_p, ...) 
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL(NULL, Object_p)
    NH_WEBIDL_CHECK_NULL(NULL, specification_p)

    Nh_WebIDL_Object *Part_p = NULL;

    for (int i = 0; i < Object_p->Parts.size; ++i) {
        Part_p = Object_p->Parts.pp[i];
        if (!strcmp(Part_p->Interface_p->Specification_p->name_p, specification_p)) {
            break;
        }
        Object_p = NULL;
    }

NH_WEBIDL_END(Object_p)
}

Nh_WebIDL_Object *Nh_WebIDL_getObject(
    Nh_WebIDL_Object *Object_p, NH_BYTE *specification_p, NH_BYTE *interface_p) 
{
NH_WEBIDL_BEGIN()

    while (Object_p != NULL) {
        if (!strcmp(Object_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Object_p->Interface_p->name_p, interface_p)) {
            NH_WEBIDL_END(Object_p) 
        }
        Object_p = Object_p->Child_p;
    }

NH_WEBIDL_END(NULL)
}

