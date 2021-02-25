#ifndef NH_HTML_VERTEX_H
#define NH_HTML_VERTEX_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Document.h"

#include "../../../Netzhaut/Netzhaut.h"

#include "../../../CSS/Header/Image.h"
#include "../../../CSS/Header/Border.h"

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    int Nh_HTML_getBackgroundVertices(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, float *vertices_p, int cornerTriangleCount
    );
    
    NH_RESULT Nh_HTML_getBackgroundImageVertices(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, float vertices_p[30], NH_CSS_Image *Image_p,
        float subtractFromZ
    );
    
    int Nh_HTML_getBorderVertices(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, float *vertices_p, NH_CSS_BORDER type,
        int cornerTriangleCount
    );
    
    NH_RESULT Nh_HTML_getImageVertices(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, float vertices_p[30], float subtractFromZ
    );

    NH_RESULT Nh_HTML_getTextVertices(
        Nh_Content *Content_p, Nh_HTML_Node *Node_p, float **vertices_pp, uint32_t **indices_pp
    );

/** @} */

#endif 
