#ifndef NH_DOM_ELEMENT_H
#define NH_DOM_ELEMENT_H

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

    Nh_WebIDL_Object *Nh_DOM_getElement(
        Nh_WebIDL_Object *Object_p
    );

    Nh_WebIDL_Object *Nh_DOM_createElement(
        Nh_WebIDL_Object *Document_p, Nh_WebIDL_DOMString *LocalName_p, Nh_WebIDL_DOMString *Namespace_p, Nh_WebIDL_DOMString *NamespacePrefix_p,
        Nh_WebIDL_DOMString *Is_p, NH_BOOL synchronousCustomElements, Nh_WebIDL_Interface *Interface_p
    );

    Nh_WebIDL_DOMString *Nh_DOM_getLocalName(
        Nh_WebIDL_Object *Element_p
    );

    Nh_WebIDL_DOMString *Nh_DOM_getNamespaceURI(
        Nh_WebIDL_Object *Element_p
    );

/** @} */

#endif
