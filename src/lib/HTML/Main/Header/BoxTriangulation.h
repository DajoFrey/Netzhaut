#ifndef NH_HTML_BOX_TRIANGULATION_H
#define NH_HTML_BOX_TRIANGULATION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Box.h"

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    int Nh_HTML_triangulateBox(
        Nh_HTML_ClipBox EnclosingBox, Nh_HTML_ClipBox EnclosedBox, NH_PIXEL radii_p[4], int cornerTriangleCount, 
        int side, Nh_Triangle *Triangles_p, bool enclosedPoint, int offset, float z
    );

/** @} */

#endif
