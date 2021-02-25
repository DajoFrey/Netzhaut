// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/PostArrange.h"

#include "../../Main/Header/Node.h"
#include "../../Main/Header/Log.h"
#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/String.h"
#include "../../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_CSS_UTILS
#include NH_HTML_UTILS
#include NH_DEFAULT_CHECK

#include <string.h>
#include <ctype.h>
#include <math.h>

// BLOCK HEIGHTS ===================================================================================

static NH_PIXEL Nh_HTML_updateBlockHeights(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];

        NH_PIXEL maxY = Nh_HTML_updateBlockHeights(Child_p);

        if (Node_p->Properties.Position.display == NH_CSS_DISPLAY_BLOCK) {
            NH_PIXEL currentMaxY = NH_HTML_MAX_Y(Node_p->Box); 
            if (maxY > currentMaxY) {Node_p->Box.Size.height += maxY - NH_HTML_MAX_Y(Nh_HTML_getContentPixelBox(Node_p, -1));}
        }
    }

    NH_END(NH_HTML_MAX_Y((*Nh_HTML_getMarginPixelBox(Node_p, -1))))

NH_END(NH_SUCCESS)
}

// ARRANGE =========================================================================================

NH_RESULT Nh_HTML_postArrange(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_HTML_updateBlockHeights(Document_p->Tree.Render.data_pp[0]);

//    Nh_HTML_Node *Node_p = Nh_HTML_getNode(Document_p, NH_HTML_TAG_HTML);
//    if (Node_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
//
//    Document_p->Content_p->Size.width  = NH_CLIP_TO_PIXEL(Node_p->Box.Size.width - 1.0f, Document_p->Content_p, NH_TRUE);
//    Document_p->Content_p->Size.height = NH_CLIP_TO_PIXEL(Node_p->Box.Size.height - 1.0f, Document_p->Content_p, NH_FALSE);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

