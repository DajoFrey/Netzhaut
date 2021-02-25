#ifndef NH_HTML_HTML_ELEMENT_H
#define NH_HTML_HTML_ELEMENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Netzhaut/Interface.h"
#include "../../Netzhaut/String.h"
#include "../../Netzhaut/Type/Logic.h"

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_HTMLElement {

        Nh_WebIDL_Interface Interface;

        Nh_String Title;
        Nh_String Lang;
        NH_BOOL translate;
        Nh_String Dir;
        NH_BOOL hidden;
        Nh_String AccessKey;
        Nh_String AccessKeyLabel;
        NH_BOOL draggable;
        NH_BOOL spellcheck;
        Nh_String Autocapitalize;

        Nh_HTML_GlobalEventHandlers GlobalEventHandlers;
        Nh_HTML_DocumentAndElementEventHandlers DocumentAndElementEventHandlers;

    } Nh_HTML_HTMLElement;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    void *Nh_HTML_HTMLElement_constructor(
        Nh_HTML_HTMLElement *Element_p, int argc, void *argv_pp[]
    );

    void *Nh_HTML_HTMLElement_click(
        Nh_HTML_HTMLElement *Element_p, int argc, void *argv_pp[]
    );

    void *Nh_HTML_HTMLElement_attachInternals(
        Nh_HTML_HTMLElement *Element_p, int argc, void *argv_pp[]
    );

/** @} */

#endif 

