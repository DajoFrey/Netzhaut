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

#include "../NhWebIDL/Runtime/String.h"
#include "../NhWebIDL/Common/Macro.h"
#include NH_WEBIDL_FLOW1
#include NH_WEBIDL_CUSTOM_CHECK1

#include "../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define DATA CharacterData_p->Attributes.pp[0]
#define LENGTH CharacterData_p->Attributes.pp[1]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeCharacterData(
    Nh_WebIDL_Object *CharacterData_p)
{
NH_DOM_BEGIN()

    DATA = Nh_allocate(sizeof(Nh_WebIDL_DOMString));
    NH_DOM_CHECK_MEM(DATA)

    *((Nh_WebIDL_DOMString*)DATA) = Nh_WebIDL_initDOMString(64);
    LENGTH = &((Nh_WebIDL_DOMString*)DATA)->length;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_appendToCharacterData(
    Nh_WebIDL_Object *CharacterData_p, Nh_WebIDL_DOMString DOMString)
{
NH_DOM_BEGIN()

    NH_WEBIDL_CHECK(NH_DOM_ERROR_BAD_STATE, Nh_WebIDL_appendToDOMString(DATA, DOMString.bytes_p, DOMString.length))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

