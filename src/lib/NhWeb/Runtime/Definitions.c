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
#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CLASSES =========================================================================================

static Nh_Web_Class Nh_Web_createClass(
    Nh_Web_Fragment *Fragment_p, Nh_Web_ParseNode *ClassRest_p, NH_BOOL partial)
{
NH_WEB_BEGIN()

    Nh_Web_Class Class;
    Class.partial    = partial;
    Class.Members    = Nh_initArray(sizeof(Nh_Web_ClassMember), 8);
    Class.name_p     = ((Nh_Web_ParseNode*)ClassRest_p->Children.pp[0])->Token_p->String.bytes_p;
    Class.Fragment_p = Fragment_p;

    Nh_List ClassMembers = Nh_initList(8);

    if (!partial) {
        Nh_Web_getParseNodes(ClassRest_p, NH_WEB_PARSE_NODE_CLASS_MEMBER, &ClassMembers);
        for (int i = 0; i < ClassMembers.size; ++i) {
            Nh_Web_ClassMember *ClassMember_p = Nh_getFromArray(&Class.Members, -1);
            if (((Nh_Web_ParseNode*)((Nh_Web_ParseNode*)ClassMembers.pp[i])->Children.pp[0])->type == NH_WEB_PARSE_NODE_PARTIAL_CLASS_MEMBER) {
                ClassMember_p->Node_p = ((Nh_Web_ParseNode*)((Nh_Web_ParseNode*)ClassMembers.pp[i])->Children.pp[0])->Children.pp[0];
            }
            else {ClassMember_p->Node_p = ((Nh_Web_ParseNode*)ClassMembers.pp[i])->Children.pp[0];} // constructor
        }
    }
    else {
        Nh_Web_getParseNodes(ClassRest_p, NH_WEB_PARSE_NODE_PARTIAL_CLASS_MEMBER, &ClassMembers);
        for (int i = 0; i < ClassMembers.size; ++i) {
            Nh_Web_ClassMember *ClassMember_p = Nh_getFromArray(&Class.Members, -1);
            ClassMember_p->Node_p = ((Nh_Web_ParseNode*)ClassMembers.pp[i])->Children.pp[0];
        }
    }
  
    Nh_List Inheritance = Nh_initList(1);
    Nh_Web_getParseNodes(ClassRest_p, NH_WEB_PARSE_NODE_INHERITANCE, &Inheritance);

    Class.Inherit_p = Nh_getFromList(&Inheritance, 0);

    Nh_freeList(&ClassMembers, NH_FALSE);
    Nh_freeList(&Inheritance, NH_FALSE);

NH_WEB_END(Class)
}

NH_WEB_RESULT Nh_Web_createClasses(
    Nh_Web_Fragment *Fragment_p)
{
NH_WEB_BEGIN()

    Fragment_p->Classes = Nh_initArray(sizeof(Nh_Web_Class), 8);

    Nh_List ClassRests = Nh_initList(8);
    Nh_List PartialClassRests = Nh_initList(8);

    Nh_Web_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEB_PARSE_NODE_CLASS_REST, &ClassRests);
    Nh_Web_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEB_PARSE_NODE_PARTIAL_CLASS_REST, &PartialClassRests);

    for (int i = 0; i < ClassRests.size; ++i) {
        Nh_Web_Class *Class_p = Nh_getFromArray(&Fragment_p->Classes, -1);
        *Class_p = Nh_Web_createClass(Fragment_p, ClassRests.pp[i], NH_FALSE);
    }

    for (int i = 0; i < PartialClassRests.size; ++i) {
        Nh_Web_Class *Class_p = Nh_getFromArray(&Fragment_p->Classes, -1);
        *Class_p = Nh_Web_createClass(Fragment_p, PartialClassRests.pp[i], NH_TRUE);
    }

    Nh_freeList(&ClassRests, NH_FALSE);
    Nh_freeList(&PartialClassRests, NH_FALSE);

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

Nh_Web_Class *Nh_Web_getClassFromFragment(
    Nh_Web_Fragment *Fragment_p, NH_BYTE *className_p)
{
NH_WEB_BEGIN()

    Nh_Web_Class *Class_p = NULL;
    for (int i = 0; i < Fragment_p->Classes.length; ++i) {
        Class_p = &((Nh_Web_Class*)Fragment_p->Classes.bytes_p)[i];
        if (!strcmp(Class_p->name_p, className_p)) {break;}
        Class_p = NULL;
    }

NH_WEB_END(Class_p)
}

