#ifndef NH_HTML_WINDOW_EVENT_HANDLERS_H
#define NH_HTML_WINDOW_EVENT_HANDLERS_H

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

    typedef struct Nh_HTML_WindowEventHandlers {
        NH_INTERFACE onafterprint;
        NH_INTERFACE onbeforeprint;
        NH_INTERFACE onbeforeunload;
        NH_INTERFACE onhashchange;
        NH_INTERFACE onlanguagechange;
        NH_INTERFACE onmessage;
        NH_INTERFACE onmessageerror;
        NH_INTERFACE onoffline;
        NH_INTERFACE ononline;
        NH_INTERFACE onpagehide;
        NH_INTERFACE onpageshow;
        NH_INTERFACE onpopstate;
        NH_INTERFACE onrejectionhandled;
        NH_INTERFACE onstorage;
        NH_INTERFACE onunhandledrejection;
        NH_INTERFACE onunload;
    } Nh_HTML_WindowEventHandlers;

/** @} */

#endif 

