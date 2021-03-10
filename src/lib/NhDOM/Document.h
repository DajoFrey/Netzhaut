#ifndef NH_DOM_DOCUMENT_H
#define NH_DOM_DOCUMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#endif

/** @addtogroup NhDOMFunctions
 *  @{
 */

    extern Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_NO_QUIRKS;
    extern Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_QUIRKS;
    extern Nh_WebIDL_DOMString NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS;

/** @} */

/** @addtogroup NhDOMFunctions
 *  @{
 */

    NH_DOM_RESULT Nh_DOM_initDocumentModes(
    );
    
    NH_DOM_RESULT Nh_DOM_freeDocumentModes(
    );

    NH_DOM_RESULT Nh_DOM_setDocumentMode(
        Nh_WebIDL_Object *Document_p, Nh_WebIDL_DOMString *Mode_p
    );

    NH_DOM_RESULT Nh_DOM_setDocumentType(
        Nh_WebIDL_Object *Document_p, Nh_WebIDL_Object *DocumentType_p
    );

/** @} */

#endif
