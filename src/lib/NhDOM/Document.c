// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Document.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhWeb/Runtime/String.h"
#include "../NhWeb/Common/Macro.h"
#include NH_WEB_FLOW1
#include NH_WEB_CUSTOM_CHECK1

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define MODE Document_p->Attributes.pp[0]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeDocument(
    Nh_Web_Object *Document_p)
{
NH_DOM_BEGIN()

    *((Nh_Web_DOMString*)MODE) = Nh_Web_initDOMString(32);
    NH_WEB_CHECK(NH_DOM_ERROR_BAD_STATE, Nh_Web_appendToDOMString(MODE, "no-quirks", 9))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_setDocumentMode(
    Nh_Web_Object *Document_p, NH_BYTE *mode_p)
{
NH_DOM_BEGIN()

    Nh_Web_freeDOMString(MODE);
    *((Nh_Web_DOMString*)MODE) = Nh_Web_initDOMString(32);
    NH_WEB_CHECK(NH_DOM_ERROR_BAD_STATE, Nh_Web_appendToDOMString(MODE, mode_p, strlen(mode_p)))

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

