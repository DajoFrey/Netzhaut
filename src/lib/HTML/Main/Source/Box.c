// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Box.h"
#include "../Header/Macros.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../HTML/Main/Header/Node.h"

#include NH_FLOW
#include NH_CSS_UTILS
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>
#include <math.h>

// BACKGROUND IMAGE BOX ============================================================================

Nh_HTML_PixelBox Nh_HTML_getBackgroundImageBox(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p, float subtractFromZ)
{
NH_BEGIN()

    Nh_HTML_PixelBox Box = {0};

    switch (Image_p->Background.origin)
    {
        case NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX : Box = Nh_HTML_getPaddingPixelBox(Node_p, 0); break;
        case NH_CSS_BACKGROUND_ORIGIN_BORDER_BOX  : Box = Nh_HTML_getBorderPixelBox(Node_p, 0); break;
        case NH_CSS_BACKGROUND_ORIGIN_CONTENT_BOX : Box = Nh_HTML_getContentPixelBox(Node_p, 0); break;
    }

    NH_PIXEL boxWidth  = Box.Size.width;
    NH_PIXEL boxHeight = Box.Size.height;

    // size
    NH_PIXEL imageWidth  = Image_p->Texture_p->width;
    NH_PIXEL imageHeight = Image_p->Texture_p->height;

//    if (Image_p->Background.Size_p[0].type == NH_CSS_BACKGROUND_SIZE_PERCENT) {
//        imageWidth = (Image_p->Background.Size_p[0].value / 100.0f) * boxWidth; 
//    }
//    if (Image_p->Background.Size_p[1].type == NH_CSS_BACKGROUND_SIZE_PERCENT) {
//        imageHeight = (Image_p->Background.Size_p[1].value / 100.0f) * boxHeight; 
//    }
    if (Image_p->Background.Size_p[0].type == NH_CSS_BACKGROUND_SIZE_PIXEL) {
        imageWidth = Image_p->Background.Size_p[0].value; 
    }
    if (Image_p->Background.Size_p[1].type == NH_CSS_BACKGROUND_SIZE_PIXEL) {
        imageHeight = Image_p->Background.Size_p[1].value; 
    }

    // position
    NH_PIXEL positionX = Box.Position.x;
    NH_PIXEL positionY = Box.Position.y;

    NH_PIXEL emptyWidth  = boxWidth - imageWidth;
    NH_PIXEL emptyHeight = boxHeight - imageHeight;

    if (Image_p->Background.Position_p[0].type == NH_CSS_BACKGROUND_POSITION_PERCENT && emptyWidth > 0) {
        positionX += (NH_PIXEL)(((float)(Image_p->Background.Position_p[0].value / 100.0f)) * (float)emptyWidth);
    }
    else {
        positionX += Image_p->Background.Position_p[0].value;
    }

    if (Image_p->Background.Position_p[1].type == NH_CSS_BACKGROUND_POSITION_PERCENT && emptyHeight > 0) {
        positionY += (NH_PIXEL)(((float)(Image_p->Background.Position_p[1].value / 100.0f)) * (float)emptyHeight);
    }
    else {
        positionY += Image_p->Background.Position_p[1].value;
    }

    Box.Position.x  = positionX;
    Box.Size.width  = imageWidth;
    Box.Position.y  = positionY;
    Box.Size.height = imageHeight;

    Box.depth -= subtractFromZ;

NH_END(Box)
}

// CLIP BOX ========================================================================================

Nh_HTML_ClipBox Nh_HTML_convertToClipBox(
    Nh_Content *Content_p, Nh_HTML_PixelBox PixelBox)
{
NH_BEGIN()

    Nh_HTML_ClipBox ClipBox = {0};

    ClipBox.Position.x = NH_PIXEL_TO_CLIP(PixelBox.Position.x, Content_p, NH_TRUE);
    ClipBox.Position.y = NH_PIXEL_TO_CLIP(PixelBox.Position.y, Content_p, NH_FALSE);

    ClipBox.Size.width  = NH_CLIP_LENGTH(NH_PIXEL_TO_CLIP(PixelBox.Size.width, Content_p, NH_TRUE));
    ClipBox.Size.height = NH_CLIP_LENGTH(NH_PIXEL_TO_CLIP(PixelBox.Size.height, Content_p, NH_FALSE));

    ClipBox.depth = PixelBox.depth;

NH_END(ClipBox)
}

// PIXEL BOX =======================================================================================

Nh_HTML_PixelBox Nh_HTML_getContentPixelBox(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()

    Nh_HTML_PixelBox Box = Nh_HTML_getPaddingPixelBox(Node_p, index); 

    Box.Position.x  += Node_p->Properties.Padding.left;
    Box.Position.y  += Node_p->Properties.Padding.top;
    Box.Size.width  -= Node_p->Properties.Padding.right + Node_p->Properties.Padding.left;
    Box.Size.height -= Node_p->Properties.Padding.bottom + Node_p->Properties.Padding.top;

NH_END(Box)
}

Nh_HTML_PixelBox Nh_HTML_getPaddingPixelBox(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()

    Nh_HTML_PixelBox Box = Nh_HTML_getBorderPixelBox(Node_p, index);

    if (Node_p->Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE) {
        Box.Position.x += Node_p->Properties.Border.Width.left;
    }
    if (Node_p->Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE) {
        Box.Position.y += Node_p->Properties.Border.Width.top;
    }
    if (Node_p->Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE) {
        Box.Size.width -= Node_p->Properties.Border.Width.right + Node_p->Properties.Border.Width.left;
    }
    if (Node_p->Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE) {
        Box.Size.height -= Node_p->Properties.Border.Width.bottom + Node_p->Properties.Border.Width.top;
    }

NH_END(Box)
}

Nh_HTML_PixelBox Nh_HTML_getBorderPixelBox(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()

    Nh_HTML_PixelBox *MarginBox_p = Nh_HTML_getMarginPixelBox(Node_p, index);
    Nh_HTML_PixelBox MarginBox = *MarginBox_p;

    MarginBox.Position.x  += Node_p->Properties.Margin.left;
    MarginBox.Position.y  += Node_p->Properties.Margin.top;

    MarginBox.Size.width  -= Node_p->Properties.Margin.right + Node_p->Properties.Margin.left;
    MarginBox.Size.height -= Node_p->Properties.Margin.bottom + Node_p->Properties.Margin.top;

NH_END(MarginBox)
}

Nh_HTML_PixelBox *Nh_HTML_getMarginPixelBox(
    Nh_HTML_Node *Node_p, int index)
{
NH_BEGIN()

    Nh_HTML_PixelBox **Next_pp = &Node_p->Box.Next_p;
    Nh_HTML_PixelBox *Previous_p = &Node_p->Box;

    int count = index < 0 ? index - 1 : 0;
    while (*Next_pp != NULL && count < index) {
        Previous_p = *Next_pp; 
        Next_pp = &(*Next_pp)->Next_p;
        if (index >= 0) {++count;}
    }

NH_END(Previous_p)
}

