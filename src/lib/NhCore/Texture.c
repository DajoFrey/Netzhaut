// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"
#include "Memory.h"
#include "Tab.h"
#include "LinkedList.h"
#include "Config.h"
#include "Font.h"

#include "../NhGraphics/Vulkan/Header/Texture.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// DECLARE =========================================================================================

static Nh_Texture *Nh_allocateTexture(
    Nh_Content *Content_p, Nh_URI URI
);

static Nh_Texture *Nh_softLoadTexture(
    Nh_Texture *Texture_p
);
static void Nh_softFreeTexture(
    Nh_Texture *Texture_p
);

// INIT ============================================================================================

NH_RESULT Nh_initDefaultTextures(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    Nh_Texture *Dummy_p = Nh_initTexture(Content_p, NULL, NULL);
//    NH_CHECK_NULL(Dummy_p)
//     
//    switch (Content_p->Tab_p->GPU.API)
//    {
//        case NH_API_VULKAN : NH_CHECK(Nh_Vk_hardLoadTexture(Content_p->Tab_p->GPU.Pointer, Dummy_p)) break;
//        case NH_API_DIRECTX : break;
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

Nh_Texture *Nh_initTexture(
    Nh_Content *Content_p, Nh_URI *URI_p, char *location_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK_NULL(NULL, Content_p)

    Nh_URI URI = URI_p == NULL ? Nh_createURI(location_p, NULL, NULL, -1) : Nh_copyURI(URI_p);

    Nh_Texture *Texture_p = Nh_getTexture(Content_p, &URI);
    if (Texture_p != NULL) {
        Nh_freeURI(&URI); 
        NH_END(Texture_p)
    }

NH_END(Nh_softLoadTexture(Nh_allocateTexture(Content_p, URI)))
}

static Nh_Texture *Nh_allocateTexture(
    Nh_Content *Content_p, Nh_URI URI)
{
NH_BEGIN()

    Nh_Texture *Texture_p = Nh_allocate(sizeof(Nh_Texture));
    NH_CHECK_NULL(NULL, Texture_p)
    NH_CHECK(NULL, Nh_appendToLinkedList(&Content_p->Textures, Texture_p))

    Texture_p->gamma    = NH_CONFIG.Settings.gamma;
    Texture_p->pixels_p = NULL;
    Texture_p->data_p   = NULL;
    Texture_p->Vulkan_p = NULL;
    Texture_p->URI      = URI;
    Texture_p->width    = 0;
    Texture_p->height   = 0;

NH_END(Texture_p)
}

// SOFT ============================================================================================

static Nh_Texture *Nh_softLoadTexture(
    Nh_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(NULL, Texture_p)

    // catch already loaded texture
    if (Texture_p->pixels_p != NULL || Texture_p->data_p != NULL) {NH_END(NULL)}

    NH_MEDIA mediaType = Nh_getMediaType(&Texture_p->URI);

    switch (mediaType)
    {
        case NH_MEDIA_IMAGE_JPEG :
        case NH_MEDIA_IMAGE_PNG  :
        {
            char *data_p = Nh_getPayload(&Texture_p->URI);
            Texture_p->pixels_p = stbi_load_from_memory(data_p, Texture_p->URI.size, &Texture_p->width, &Texture_p->height, &Texture_p->compression, STBI_rgb_alpha);
            Nh_freePayload(&Texture_p->URI);
            NH_CHECK_NULL(NULL, Texture_p->pixels_p)

            break;
        }
        case NH_MEDIA_FONT_TTF : 
        case NH_MEDIA_FONT_OTF :
        { 
            Nh_Font *Font_p = Nh_getFontFromURI(&Texture_p->URI);
            NH_CHECK(NULL, Nh_loadFont(Font_p))

            Texture_p->width    = Font_p->Atlas.width;
            Texture_p->height   = Font_p->Atlas.height;
            Texture_p->pixels_p = Font_p->Atlas.data_p;
            NH_CHECK_NULL(NULL, Texture_p->pixels_p)

            break;
        }
        case -1 :
        {
            Texture_p->data_p = Nh_allocate(sizeof(uint32_t) * 10000);
            NH_CHECK_NULL(NULL, Texture_p->data_p)
            for (int i = 0; i < 10000; ++i) {((uint32_t*)Texture_p->data_p)[i] = 255;}

            Texture_p->width  = 100;
            Texture_p->height = 100;
 
            break;
        }
    }

#include NH_DEFAULT_CHECK

NH_END(Texture_p)
}

static void Nh_softFreeTexture(
    Nh_Texture *Texture_p)
{
NH_BEGIN()

    if (Texture_p == NULL) {NH_SILENT_END()}

    NH_MEDIA mediaType = Nh_getMediaType(&Texture_p->URI);
   
    switch (mediaType)
    {
        case NH_MEDIA_IMAGE_JPEG :
        case NH_MEDIA_IMAGE_PNG  :

            stbi_image_free(Texture_p->pixels_p);
            break;

        case NH_MEDIA_FONT_TTF : 
        case NH_MEDIA_FONT_OTF :
            
            Texture_p->pixels_p = NULL;
            Texture_p->data_p = NULL;
            break;

        case -1 : 
        
            Nh_free(Texture_p->data_p);
            break;
    }

NH_SILENT_END()
}

// DELETE ==========================================================================================

NH_RESULT Nh_deleteTextures(
    Nh_Content *Content_p)
{
NH_BEGIN()

    while (Content_p->Textures.count > 0) {
        NH_CHECK(Nh_deleteTexture(Content_p, Nh_getFromLinkedList(&Content_p->Textures, 0)))
    }

    Nh_destroyLinkedList(&Content_p->Textures, true);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_deleteTexture(
    Nh_Content *Content_p, Nh_Texture *Texture_p)
{
NH_BEGIN()

//    NH_CHECK_NULL(Content_p)
//    NH_CHECK_NULL(Texture_p)
//
//    int index = -1;
//    for (int i = 0; i < Content_p->Textures.count; ++i) {
//        if (Texture_p == Nh_getFromLinkedList(&Content_p->Textures, i)) {index = i; break;}
//    }
//
//    if (index == -1) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}
//
//    if (Texture_p->Vulkan_p != NULL) {Nh_Vk_destroyTexture(Content_p->Tab_p->GPU.Pointer, Texture_p->Vulkan_p);}
//
//    Nh_softFreeTexture(Texture_p);
//    Nh_freeURI(&Texture_p->URI);
//    Nh_removeFromLinkedList(&Content_p->Textures, index, true);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_updateTexture(
    Nh_Content *Content_p, Nh_Texture *Texture_p)
{
NH_BEGIN()

//    if (Content_p == NULL || Texture_p == NULL) {NH_END(NH_SUCCESS)}
//
//    Nh_softFreeTexture(Texture_p);
//    NH_CHECK_NULL(Nh_softLoadTexture(Texture_p))
//
//    if (Texture_p->Vulkan_p != NULL) {
//        NH_CHECK(Nh_Vk_hardReloadTexture(Content_p->Tab_p->GPU.Pointer, Texture_p))
//    }

NH_END(NH_SUCCESS)
}

// CHECK ===========================================================================================

NH_BOOL Nh_textureLoaded(
    Nh_Content *Content_p, char *location_p, char *base_p)
{
NH_BEGIN()

    Nh_Texture *Texture_p = Nh_getTextureFromLocation(Content_p, location_p, base_p);

NH_END(Texture_p != NULL && Texture_p->pixels_p != NULL)
}

// GET =============================================================================================

Nh_Texture *Nh_getTextureFromLocation(
    Nh_Content *Content_p, char *location_p, char *base_p)
{
NH_BEGIN()

    for (int i = 0; i < Content_p->Textures.count; ++i) 
    {
        Nh_Texture *Texture_p = Nh_getFromLinkedList(&Content_p->Textures, i);
        if (Nh_correspondingURI(&Texture_p->URI, location_p, base_p)) {NH_END(Texture_p)}
    }

NH_END(NULL)
}

Nh_Texture *Nh_getTexture(
    Nh_Content *Content_p, Nh_URI *URI_p)
{
NH_BEGIN()

    if (URI_p == NULL) {NH_END(Nh_getFromLinkedList(&Content_p->Textures, 0))}

    for (int i = 0; i < Content_p->Textures.count; ++i) 
    {
        Nh_Texture *Texture_p = Nh_getFromLinkedList(&Content_p->Textures, i);
        if (Nh_equivalentURI(URI_p, &Texture_p->URI)) {NH_END(Texture_p)}
    }

NH_END(NULL)
}

float Nh_getTextureWidth(
    Nh_Content *Content_p, Nh_Texture *Texture_p)
{
NH_BEGIN()

    float width = (((float)Texture_p->width) / (float)Content_p->Tab_p->Size.width) * 2.0f;

NH_END(width)
}

float Nh_getTextureHeight(
    Nh_Content *Content_p, Nh_Texture *Texture_p)
{
NH_BEGIN()

    float height = (((float)Texture_p->height) / (float)Content_p->Tab_p->Size.height) * 2.0f;

NH_END(height)
}

//void Nh_getTextureScale(
//    Nh_Content *Content_p, Nh_HTML_PixelBox *Box_p, Nh_Texture *Texture_p, float scale_p[2])
//{
//NH_BEGIN()
//    
////    scale_p[0] = ((float)Nh_getTextureWidth(Content_p, Texture_p)) / (float)Box_p->Size.width;
////    scale_p[1] = ((float)Nh_getTextureHeight(Content_p, Texture_p)) / (float)Box_p->Size.height;
//
//NH_SILENT_END()
//}
//
//void Nh_getScaledTextureRepeat(
//    Nh_Content *Content_p, Nh_HTML_PixelBox *Box_p, Nh_Texture *Texture_p, float repeat_p[2])
//{
//NH_BEGIN()
//    
//    float scale_p[2];
//    Nh_getTextureScale(Content_p, Box_p, Texture_p, scale_p);
//
//    if (scale_p[1] > scale_p[0]) {
//        repeat_p[0] = scale_p[1];
//        repeat_p[1] = 1.0f;
//    }
//    else {
//        repeat_p[0] = 1.0f;
//        repeat_p[1] = scale_p[0];
//    }
//
//NH_SILENT_END()
//}

