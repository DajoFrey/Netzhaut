#ifndef NH_TEXTURE_H
#define NH_TEXTURE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Window.h"
#include "Tab.h"
#include "Content.h"
#include "URI.h"

#include "Common/Result.h"
#include "External/stb_image.h"

typedef struct Nh_HTML_PixelBox Nh_HTML_PixelBox;
typedef struct Nh_Vk_Texture Nh_Vk_Texture;

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_Texture {
        Nh_URI URI;    
        Nh_Vk_Texture *Vulkan_p; 
        stbi_uc *pixels_p;         
        NH_PIXEL width;                 
        NH_PIXEL height;                
        int compression;           
        float gamma;
        void *data_p;               
    } Nh_Texture;
    
/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initDefaultTextures(
        Nh_Content *Content_p
    );

    Nh_Texture *Nh_initTexture(
        Nh_Content *Content_p, Nh_URI *URI_p, char *chars_p
    );
  
    NH_RESULT Nh_deleteTextures(
       Nh_Content *Content_p
    );
   
    NH_RESULT Nh_deleteTexture(
        Nh_Content *Content_p, Nh_Texture *Texture_p
    );
   
    NH_RESULT Nh_updateTexture(
        Nh_Content *Content_p, Nh_Texture *Texture_p
    );

    NH_BOOL Nh_textureLoaded(
        Nh_Content *Content_p, char *location_p, char *base_p
    );

    Nh_Texture *Nh_getTextureFromLocation(
        Nh_Content *Content_p, char *location_p, char *base_p
    );

    Nh_Texture *Nh_getTexture(
        Nh_Content *Content_p, Nh_URI *URI_p 
    );
    
    float Nh_getTextureWidth(
        Nh_Content *Content_p, Nh_Texture *Texture_p
    );
    
    float Nh_getTextureHeight(
        Nh_Content *Content_p, Nh_Texture *Texture_p
    );

    void Nh_getTextureScale(
        Nh_Content *Content_p, Nh_HTML_PixelBox *Box_p, Nh_Texture *Texture_p, float scale_p[2]
    );

    void Nh_getScaledTextureRepeat(
        Nh_Content *Content_p, Nh_HTML_PixelBox *Box_p, Nh_Texture *Texture_p, float repeat_p[2]
    );

/** @} */

#endif
