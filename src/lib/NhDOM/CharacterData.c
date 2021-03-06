// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "CharacterData.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhWeb/Runtime/String.h"
#include "../NhWeb/Common/Macro.h"
#include NH_WEB_FLOW1
#include NH_WEB_CUSTOM_CHECK1

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define DATA CharacterData_p->Attributes.pp[0]
#define LENGTH *((NH_WEB_UNSIGNED_LONG*)CharacterData_p->Attributes.pp[1])

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeCharacterData(
    Nh_Web_Object *CharacterData_p)
{
NH_DOM_BEGIN()

    *((Nh_Web_DOMString*)DATA) = Nh_Web_initDOMString(64);
    LENGTH = 0;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_appendToCharacterData(
    Nh_Web_Object *CharacterData_p, Nh_Web_DOMString DOMString)
{
NH_DOM_BEGIN()

    NH_WEB_CHECK(NH_DOM_ERROR_BAD_STATE, Nh_Web_appendToDOMString(DATA, DOMString.bytes_p, DOMString.length))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

