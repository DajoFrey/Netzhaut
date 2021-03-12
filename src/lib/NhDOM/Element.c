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
#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

#define NAMESPACE_URI Element_p->Attributes.pp[0]
#define NAMESPACE_PREFIX Element_p->Attributes.pp[1]
#define LOCAL_NAME Element_p->Attributes.pp[2]
#define TAG_NAME Element_p->Attributes.pp[3]
#define ATTRIBUTES Element_p->Attributes.pp[9]

typedef struct Nh_DOM_Element {
    int TODO;
} Nh_DOM_Element;

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeElement(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()

    Element_p->internal_p = Nh_allocate(sizeof(Nh_DOM_Element));
    NH_DOM_CHECK_MEM(Element_p->internal_p)

    ATTRIBUTES = Nh_DOM_createNamedNodeMap(Element_p);
    NH_DOM_CHECK_MEM(ATTRIBUTES)

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// INTERNAL ========================================================================================

Nh_WebIDL_Object *Nh_DOM_getElement(
    Nh_WebIDL_Object *Object_p)
{
NH_DOM_BEGIN()
NH_DOM_END(Nh_WebIDL_getObject(Object_p, "DOM", "Element"))
}

static Nh_WebIDL_DOMString Nh_DOM_allocateQualifiedName(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()

    Nh_WebIDL_DOMString QualifiedName = Nh_WebIDL_initDOMString(16);

    if (!NAMESPACE_PREFIX) {
        Nh_WebIDL_appendToDOMString(&QualifiedName, ((Nh_WebIDL_DOMString*)LOCAL_NAME)->bytes_p, ((Nh_WebIDL_DOMString*)LOCAL_NAME)->length);
    }
    else {
        NH_BYTE qualifiedName_p[255] = {'\0'};
        sprintf(qualifiedName_p, "%s:%s",((Nh_WebIDL_DOMString*)NAMESPACE_PREFIX)->bytes_p, ((Nh_WebIDL_DOMString*)LOCAL_NAME)->bytes_p);
        Nh_WebIDL_appendToDOMString(&QualifiedName, qualifiedName_p, strlen(qualifiedName_p));
    }

NH_DOM_END(QualifiedName)
}

// https://dom.spec.whatwg.org/#element-html-uppercased-qualified-name
static Nh_WebIDL_DOMString Nh_DOM_allocateHTMLUppercasedQualifiedName(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()

    Nh_WebIDL_DOMString QualifiedName = Nh_DOM_allocateQualifiedName(Element_p);

    if (NAMESPACE_URI == &NH_WEBIDL_HTML_NAMESPACE) {
        for (int i = 0; i < QualifiedName.length; ++i) {
            QualifiedName.bytes_p[i] = toupper(QualifiedName.bytes_p[i]);
        }
    }

NH_DOM_END(QualifiedName)
}

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

    Nh_WebIDL_Object *Element_p = Nh_DOM_getElement(Object_p);
    NH_DOM_CHECK_NULL(NULL, Element_p)

    NAMESPACE_URI    = Namespace_p;
    NAMESPACE_PREFIX = NamespacePrefix_p;
    LOCAL_NAME       = LocalName_p;

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

Nh_WebIDL_DOMString *Nh_DOM_getLocalName(
    Nh_WebIDL_Object *Element_p)
{
NH_DOM_BEGIN()
NH_DOM_END(Element_p == NULL ? NULL : LOCAL_NAME)
}

// API OPERATIONS ==================================================================================

