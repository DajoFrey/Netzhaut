#ifndef NH_CSS_IMAGE_H
#define NH_CSS_IMAGE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Background.h"

#include "../../NhCore/Netzhaut.h"
#include "../../NhCore/Texture.h"

#endif

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Image {

        Nh_Texture *Texture_p;
        int buffer, descriptor, uniform;

        struct {
            NH_CSS_BackgroundSize Size_p[2];
            NH_CSS_BackgroundPosition Position_p[2];
            NH_CSS_BACKGROUND_ORIGIN origin;        
            NH_CSS_BACKGROUND_REPEAT repeat;
        } Background;

    } NH_CSS_Image;
    
/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_addImage(
        Nh_LinkedList *Images_p, Nh_Texture *Texture_p
    );
   
    NH_CSS_Image *Nh_CSS_getImage(
        Nh_LinkedList *Images_p, int index
    );
    
    void Nh_CSS_destroyImages(
        Nh_LinkedList *Images_p
    );

/** @} */

#endif 
