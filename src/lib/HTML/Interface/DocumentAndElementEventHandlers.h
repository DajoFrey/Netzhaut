#ifndef NH_HTML_DOCUMENT_AND_ELEMENT_EVENT_HANDLERS_H
#define NH_HTML_DOCUMENT_AND_ELEMENT_EVENT_HANDLERS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Netzhaut/Interface.h"

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_DocumentAndElementEventHandlers {
        NH_INTERFACE oncopy;
        NH_INTERFACE oncut;
        NH_INTERFACE onpaste;
    } Nh_HTML_DocumentAndElementEventHandlers;

/** @} */

#endif 

