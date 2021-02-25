// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Background.h"
#include "../Header/Helper.h"
#include "../Header/Color.h"
#include "../Header/Macros.h"
#include "../Header/Image.h"

#include NH_FLOW
#include NH_CSS_UTILS

#include "../../Netzhaut/Tab.h"
#include "../../HTML/Main/Header/Document.h"

#include <string.h>
#include <ctype.h>

// SET =============================================================================================

void Nh_CSS_setBackgroundColor(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    for (int i = 0; i < 4; ++i) {Node_p->Properties.Background.color_p[i] = 0.0f;}
    if (Node_p->tag == NH_HTML_TAG_BODY) {
        for (int i = 0; i < 4; ++i) {Node_p->Properties.Background.color_p[i] = 1.0f;}
    }

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Properties.Background.color_p);
    }

NH_SILENT_END()
}

inline static void Nh_CSS_setBackgroundImage(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Nh_CSS_destroyImages(&Node_p->Properties.Background.Images);
    NH_INIT_LINKED_LIST(Node_p->Properties.Background.Images)

    if (Property_p != NULL) 
    {
        for (int i = 0; i < Property_p->valueCount; ++i)
        {
            if (strstr(Property_p->values_pp[i], "url(")) {
                char url_p[512] = {'\0'};
                Nh_CSS_strip(NH_CSS_STRIP_URL, Property_p->values_pp[i], url_p);
                Nh_CSS_addImage(&Node_p->Properties.Background.Images, Nh_Gfx_initTexture(Content_p, NULL, url_p));
            }
        }
    }

NH_SILENT_END()
}

void Nh_CSS_setBackgroundSize(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    Image_p->Background.Size_p[0].type  = NH_CSS_BACKGROUND_SIZE_AUTO;
    Image_p->Background.Size_p[0].value = 0.0f;
    Image_p->Background.Size_p[1].type  = NH_CSS_BACKGROUND_SIZE_AUTO;
    Image_p->Background.Size_p[1].value = 0.0f;

    if (Property_p != NULL && Property_p->valueCount == 1) 
    {
        if (strstr(Property_p->values_pp[0], "contain")) {
            Image_p->Background.Size_p[0].type = NH_CSS_BACKGROUND_SIZE_CONTAIN;
            Image_p->Background.Size_p[1].type = NH_CSS_BACKGROUND_SIZE_CONTAIN;
        }
        else if (strstr(Property_p->values_pp[0], "cover")) {
            Image_p->Background.Size_p[0].type = NH_CSS_BACKGROUND_SIZE_COVER;
            Image_p->Background.Size_p[1].type = NH_CSS_BACKGROUND_SIZE_COVER;
        }
        else if (strstr(Property_p->values_pp[0], "%")) {
        }
        else if (strstr(Property_p->values_pp[0], "px")) 
        {
            Image_p->Background.Size_p[0].type = NH_CSS_BACKGROUND_SIZE_PIXEL;
            Image_p->Background.Size_p[1].type = NH_CSS_BACKGROUND_SIZE_PIXEL;
       
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);

            Image_p->Background.Size_p[0].value = NH_CLIP_LENGTH(NH_PIXEL_TO_CLIP(pixel, Content_p, NH_TRUE));
            Image_p->Background.Size_p[1].value = Image_p->Background.Size_p[0].value;
        }
    } 
    else if (Property_p != NULL && Property_p->valueCount == 2) 
    {
        for (int i = 0; i < 2; ++i) 
        {
            if (strstr(Property_p->values_pp[i], "%")) {
                Image_p->Background.Size_p[i].type = NH_CSS_BACKGROUND_SIZE_PERCENT;
                Image_p->Background.Size_p[i].value = strtof(Property_p->values_pp[i], NULL);
            }
            else if (strstr(Property_p->values_pp[i], "px")) {
            }
        }
    }

NH_SILENT_END()
}

