// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "NamedNodeMap.h"

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

#define ELEMENT ((Nh_DOM_NamedNodeMap*)NamedNodeMap_p->internal_p)->Element_p
#define ATTRIBUTE_LIST ((Nh_DOM_NamedNodeMap*)NamedNodeMap_p->internal_p)->AttributeList
#define LENGTH NamedNodeMap_p->Attributes.pp[0]

typedef struct Nh_DOM_NamedNodeMap {
    Nh_WebIDL_Object *Element_p;
    Nh_List AttributeList;
} Nh_DOM_NamedNodeMap;

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeNamedNodeMap(
    Nh_WebIDL_Object *NamedNodeMap_p)
{
NH_DOM_BEGIN()

    NamedNodeMap_p->internal_p = Nh_allocate(sizeof(Nh_DOM_NamedNodeMap));
    NH_DOM_CHECK_MEM(NamedNodeMap_p->internal_p)

    ELEMENT = NULL;
    ATTRIBUTE_LIST = Nh_initList(8);

    LENGTH = &ATTRIBUTE_LIST.size;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

Nh_WebIDL_Object *Nh_DOM_createNamedNodeMap(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()

#include NH_DOM_CUSTOM_CHECK

    Nh_WebIDL_Object *NamedNodeMap_p = Nh_WebIDL_createObject("DOM", "NamedNodeMap");
    NH_DOM_CHECK_MEM(NULL, NamedNodeMap_p)

    ELEMENT = Element_p;

#include NH_DOM_DEFAULT_CHECK

NH_DOM_END(NamedNodeMap_p)
}

// API =============================================================================================

