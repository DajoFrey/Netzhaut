#ifndef NH_HTML_GLOBAL_EVENT_HANDLERS_H
#define NH_HTML_GLOBAL_EVENT_HANDLERS_H

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

    typedef struct Nh_HTML_GlobalEventHandlers {
        NH_INTERFACE onabort;
        NH_INTERFACE onauxclick;
        NH_INTERFACE onblur;
        NH_INTERFACE oncancel;
        NH_INTERFACE oncanplay;
        NH_INTERFACE oncanplaythrough;
        NH_INTERFACE onchange;
        NH_INTERFACE onclick;
        NH_INTERFACE onclose;
        NH_INTERFACE oncontextmenu;
        NH_INTERFACE oncuechange;
        NH_INTERFACE ondblclick;
        NH_INTERFACE ondrag;
        NH_INTERFACE ondragend;
        NH_INTERFACE ondragenter;
        NH_INTERFACE ondragleave;
        NH_INTERFACE ondragover;
        NH_INTERFACE ondragstart;
        NH_INTERFACE ondrop;
        NH_INTERFACE ondurationchange;
        NH_INTERFACE onemptied;
        NH_INTERFACE onended;
        NH_INTERFACE onerror;
        NH_INTERFACE onfocus;
        NH_INTERFACE onformdata;
        NH_INTERFACE oninput;
        NH_INTERFACE oninvalid;
        NH_INTERFACE onkeydown;
        NH_INTERFACE onkeypress;
        NH_INTERFACE onkeyup;
        NH_INTERFACE onload;
        NH_INTERFACE onloadeddata;
        NH_INTERFACE onloadedmetadata;
        NH_INTERFACE onloadstart;
        NH_INTERFACE onmousedown;
        NH_INTERFACE onmouseenter;
        NH_INTERFACE onmouseleave;
        NH_INTERFACE onmousemove;
        NH_INTERFACE onmouseout;
        NH_INTERFACE onmouseover;
        NH_INTERFACE onmouseup;
        NH_INTERFACE onpause;
        NH_INTERFACE onplay;
        NH_INTERFACE onplaying;
        NH_INTERFACE onprogress;
        NH_INTERFACE onratechange;
        NH_INTERFACE onreset;
        NH_INTERFACE onresize;
        NH_INTERFACE onscroll;
        NH_INTERFACE onsecuritypolicyviolation;
        NH_INTERFACE onseeked;
        NH_INTERFACE onseeking;
        NH_INTERFACE onselect;
        NH_INTERFACE onslotchange;
        NH_INTERFACE onstalled;
        NH_INTERFACE onsubmit;
        NH_INTERFACE onsuspend;
        NH_INTERFACE ontimeupdate;
        NH_INTERFACE ontoggle;
        NH_INTERFACE onvolumechange;
        NH_INTERFACE onwaiting;
        NH_INTERFACE onwebkitanimationend;
        NH_INTERFACE onwebkitanimationiteration;
        NH_INTERFACE onwebkitanimationstart;
        NH_INTERFACE onwebkittransitionend;
        NH_INTERFACE onwheel;
    } Nh_HTML_GlobalEventHandlers;

/** @} */

#endif 