void Nh_CSS_setBackgroundOrigin(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    Image_p->Background.origin = NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX;

    if (Property_p != NULL && Property_p->valueCount == 1) {
             if (strstr(Property_p->values_pp[0], "border-box"))  {Image_p->Background.origin = NH_CSS_BACKGROUND_ORIGIN_BORDER_BOX;}
        else if (strstr(Property_p->values_pp[0], "padding-box")) {Image_p->Background.origin = NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX;}
        else if (strstr(Property_p->values_pp[0], "content-box")) {Image_p->Background.origin = NH_CSS_BACKGROUND_ORIGIN_CONTENT_BOX;}
    } 

NH_SILENT_END()
}

void Nh_CSS_setBackgroundPosition(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p, NH_CSS_Image *Image_p)
{
NH_BEGIN()

    Image_p->Background.Position_p[0].type  = NH_CSS_BACKGROUND_POSITION_PERCENT;
    Image_p->Background.Position_p[0].value = 0.0f;
    Image_p->Background.Position_p[1].type  = NH_CSS_BACKGROUND_POSITION_PERCENT;
    Image_p->Background.Position_p[1].value = 0.0f;

    if (Property_p != NULL && (Property_p->valueCount == 1 || Property_p->valueCount == 2)) 
    {
        if (strstr(Property_p->values_pp[0], "center")) {
            Image_p->Background.Position_p[0].value = 50.0f;
            Image_p->Background.Position_p[1].value = 50.0f;
        }
        if (strstr(Property_p->values_pp[0], "top")) {
            Image_p->Background.Position_p[0].value = 50.0f;
            Image_p->Background.Position_p[1].value = 0.0f;
        }
        if (strstr(Property_p->values_pp[0], "left")) {
            Image_p->Background.Position_p[0].value = 0.0f;
            Image_p->Background.Position_p[1].value = 50.0f;
        }
        if (strstr(Property_p->values_pp[0], "bottom")) {
            Image_p->Background.Position_p[0].value = 50.0f;
            Image_p->Background.Position_p[1].value = 100.0f;
        }
        if (strstr(Property_p->values_pp[0], "right")) {
            Image_p->Background.Position_p[0].value = 100.0f;
            Image_p->Background.Position_p[1].value = 50.0f;
        }
        if (strstr(Property_p->values_pp[0], "px")) {
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Content_p, Property_p->values_pp[0]);
            Image_p->Background.Position_p[0].type = NH_CSS_BACKGROUND_POSITION_LENGTH;
            Image_p->Background.Position_p[0].value =
                NH_CLIP_LENGTH(NH_PIXEL_TO_CLIP(pixel, Content_p, NH_FALSE));
            Image_p->Background.Position_p[1].value = 50.0f;
        }
    } 
    if (Property_p != NULL && Property_p->valueCount == 2)
    {
        if (strstr(Property_p->values_pp[1], "center")) {
        }
        if (strstr(Property_p->values_pp[1], "top")) {
            Image_p->Background.Position_p[1].value = 0.0f;
        }
        if (strstr(Property_p->values_pp[1], "left")) {
            Image_p->Background.Position_p[0].value = 0.0f;
        }
        if (strstr(Property_p->values_pp[1], "bottom")) {
            Image_p->Background.Position_p[1].value = 100.0f;
        }
        if (strstr(Property_p->values_pp[1], "right")) {
            Image_p->Background.Position_p[0].value = 100.0f;
        }
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeBackgroundProperties(
    Nh_Content *Content_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_CSS_setBackgroundColor(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BACKGROUND_COLOR]);
    Nh_CSS_setBackgroundImage(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BACKGROUND_IMAGE]);
    
    for (int i = 0; i < Node_p->Properties.Background.Images.count; ++i) 
    {
        NH_CSS_Image *Image_p = Nh_CSS_getImage(&Node_p->Properties.Background.Images, i);

        Nh_CSS_setBackgroundSize(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BACKGROUND_SIZE], Image_p);
        Nh_CSS_setBackgroundOrigin(Node_p, Properties_pp[NH_CSS_PROPERTY_BACKGROUND_ORIGIN], Image_p);
        Nh_CSS_setBackgroundPosition(Content_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BACKGROUND_POSITION], Image_p);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

