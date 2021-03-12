// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Document.h"

#include "../Common/Macro.h"
#include NH_HTML_FLOW
#include NH_HTML_DEFAULT_CHECK

#include "../../NhDOM/Node.h"
#include "../../NhWebIDL/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

// INITIALIZE ======================================================================================

NH_HTML_RESULT Nh_HTML_initializeDocument(
    Nh_WebIDL_Object *Document_p)
{
NH_HTML_BEGIN()

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// API =============================================================================================

Nh_WebIDL_Object *Nh_HTML_createDocument()
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *Document_p = Nh_WebIDL_createCompositeObject("HTML", "Document");

    // The node document of a document is that document itself. All nodes have a node document at all times. 
    Nh_DOM_setNodeDocument(Nh_DOM_getNode(Document_p), Document_p);

NH_HTML_END(Document_p)
}

