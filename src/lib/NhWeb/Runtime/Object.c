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
#include NH_WEB_FLOW
#include NH_WEB_CUSTOM_CHECK

#include "../../NhLoader/Loader.h"
#include "../../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OBJECT ==========================================================================================

static NH_WEB_RESULT Nh_Web_addAttribute(
    Nh_Web_Object *Object_p, Nh_Web_ParseNode *Node_p)
{
NH_WEB_BEGIN()

    Nh_Web_ParseNode *Type_p = NULL;

    Nh_List Types = Nh_initList(1);
    switch (Node_p->type)
    {
        case NH_WEB_PARSE_NODE_READ_ONLY_MEMBER :
        case NH_WEB_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        case NH_WEB_PARSE_NODE_PRIVATE_ATTRIBUTE :
            Nh_Web_getParseNodes(Node_p, NH_WEB_PARSE_NODE_TYPE, &Types);
            break;
    }

    if (Types.size == 1) 
    {
        NH_WEB_TYPE type = Nh_Web_getType(Types.pp[0]);
        size_t size = Nh_Web_getAllocationSize(type); 
        Nh_appendToList(&Object_p->Attributes, size <= 0 ? NULL : Nh_allocate(size));
        NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
    }

    Nh_freeList(&Types, NH_FALSE);

NH_WEB_DIAGNOSTIC_END(NH_WEB_ERROR_BAD_STATE)
}
 
static NH_BOOL Nh_Web_isAttribute(
    Nh_Web_ParseNode *Node_p)
{
NH_WEB_BEGIN()

    switch (Node_p->type)
    {
        case NH_WEB_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        case NH_WEB_PARSE_NODE_PRIVATE_ATTRIBUTE :
            NH_WEB_END(NH_TRUE)
            break;
    }

NH_WEB_END(NH_FALSE)
}

static Nh_Web_Object *Nh_Web_createObjectFromClass(
    Nh_Web_Class *Class_p)
{
NH_WEB_BEGIN()

    Nh_Web_Object *Object_p = Nh_allocate(sizeof(Nh_Web_Object));
    NH_WEB_CHECK_MEM(NULL, Object_p)
 
    Object_p->Class_p    = Class_p;
    Object_p->Inherit_p  = NULL;
    Object_p->Parent_p   = NULL;
    Object_p->Attributes = Nh_initList(8);

    for (int i = 0; i < Class_p->Members.length; ++i) 
    {
        Nh_Web_ClassMember *ClassMember_p = &((Nh_Web_ClassMember*)Class_p->Members.bytes_p)[i];
        if (Nh_Web_isAttribute(ClassMember_p->Node_p)) {
            NH_WEB_CHECK(NULL, Nh_Web_addAttribute(Object_p, ClassMember_p->Node_p))
        }
    }

NH_WEB_END(Object_p)
}

typedef int (*init_f)(Nh_Web_Object *Object_p);

Nh_Web_Object *Nh_Web_createObject(
    NH_BYTE *specification_p, NH_BYTE *class_p, ...)
{
NH_WEB_BEGIN()

    NH_WEB_CHECK_NULL(NULL, specification_p)
    NH_WEB_CHECK_NULL(NULL, class_p)

    Nh_Web_Class *Class_p = Nh_Web_getClass(specification_p, class_p);
    NH_WEB_CHECK_NULL(NULL, Class_p)

    if (Class_p->partial) {NH_WEB_END(NULL)}

    Nh_Web_Object *Object_p = Nh_Web_createObjectFromClass(Class_p);
    NH_WEB_CHECK_NULL(NULL, Object_p)

    NH_BYTE funcName_p[255] = {'\0'};
    sprintf(funcName_p, "Nh_%s_initialize%s", Object_p->Class_p->Fragment_p->Specification_p->name_p, Object_p->Class_p->name_p);

    NH_BYTE libName_p[255] = {'\0'};
    sprintf(libName_p, "Nh%s", Object_p->Class_p->Fragment_p->Specification_p->name_p);

    init_f initNewObject_f = NH_LOADER.loadFunction_f(libName_p, 0, funcName_p);
    if (initNewObject_f) {initNewObject_f(Object_p);} 

    // TODO cross specification inheritance
    if (Class_p->Inherit_p != NULL && Class_p->Inherit_p->Children.size > 0) {
        Object_p->Inherit_p = Nh_Web_createObject(Class_p->Fragment_p->Specification_p->name_p, ((Nh_Web_ParseNode*)Class_p->Inherit_p->Children.pp[1])->Token_p->String.bytes_p);
    }

NH_WEB_END(Object_p)
}

