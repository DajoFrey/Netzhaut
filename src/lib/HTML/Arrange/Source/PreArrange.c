// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/PreArrange.h"

#include "../../Main/Header/Node.h"
#include "../../Main/Header/Log.h"
#include "../../Main/Header/Macros.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/String.h"

#include "../../../Graphics/Main/Header/Texture.h"

#include NH_FLOW
#include NH_CSS_UTILS
#include NH_HTML_UTILS
#include NH_DEFAULT_CHECK

#include <string.h>
#include <ctype.h>
#include <math.h>

// DECLARE ==========================================================================================

static inline float Nh_HTML_initZ(
    Nh_HTML_Node *Node_p
);

static inline void Nh_HTML_initYPosition(
    Nh_HTML_Node *Node_p
);
static inline void Nh_HTML_initXPosition(
    Nh_HTML_Node *Node_p
);

static inline NH_PIXEL Nh_HTML_initBlockHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p
);
static inline NH_PIXEL Nh_HTML_initBlockWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p
);

static inline void Nh_HTML_updateParents(
    Nh_HTML_Node *Node_p
);

static inline NH_PIXEL Nh_HTML_getContentWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p
);
static inline NH_PIXEL Nh_HTML_getContentHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p
);

// ARRANGE =========================================================================================

static NH_RESULT Nh_HTML_initInlineRecursively(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE)
    {
        Nh_HTML_initXPosition(Node_p);
        Nh_HTML_initYPosition(Node_p);
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
        if (Nh_HTML_isMetaNode(Child_p) || Child_p->Properties.Position.display == NH_CSS_DISPLAY_NONE) {continue;}
        NH_CHECK(Nh_HTML_initInlineRecursively(Content_p, Child_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_initBlockRecursively(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_initXPosition(Node_p);
    Nh_HTML_initYPosition(Node_p);
    Node_p->Box.Size.height = Nh_HTML_initBlockHeight(Content_p, Node_p);
    Node_p->Box.Size.width  = Nh_HTML_initBlockWidth(Content_p, Node_p);
    Nh_HTML_updateParents(Node_p);
 
    for (int i = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
        if (Nh_HTML_isMetaNode(Child_p) || Child_p->Properties.Position.display != NH_CSS_DISPLAY_BLOCK || Child_p->Properties.Position.display == NH_CSS_DISPLAY_NONE) {continue;}
        NH_CHECK(Nh_HTML_initBlockRecursively(Content_p, Child_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_setDepthRecursively(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Node_p->Box.depth = Nh_HTML_initZ(Node_p);
 
    for (int i = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
        if (Nh_HTML_isMetaNode(Child_p)) {continue;}
        NH_CHECK(Nh_HTML_setDepthRecursively(Content_p, Child_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_preArrange(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_setDepthRecursively(Document_p->Content_p, Document_p->Tree.Root_p))
    NH_CHECK(Nh_HTML_initBlockRecursively(Document_p->Content_p, Document_p->Tree.Root_p))
    NH_CHECK(Nh_HTML_initInlineRecursively(Document_p->Content_p, Document_p->Tree.Root_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DEPTH ===========================================================================================

static inline float Nh_HTML_initZ(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float depth = 1.0f;
    
    Nh_HTML_Node *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL) 
    {
        depth -= 0.0001f;
        Parent_p = Parent_p->Parent_p;
    }

    if (!Nh_HTML_isInFlow(Node_p)) {
        depth -= 0.1f;
    }

NH_END(depth)
}

// POSITION ========================================================================================

static inline void Nh_HTML_initXPosition(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML) {Node_p->Box.Position.x = 0;}
    else {
        Node_p->Box.Position.x = Nh_HTML_getContentPixelBox(Node_p->Parent_p, 0).Position.x;
    }

NH_SILENT_END()
}

static inline void Nh_HTML_initYPosition(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML) {Node_p->Box.Position.y = 0;}
    else {
        Node_p->Box.Position.y = Nh_HTML_getContentPixelBox(Node_p->Parent_p, 0).Position.y;
    }

NH_SILENT_END()
}

// HEIGHT ==========================================================================================

static NH_PIXEL Nh_HTML_boxModelHeight(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL height = 0;

    height += Node_p->Properties.Padding.top;
    height += Node_p->Properties.Padding.bottom;

    if (Node_p->Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE) {
        height += Node_p->Properties.Border.Width.top;
    }
    if (Node_p->Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE) {
        height += Node_p->Properties.Border.Width.bottom;
    }

    height += Node_p->Properties.Margin.top;
    height += Node_p->Properties.Margin.bottom;

NH_END(height)
}

static inline NH_PIXEL Nh_HTML_initBlockHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL height = 0;

    switch (Node_p->Properties.Position.Height.type)
    {
        case NH_CSS_SIZE_AUTO    : height = Nh_HTML_getContentHeight(Content_p, Node_p); break;
        case NH_CSS_SIZE__LENGTH : height = Node_p->Properties.Position.Height.value; break;
    }

    float minHeight = 0.0f;
    switch (Node_p->Properties.Position.MinHeight.type)
    {
        case NH_CSS_SIZE_AUTO    : minHeight = Nh_HTML_getContentHeight(Content_p, Node_p); break;
        case NH_CSS_SIZE__LENGTH : minHeight = Node_p->Properties.Position.MinHeight.value; break;
    }

    if (height < minHeight) {height = minHeight;}

NH_END(height + Nh_HTML_boxModelHeight(Node_p))
}

// WIDTH ===========================================================================================

static NH_PIXEL Nh_HTML_boxModelWidth(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL width = 0;

    width += Node_p->Properties.Padding.left;
    width += Node_p->Properties.Padding.right;

    if (Node_p->Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE) {
        width += Node_p->Properties.Border.Width.left;
    }
    if (Node_p->Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE) {
        width += Node_p->Properties.Border.Width.right;
    }

    width += Node_p->Properties.Margin.left;
    width += Node_p->Properties.Margin.right;

NH_END(width)
}

static inline NH_PIXEL Nh_HTML_initBlockWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL width = 0;

    switch (Node_p->Properties.Position.Width.type)
    {
        case NH_CSS_SIZE_AUTO        : 
        {
            switch (Node_p->tag) {
               // root element fills entire tab width
               case NH_HTML_TAG_HTML : width = Content_p->Tab_p->Size.width; break;
               // default is full width of parent content
               default : width = Nh_HTML_getContentPixelBox(Node_p->Parent_p, 0).Size.width;
            }
            NH_END(width)
        }
        case NH_CSS_SIZE_MIN_CONTENT : width = Nh_HTML_getContentWidth(Content_p, Node_p); break;
        case NH_CSS_SIZE__LENGTH     : width = Node_p->Properties.Position.Width.value; break;
    }

    NH_PIXEL minWidth = 0;
    switch (Node_p->Properties.Position.MinWidth.type)
    {
        case NH_CSS_SIZE_AUTO    : minWidth = Nh_HTML_getContentWidth(Content_p, Node_p); break;
        case NH_CSS_SIZE__LENGTH : minWidth = Node_p->Properties.Position.MinWidth.value; break;
    }

    if (width < minWidth) {width = minWidth;}

NH_END(width + Nh_HTML_boxModelWidth(Node_p))
}

// PARENTS =========================================================================================

static inline void Nh_HTML_updateParents(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Node *Parent_p = Node_p->Parent_p;

    while (Parent_p != NULL) 
    {
        Nh_HTML_PixelBox ParentContentBox = Nh_HTML_getContentPixelBox(Parent_p, 0);

        if (NH_HTML_MAX_Y(ParentContentBox) < (Node_p->Box.Position.y + Node_p->Box.Size.height)) {
            Parent_p->Box.Size.height += 
                Node_p->Box.Position.y + Node_p->Box.Size.height - NH_HTML_MAX_Y(ParentContentBox);
        }
        if (NH_HTML_MAX_X(ParentContentBox) < (Node_p->Box.Position.x + Node_p->Box.Size.width)) {
            Parent_p->Box.Size.width +=
                Node_p->Box.Position.x + Node_p->Box.Size.width - NH_HTML_MAX_X(ParentContentBox);
        }

        Parent_p = Parent_p->Parent_p;
    }

NH_SILENT_END()
}

// HELPER ==========================================================================================

static inline NH_PIXEL Nh_HTML_getContentWidth(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL width = 0;

    if (Node_p->tag == NH_HTML_TAG_IMG) {
        Nh_Texture *Texture_p = Nh_Gfx_getTextureFromLocation(Content_p, Nh_HTML_getSrc(Node_p), NULL);
        if (Texture_p != NULL) {width += Texture_p->width;}
    }

NH_END(width)
}

static inline NH_PIXEL Nh_HTML_getContentHeight(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_PIXEL height = 0;

    if (Node_p->tag == NH_HTML_TAG_IMG) {
        Nh_Texture *Texture_p = Nh_Gfx_getTextureFromLocation(Content_p, Nh_HTML_getSrc(Node_p), NULL);
        if (Texture_p != NULL) {height += Texture_p->height;}
    }

NH_END(height)
}

