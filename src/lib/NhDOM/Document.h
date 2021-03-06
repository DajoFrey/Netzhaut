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

    NH_DOM_RESULT Nh_DOM_setDocumentMode(
        Nh_Web_Object *Document_p, NH_BYTE *mode_p
    );

/** @} */

#endif
