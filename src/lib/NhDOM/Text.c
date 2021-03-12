// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Text.h"
#include "Node.h"
#include "CharacterData.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhWebIDL/Runtime/Object.h"
#include "../NhWebIDL/Runtime/String.h"
#include "../NhWebIDL/Common/Macro.h"
#include NH_WEBIDL_FLOW1
#include NH_WEBIDL_CUSTOM_CHECK1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeText(
    Nh_WebIDL_Object *CharacterData_p)
{
NH_DOM_BEGIN()

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

Nh_WebIDL_Object *Nh_DOM_createText(
    Nh_WebIDL_DOMString Data, Nh_WebIDL_Object *NodeDocument_p)
{
NH_DOM_BEGIN()

#include NH_DOM_CUSTOM_CHECK

    Nh_WebIDL_Object *Text_p = Nh_WebIDL_createObject("DOM", "Text");
    NH_DOM_CHECK_MEM(NULL, Text_p)

    NH_DOM_CHECK(NULL, Nh_DOM_setNodeDocument(Text_p->Child_p->Child_p, NodeDocument_p))
    NH_DOM_CHECK(NULL, Nh_DOM_appendToCharacterData(Text_p->Child_p, Data))

#include NH_DOM_DEFAULT_CHECK

NH_DOM_END(Text_p)
}

NH_DOM_RESULT Nh_DOM_appendToText(
    Nh_WebIDL_Object *Text_p, Nh_WebIDL_DOMString DOMString)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(Nh_DOM_appendToCharacterData(Text_p->Child_p, DOMString))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

