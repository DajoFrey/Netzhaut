#ifndef NH_HTML_BOX_H
#define NH_HTML_BOX_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../CSS/Header/Image.h"
#include "../../../NhCore/Math.h"

typedef struct Nh_HTML_Node Nh_HTML_Node;
typedef struct Nh_HTML_Document Nh_HTML_Document;

#endif

/** @addtogroup HTMLStructs Structs
 *  \ingroup HTML
 *  @{
 */

    typedef struct Nh_HTML_PixelBox {
        Nh_PixelPosition Position;
        Nh_PixelSize Size;
        float depth;
        struct Nh_HTML_PixelBox *Next_p;
    } Nh_HTML_PixelBox;

    typedef struct Nh_HTML_ClipBox {
        Nh_ClipPosition Position;
        Nh_ClipSize Size;
        float depth;
    } Nh_HTML_ClipBox;

/** @} */

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    Nh_HTML_PixelBox Nh_HTML_getBackgroundImageBox(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p, float subtractFromZ
    );

    Nh_HTML_PixelBox Nh_HTML_getContentPixelBox(
        Nh_HTML_Node *Node_p, int index
    );

    Nh_HTML_PixelBox Nh_HTML_getBorderPixelBox(
        Nh_HTML_Node *Node_p, int index
    );
       
    Nh_HTML_PixelBox Nh_HTML_getPaddingPixelBox(
        Nh_HTML_Node *Node_p, int index
    );

    Nh_HTML_PixelBox *Nh_HTML_getMarginPixelBox(
        Nh_HTML_Node *Node_p, int index
    );

    Nh_HTML_ClipBox Nh_HTML_convertToClipBox(
        Nh_Content *Content_p, Nh_HTML_PixelBox PixelBox
    );

/** @} */

#endif
