// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Node.h"
#include "CharacterData.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhWeb/Runtime/String.h"
#include "../NhWeb/Runtime/Object.h"

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeComment(
    Nh_Web_Object *Node_p)
{
NH_DOM_BEGIN()

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

Nh_Web_Object *Nh_DOM_createComment(
    Nh_Web_DOMString DOMString)
{
NH_DOM_BEGIN()

#include NH_DOM_CUSTOM_CHECK

    Nh_Web_Object *Comment_p = Nh_Web_createObject("DOM", "Comment");
    NH_DOM_CHECK_MEM(NULL, Comment_p)
    NH_DOM_CHECK(NULL, Nh_DOM_appendToCharacterData(Comment_p->Inherit_p, DOMString))

#include NH_DOM_DEFAULT_CHECK

NH_DOM_END(Comment_p)
}

// API =============================================================================================

