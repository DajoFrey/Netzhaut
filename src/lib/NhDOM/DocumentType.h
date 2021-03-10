#ifndef NH_DOM_DOCUMENT_TYPE_H
#define NH_DOM_DOCUMENT_TYPE_H

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

    Nh_WebIDL_Object *Nh_DOM_createDocumentType(
        Nh_WebIDL_DOMString *Name_p, Nh_WebIDL_DOMString *PublicId_p, Nh_WebIDL_DOMString *SystemId_p
    );

/** @} */

#endif
