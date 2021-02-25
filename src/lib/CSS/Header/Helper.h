#ifndef NH_CSS_HELPER_H
#define NH_CSS_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../HTML/Main/Header/Document.h"

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_STRIP {
        NH_CSS_STRIP_URL,
    } NH_CSS_STRIP;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_PIXEL Nh_CSS_getLengthInPixel(
        Nh_HTML_Node *Node_p, Nh_Content *Content_p, char *str_p
    );
    
    void Nh_CSS_strip(
        NH_CSS_STRIP type, char *in_p, char *out_p
    );

/** @} */

#endif 
