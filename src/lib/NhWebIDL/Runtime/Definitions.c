// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Definitions.h"
#include "Builtin.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include "../../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CLASSES =========================================================================================

static const NH_BYTE *Nh_WebIDL_getInterfaceMemberName(
    Nh_WebIDL_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE *name_p = NULL;

    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_CONST :
            name_p = ((Nh_WebIDL_ParseNode*)Node_p->Children.pp[2])->Token_p->String.bytes_p;
            break;

        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        {
            Nh_List AttributeNames = Nh_initList(1);
            Nh_WebIDL_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_ATTRIBUTE_NAME, &AttributeNames);
            if (AttributeNames.size == 1) {
                if (((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Token_p->String.bytes_p;
                } 
            }
            Nh_freeList(&AttributeNames, NH_FALSE);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_OPERATION :
        {
            Nh_List OperationNames = Nh_initList(1);
            Nh_WebIDL_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_OPERATION_NAME, &OperationNames);
            if (OperationNames.size == 1) {
                if (((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Token_p->String.bytes_p;
                } 
            }
            Nh_freeList(&OperationNames, NH_FALSE);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE :
            break;
    }

NH_WEBIDL_END(name_p)
}

static NH_WEBIDL_RESULT Nh_WebIDL_getInterfaceMembers(
    Nh_WebIDL_Interface *Interface_p, Nh_WebIDL_ParseNode *InterfaceRest_p, NH_BOOL partial)
{
NH_WEBIDL_BEGIN()

    Nh_List InterfaceMembers = Nh_initList(8);

    if (!partial) {
        Nh_WebIDL_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_CLASS_MEMBER, &InterfaceMembers);
        for (int i = 0; i < InterfaceMembers.size; ++i) {
            Nh_WebIDL_InterfaceMember *InterfaceMember_p = Nh_incrementArray(&Interface_p->Members);
            if (((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0])->type == NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_MEMBER) {
                InterfaceMember_p->Node_p = ((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0])->Children.pp[0];
            }
            else {InterfaceMember_p->Node_p = ((Nh_WebIDL_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0];} // constructor
            InterfaceMember_p->name_p = Nh_WebIDL_getInterfaceMemberName(InterfaceMember_p->Node_p);
        }
    }
    else {
        Nh_WebIDL_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_MEMBER, &InterfaceMembers);
        for (int i = 0; i < InterfaceMembers.size; ++i) {
            Nh_WebIDL_InterfaceMember *InterfaceMember_p = Nh_incrementArray(&Interface_p->Members);
            InterfaceMember_p->Node_p = ((Nh_WebIDL_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0];
            InterfaceMember_p->name_p = Nh_WebIDL_getInterfaceMemberName(InterfaceMember_p->Node_p);
        }
    }
  
    Nh_freeList(&InterfaceMembers, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

static NH_WEBIDL_RESULT Nh_WebIDL_getInterfaceInheritance(
    Nh_WebIDL_Interface *Interface_p, Nh_WebIDL_ParseNode *InterfaceRest_p)
{
NH_WEBIDL_BEGIN()

    Nh_List Inheritance = Nh_initList(1);
    Nh_WebIDL_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_INHERITANCE, &Inheritance);

    if (Inheritance.size == 1 && ((Nh_WebIDL_ParseNode*)Inheritance.pp[0])->Children.size > 0) 
    {
        Interface_p->Inheritance_p = Nh_allocate(sizeof(Nh_WebIDL_InterfaceInheritance));
        NH_WEBIDL_CHECK_MEM(Interface_p->Inheritance_p)
        Interface_p->Inheritance_p->interface_p = ((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)Inheritance.pp[0])->Children.pp[1])->Token_p->String.bytes_p;
        Interface_p->Inheritance_p->specification_p = NULL;
        if (((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.size == 2) {
            Interface_p->Inheritance_p->specification_p = ((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)((Nh_WebIDL_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.pp[1])->Token_p->String.bytes_p;
        }
    }

    Nh_freeList(&Inheritance, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

static Nh_WebIDL_Interface Nh_WebIDL_createInterface(
    Nh_WebIDL_Specification *Specification_p, Nh_WebIDL_ParseNode *InterfaceRest_p, NH_BOOL partial)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Interface Interface;
    Interface.partial = partial;
    Interface.Members = Nh_initArray(sizeof(Nh_WebIDL_InterfaceMember), 8);
    Interface.name_p  = ((Nh_WebIDL_ParseNode*)InterfaceRest_p->Children.pp[0])->Token_p->String.bytes_p;
    Interface.Specification_p = Specification_p;
    Interface.Inheritance_p   = NULL;

    Nh_WebIDL_getInterfaceMembers(&Interface, InterfaceRest_p, partial);
    Nh_WebIDL_getInterfaceInheritance(&Interface, InterfaceRest_p);

NH_WEBIDL_END(Interface)
}

NH_WEBIDL_RESULT Nh_WebIDL_createInterfaces(
    Nh_WebIDL_Fragment *Fragment_p)
{
NH_WEBIDL_BEGIN()

    Fragment_p->Interfaces = Nh_initArray(sizeof(Nh_WebIDL_Interface), 8);

    Nh_List InterfaceRests = Nh_initList(8);
    Nh_List PartialInterfaceRests = Nh_initList(8);

    Nh_WebIDL_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_CLASS_REST, &InterfaceRests);
    Nh_WebIDL_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_REST, &PartialInterfaceRests);

    for (int i = 0; i < InterfaceRests.size; ++i) {
        Nh_WebIDL_Interface *Interface_p = Nh_incrementArray(&Fragment_p->Interfaces);
        NH_WEBIDL_CHECK_MEM(Interface_p)
        *Interface_p = Nh_WebIDL_createInterface(Fragment_p->Specification_p, InterfaceRests.pp[i], NH_FALSE);
    }

    for (int i = 0; i < PartialInterfaceRests.size; ++i) {
        Nh_WebIDL_Interface *Interface_p = Nh_incrementArray(&Fragment_p->Interfaces);
        NH_WEBIDL_CHECK_MEM(Interface_p)
        *Interface_p = Nh_WebIDL_createInterface(Fragment_p->Specification_p, PartialInterfaceRests.pp[i], NH_TRUE);
    }

    Nh_freeList(&InterfaceRests, NH_FALSE);
    Nh_freeList(&PartialInterfaceRests, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

Nh_WebIDL_Interface *Nh_WebIDL_getInterfaceFromFragment(
    Nh_WebIDL_Fragment *Fragment_p, NH_BYTE *className_p)
{
NH_WEBIDL_BEGIN()

    Nh_WebIDL_Interface *Interface_p = NULL;
    for (int i = 0; i < Fragment_p->Interfaces.length; ++i) {
        Interface_p = &((Nh_WebIDL_Interface*)Fragment_p->Interfaces.bytes_p)[i];
        if (!strcmp(Interface_p->name_p, className_p)) {break;}
        Interface_p = NULL;
    }

NH_WEBIDL_END(Interface_p)
}

