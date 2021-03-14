// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "BrowsingContexts.h"

#include "../Common/Macro.h"
#include NH_HTML_FLOW
#include NH_HTML_DEFAULT_CHECK

#include "../../NhDOM/Node.h"
#include "../../NhWebIDL/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// BROWSING CONTEXT ================================================================================

static void Nh_HTML_initBrowsingContext(
    Nh_HTML_BrowsingContext *Context_p)
{

}

Nh_HTML_BrowsingContext *Nh_HTML_createBrowsingContext(
    Nh_WebIDL_Object *Creator_p, Nh_WebIDL_Object *Embedder_p, Nh_HTML_BrowsingContextGroup *Group_p)
{
NH_HTML_BEGIN()

    Nh_HTML_BrowsingContext *NewContext_p = Nh_allocate(sizeof(Nh_HTML_BrowsingContext));
    NH_HTML_CHECK_MEM(NULL, NewContext_p)

    if (Creator_p) {

    }

NH_HTML_END(NewContext_p)
}

// BROWSING CONTEXT GROUP ==========================================================================

Nh_HTML_BrowsingContextGroup *Nh_HTML_createBrowsingContextGroup()
{
NH_HTML_BEGIN()

#include NH_HTML_CUSTOM_CHECK

    Nh_HTML_BrowsingContextGroup *NewGroup_p = Nh_allocate(sizeof(Nh_HTML_BrowsingContextGroup));
    NH_HTML_CHECK_MEM(NULL, NewGroup_p)

    Nh_appendToList(&NH_HTML_BROWSING_CONTEXT_GROUPS, NewGroup_p);

    Nh_HTML_BrowsingContext *NewContext_p = Nh_HTML_createBrowsingContext(NULL, NULL, NewGroup_p);
    NH_HTML_CHECK_MEM(NULL, NewContext_p)

#include NH_HTML_DEFAULT_CHECK

NH_HTML_END(Document_p)
}

