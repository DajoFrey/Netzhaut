// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Arrange.h"

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

// CHARS ===========================================================================================

static void Nh_HTML_moveSiblingRecursively(
    Nh_HTML_Node *Node_p, Nh_HTML_Node *Sibling_p)
{
NH_BEGIN()

    Sibling_p->Box.Position.x = NH_HTML_MAX_X((*Nh_HTML_getMarginPixelBox(Node_p, -1)));
    Sibling_p->Box.Position.y = Nh_HTML_getMarginPixelBox(Node_p, -1)->Position.y;

    for (int i = 0; i <= Sibling_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Deeper_p = Sibling_p->Children.data_pp[i];
        if (Deeper_p->tag == NH_HTML_TAG_CHAR) {break;}
        Nh_HTML_moveSiblingRecursively(Node_p, Deeper_p);
    }

NH_SILENT_END()
}

static NH_RESULT Nh_HTML_moveSiblings(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    int i = 0;
    for (i = 0; i <= Node_p->Parent_p->Children.last && Node_p->Parent_p->Children.data_pp[i] != Node_p; ++i) {}

    Nh_HTML_Node *Sibling_p = Node_p->Parent_p->Children.data_pp[++i];
    while (i <= Node_p->Parent_p->Children.last) 
    {
        if (Sibling_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE) {
            Nh_HTML_moveSiblingRecursively(Node_p, Sibling_p);
            Sibling_p = Node_p->Parent_p->Children.data_pp[++i];
        }
        else {
            Sibling_p->Box.Position.y = Nh_HTML_getMarginPixelBox(Node_p, -1)->Position.y;
            break;
        }
    }

NH_END(NH_SUCCESS)
}

static NH_BOOL Nh_HTML_newLine(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Node *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL && Parent_p->Properties.Position.display != NH_CSS_DISPLAY_BLOCK) {
        Parent_p = Parent_p->Parent_p;
    }
    if (Parent_p == NULL) {NH_END(NH_FALSE)}
    Nh_HTML_PixelBox EnclosingBlock = Nh_HTML_getContentPixelBox(Parent_p, 0);

NH_END(Node_p->Box.Position.x + Node_p->Box.Size.width > EnclosingBlock.Position.x + EnclosingBlock.Size.width)
}

static NH_RESULT Nh_HTML_arrangeInlineChar(
    Nh_HTML_Node *Char_p)
{
NH_BEGIN()

    Nh_HTML_PixelBox *MarginBox_p = Nh_HTML_getMarginPixelBox(Char_p->Parent_p, -1);
    Nh_HTML_PixelBox ContentBox   = Nh_HTML_getContentPixelBox(Char_p->Parent_p, -1);

    Nh_Glyph Glyph = Nh_getGlyph(
        Char_p->Parent_p->Properties.Text.Font_p, Char_p->text_p, Char_p->Parent_p->Properties.Text.fontSize
    );

    Char_p->Box.Position.x = ContentBox.Position.x + ContentBox.Size.width;
    Char_p->Box.Position.y = ContentBox.Position.y;

    if (Nh_HTML_newLine(Char_p)) 
    {
        MarginBox_p->Next_p = Nh_allocate(sizeof(Nh_HTML_PixelBox));
        NH_CHECK_MEM(MarginBox_p->Next_p)

        Nh_HTML_Node *Parent_p = Char_p->Parent_p;
        while (Parent_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE) {
            Parent_p = Parent_p->Parent_p;
        }
        Nh_HTML_PixelBox EnclosingBox = Nh_HTML_getContentPixelBox(Parent_p, 0);

        MarginBox_p->Next_p->Next_p = NULL;
        MarginBox_p->Next_p->depth = MarginBox_p->depth;

        MarginBox_p->Next_p->Size.height = 0; 
        MarginBox_p->Next_p->Size.width  = 0; 

        MarginBox_p->Next_p->Position.y = MarginBox_p->Position.y + MarginBox_p->Size.height;
        MarginBox_p->Next_p->Position.x = EnclosingBox.Position.x; 

        ContentBox = Nh_HTML_getContentPixelBox(Char_p->Parent_p, -1);

        Char_p->Box.Position.x = ContentBox.Position.x;
        Char_p->Box.Position.y = ContentBox.Position.y;

        MarginBox_p->Next_p->Size.width += *Char_p->text_p == ' ' ? Char_p->Box.Size.width : Glyph.xAdvance;
        if (Char_p->Box.Size.height > MarginBox_p->Next_p->Size.height) {
            MarginBox_p->Next_p->Size.height = Char_p->Box.Size.height;
        }

        NH_END(NH_SUCCESS)
    }

    MarginBox_p->Size.width += *Char_p->text_p == ' ' ? Char_p->Box.Size.width : Glyph.xAdvance;
    if (Char_p->Box.Size.height > MarginBox_p->Size.height) {
        MarginBox_p->Size.height = Char_p->Box.Size.height;
    }

NH_END(NH_SUCCESS)
}

// ARRANGE =========================================================================================

static NH_RESULT Nh_HTML_arrangeRecursively(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_CHAR) {NH_END(Nh_HTML_arrangeInlineChar(Node_p))}

    Nh_HTML_PixelBox *MarginBox_p = NULL;
    float depth = Node_p->Box.depth;

    for (int i = 0; i <= Node_p->Children.last; ++i) 
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
        if (Child_p->Properties.Position.display == NH_CSS_DISPLAY_NONE) {continue;}
        Nh_HTML_arrangeRecursively(Child_p);

        if (Node_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE
        &&  Node_p->tag != NH_HTML_TAG_TEXT) 
        {
            Nh_HTML_PixelBox *ChildMarginBox_p = &Child_p->Box;
            while (ChildMarginBox_p != NULL) 
            {
                if (MarginBox_p == NULL) {
                    MarginBox_p = &Node_p->Box;
                }
                else {
                    MarginBox_p->Next_p = Nh_allocate(sizeof(Nh_HTML_PixelBox));
                    NH_CHECK_MEM(MarginBox_p->Next_p)
                    MarginBox_p = MarginBox_p->Next_p;
                }
                memcpy(MarginBox_p, ChildMarginBox_p, sizeof(Nh_HTML_PixelBox));
                MarginBox_p->depth = depth;
                ChildMarginBox_p = ChildMarginBox_p->Next_p;
            }
        }
    }

    if (Node_p->Properties.Position.display == NH_CSS_DISPLAY_INLINE) {
        Nh_HTML_moveSiblings(Node_p);
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_arrange(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_HTML_arrangeRecursively(Document_p->Tree.Render.data_pp[0]);

//    Nh_HTML_Node *Node_p = Nh_HTML_getNode(Document_p, NH_HTML_TAG_HTML);
//    if (Node_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
//
//    Document_p->Content_p->Size.width  = NH_CLIP_TO_PIXEL(Node_p->Box.Size.width - 1.0f, Document_p->Content_p, NH_TRUE);
//    Document_p->Content_p->Size.height = NH_CLIP_TO_PIXEL(Node_p->Box.Size.height - 1.0f, Document_p->Content_p, NH_FALSE);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