// COMPOSITE OBJECT ================================================================================

Nh_Web_CompositeObject *Nh_Web_createCompositeObject(
    NH_BYTE *specification_p, NH_BYTE *class_p, ...)
{
NH_WEB_BEGIN()

    NH_WEB_CHECK_NULL(NULL, specification_p)
    NH_WEB_CHECK_NULL(NULL, class_p)

    Nh_Web_Class *Class_p = Nh_Web_getClass(specification_p, class_p);
    NH_WEB_CHECK_NULL(NULL, Class_p)

    if (!Class_p->partial) {NH_WEB_END(NULL)}

    Nh_List Classes = Nh_Web_getCompositeClasses(Class_p);
    if (Classes.size <= 1) {NH_WEB_END(NULL)}

    Nh_Web_CompositeObject *CompositeObject_p = Nh_allocate(sizeof(Nh_Web_CompositeObject));
    NH_WEB_CHECK_MEM(NULL, CompositeObject_p)
 
    CompositeObject_p->PartialObjects = Nh_initList(Classes.size);

    for (int i = 0; i < Classes.size; ++i) 
    {
        Class_p = Classes.pp[i];

        Nh_Web_Object *Object_p = Nh_Web_createObjectFromClass(Class_p);
        NH_WEB_CHECK_NULL(NULL, Object_p)
    
        Nh_appendToList(&CompositeObject_p->PartialObjects, Object_p);

        NH_BYTE funcName_p[255] = {'\0'};
        sprintf(funcName_p, "Nh_%s_initialize%s", Object_p->Class_p->Fragment_p->Specification_p->name_p, Object_p->Class_p->name_p);
    
        NH_BYTE libName_p[255] = {'\0'};
        sprintf(libName_p, "Nh%s", Object_p->Class_p->Fragment_p->Specification_p->name_p);
    
        init_f initNewObject_f = NH_LOADER.loadFunction_f(libName_p, 0, funcName_p);
        if (initNewObject_f) {initNewObject_f(Object_p);} 
    
        // TODO cross specification inheritance
        if (Class_p->Inherit_p != NULL && Class_p->Inherit_p->Children.size > 0) {
            Object_p->Inherit_p = Nh_Web_createObject(Class_p->Fragment_p->Specification_p->name_p, ((Nh_Web_ParseNode*)Class_p->Inherit_p->Children.pp[1])->Token_p->String.bytes_p);
        }
    }

    Nh_freeList(&Classes, NH_FALSE);

NH_WEB_END(CompositeObject_p)
}

Nh_Web_Object *Nh_Web_decompose(
    Nh_Web_CompositeObject *CompositeObject_p, NH_BYTE *specification_p, ...) 
{
NH_WEB_BEGIN()

    NH_WEB_CHECK_NULL(NULL, CompositeObject_p)
    NH_WEB_CHECK_NULL(NULL, specification_p)

    Nh_Web_Object *Object_p = NULL;

    for (int i = 0; i < CompositeObject_p->PartialObjects.size; ++i) {
        Object_p = CompositeObject_p->PartialObjects.pp[i];
        if (!strcmp(Object_p->Class_p->Fragment_p->Specification_p->name_p, specification_p)) {
            break;
        }
        Object_p = NULL;
    }

NH_WEB_END(Object_p)
}

