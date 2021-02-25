#ifndef NH_CSS_UTILS_H
#define NH_CSS_UTILS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Common/API.h"

#endif

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct Nh_CSS_Generic {
        int type;
        NH_PIXEL value;
        void *extra_p;
    } Nh_CSS_Generic;

    typedef struct Nh_CSS_PixelCorners {
        NH_PIXEL topLeft;
        NH_PIXEL topRight;
        NH_PIXEL bottomRight;
        NH_PIXEL bottomLeft;
    } Nh_CSS_PixelCorners;
    
    typedef struct Nh_CSS_Corners {
        float topLeft;
        float topRight;
        float bottomRight;
        float bottomLeft;
    } Nh_CSS_Corners;

    typedef struct Nh_CSS_PixelSides {
        NH_PIXEL top;
        NH_PIXEL right;
        NH_PIXEL bottom;
        NH_PIXEL left; 
    } Nh_CSS_PixelSides;

    typedef struct Nh_CSS_Sides {
        float top;
        float right;
        float bottom;
        float left; 
    } Nh_CSS_Sides;

    typedef struct Nh_CSS_Types {
        int top;
        int right;
        int bottom;
        int left; 
    } Nh_CSS_Types;
   
    typedef struct Nh_CSS_SidesColor {
        float top_p[4];
        float right_p[4];
        float bottom_p[4];
        float left_p[4]; 
    } Nh_CSS_SidesColor;

/** @} */

#endif
