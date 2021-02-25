// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Cursor.h"
#include "../Header/Macros.h"

#include "../../HTML/Main/Header/Node.h"
#include "../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// CURSOR ==========================================================================================

NH_RESULT Nh_CSS_computeCursorProperty(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    switch (Node_p->tag)
    {
        case NH_HTML_TAG_CHAR : Node_p->Properties.cursor = NH_CSS_CURSOR_TEXT; break;
        default : Node_p->Properties.cursor = NH_CSS_CURSOR_DEFAULT;
    }

    Nh_HTML_Node *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL)
    {
        if (Parent_p->tag == NH_HTML_TAG_SELECT) {
            Node_p->Properties.cursor = NH_CSS_CURSOR_DEFAULT;
            break;
        }
        Parent_p = Parent_p->Parent_p;
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_IO_MOUSE_CURSOR Nh_CSS_normalizeMouseCursor(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    switch (Node_p->Properties.cursor)
    {
        case NH_CSS_CURSOR_TEXT : NH_DIAGNOSTIC_END(NH_IO_MOUSE_CURSOR_TEXT) 
    }

NH_DIAGNOSTIC_END(NH_IO_MOUSE_CURSOR_DEFAULT)
}

