// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Element.h"
#include "Node.h"
#include "NamedNodeMap.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include "../NhWebIDL/Runtime/Object.h"
#include "../NhWebIDL/Runtime/String.h"
#include "../NhWebIDL/Runtime/Runtime.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define NAMESPACE_URI Element_p->Attributes.pp[0]
#define PREFIX Element_p->Attributes.pp[1]
#define LOCAL_NAME Element_p->Attributes.pp[2]
#define ATTRIBUTES Element_p->Attributes.pp[9]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeElement(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()

    ATTRIBUTES = Nh_DOM_createNamedNodeMap(Element_p);
    NH_DOM_CHECK_MEM(ATTRIBUTES)

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

// https://dom.spec.whatwg.org/#concept-create-element
Nh_WebIDL_Object *Nh_DOM_createElement(
    Nh_WebIDL_Object *Document_p, Nh_WebIDL_DOMString *LocalName_p, Nh_WebIDL_DOMString *Namespace_p, Nh_WebIDL_DOMString *NamespacePrefix_p,
    Nh_WebIDL_DOMString *Is_p, NH_BOOL synchronousCustomElements, Nh_WebIDL_Interface *Interface_p)
{
NH_DOM_BEGIN()

#include NH_DOM_CUSTOM_CHECK

    NH_DOM_CHECK_NULL(NULL, Document_p)
    NH_DOM_CHECK_NULL(NULL, LocalName_p)

    if (Interface_p == NULL) {
        Interface_p = Nh_WebIDL_getInterface("DOM", "Element");
    }

    NH_DOM_CHECK_NULL(NULL, Interface_p)

    Nh_WebIDL_Object *Object_p = Nh_WebIDL_createObjectFromInterface(Interface_p);
    NH_DOM_CHECK_NULL(NULL, Object_p)

    Nh_WebIDL_Object *Element_p = Nh_WebIDL_getObject(Object_p, "DOM", "Element");
    NH_DOM_CHECK_NULL(NULL, Element_p)

    NAMESPACE_URI = Namespace_p;
    PREFIX        = NamespacePrefix_p;
    LOCAL_NAME    = LocalName_p;

    NH_DOM_CHECK(NULL, Nh_DOM_setNodeDocument(Element_p->Child_p, Document_p))

#include NH_DOM_DEFAULT_CHECK

NH_DOM_END(Object_p)
}

Nh_WebIDL_DOMString *Nh_DOM_getNamespaceURI(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(Element_p == NULL ? NULL : NAMESPACE_URI)
}

// API =============================================================================================

