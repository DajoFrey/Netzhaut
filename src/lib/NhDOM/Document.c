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

#include "../NhWebIDL/Runtime/String.h"
#include "../NhWebIDL/Common/Macro.h"
#include NH_WEBIDL_FLOW1
#include NH_WEBIDL_CUSTOM_CHECK1

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define MODE ((Nh_DOM_Document*)Document_p->internal_p)->Mode_p

#define DOCTYPE Document_p->Attributes.pp[9]

typedef struct Nh_DOM_Document {
    Nh_WebIDL_DOMString *Mode_p;
} Nh_DOM_Document;

// DATA ============================================================================================

Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_QUIRKS;
Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeDocument(
    Nh_WebIDL_Object *Document_p)
{
NH_DOM_BEGIN()

    MODE = &NH_DOM_DOCUMENT_MODE_NO_QUIRKS;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

NH_DOM_RESULT Nh_DOM_initDocumentModes()
{
NH_DOM_BEGIN()

    NH_DOM_DOCUMENT_MODE_NO_QUIRKS = Nh_WebIDL_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_QUIRKS = Nh_WebIDL_initDOMString(16);
    NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS = Nh_WebIDL_initDOMString(16);

    Nh_WebIDL_appendToDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS, "no-quirks", 9);
    Nh_WebIDL_appendToDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS, "quirks", 6);
    Nh_WebIDL_appendToDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS, "limited-quirks", 14);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_freeDocumentModes()
{
NH_DOM_BEGIN()

    Nh_WebIDL_freeDOMString(&NH_DOM_DOCUMENT_MODE_NO_QUIRKS);
    Nh_WebIDL_freeDOMString(&NH_DOM_DOCUMENT_MODE_QUIRKS);
    Nh_WebIDL_freeDOMString(&NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_setDocumentMode(
    Nh_WebIDL_Object *Document_p, Nh_WebIDL_DOMString *Mode_p)
{
NH_DOM_BEGIN()

    MODE = Mode_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

NH_DOM_RESULT Nh_DOM_setDocumentType(
    Nh_WebIDL_Object *Document_p, Nh_WebIDL_Object *DocumentType_p)
{
NH_DOM_BEGIN()

    DOCTYPE = DocumentType_p;

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

