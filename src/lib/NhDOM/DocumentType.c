// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "DocumentType.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include "../NhWebIDL/Runtime/Object.h"
#include "../NhWebIDL/Runtime/String.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

#define NAME DocumentType_p->Attributes.pp[0]
#define PUBLIC_ID DocumentType_p->Attributes.pp[1] 
#define SYSTEM_ID DocumentType_p->Attributes.pp[2]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeDocumentType(
    Nh_WebIDL_Object *DocumentType_p)
{
NH_DOM_BEGIN()

    NAME = Nh_allocate(sizeof(Nh_WebIDL_DOMString));
    NH_DOM_CHECK_MEM(NAME)
    *((Nh_WebIDL_DOMString*)NAME) = Nh_WebIDL_initDOMString(32);

    PUBLIC_ID = Nh_allocate(sizeof(Nh_WebIDL_DOMString));
    NH_DOM_CHECK_MEM(PUBLIC_ID)
    *((Nh_WebIDL_DOMString*)PUBLIC_ID) = Nh_WebIDL_initDOMString(32);

    SYSTEM_ID = Nh_allocate(sizeof(Nh_WebIDL_DOMString));
    NH_DOM_CHECK_MEM(SYSTEM_ID)
    *((Nh_WebIDL_DOMString*)SYSTEM_ID) = Nh_WebIDL_initDOMString(32);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

Nh_WebIDL_Object *Nh_DOM_createDocumentType(
    Nh_WebIDL_DOMString *Name_p, Nh_WebIDL_DOMString *PublicId_p, Nh_WebIDL_DOMString *SystemId_p)
{
NH_DOM_BEGIN()

#include NH_DOM_CUSTOM_CHECK

    Nh_WebIDL_Object *DocumentType_p = Nh_WebIDL_createObject("DOM", "DocumentType");
    NH_DOM_CHECK_MEM(NULL, DocumentType_p)

    if (Name_p != NULL) {
        Nh_WebIDL_appendToDOMString(NAME, Name_p->bytes_p, Name_p->length);
    }
    if (PublicId_p != NULL) {
        Nh_WebIDL_appendToDOMString(PUBLIC_ID, PublicId_p->bytes_p, PublicId_p->length);
    }
    if (SystemId_p != NULL) {
        Nh_WebIDL_appendToDOMString(SYSTEM_ID, SystemId_p->bytes_p, SystemId_p->length);
    }

#include NH_DOM_DEFAULT_CHECK

NH_DOM_END(DocumentType_p)
}

// API =============================================================================================

