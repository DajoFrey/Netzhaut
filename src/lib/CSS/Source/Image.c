// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Image.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/Memory.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// IMAGE LIST ======================================================================================

NH_RESULT Nh_CSS_addImage(
    Nh_LinkedList *Images_p, Nh_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Images_p)

    NH_CSS_Image *Image_p = Nh_allocate(sizeof(NH_CSS_Image));
    NH_CHECK_MEM(Image_p)

    Image_p->buffer     = -1;
    Image_p->descriptor = -1;
    Image_p->uniform    = -1;
    Image_p->Texture_p  = Texture_p;

    Nh_CSS_setBackgroundSize(NULL, NULL, NULL, Image_p);
    Nh_CSS_setBackgroundOrigin(NULL, NULL, Image_p);
    Nh_CSS_setBackgroundPosition(NULL, NULL, NULL, Image_p);

    Nh_appendToLinkedList(Images_p, Image_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_CSS_Image *Nh_CSS_getImage(
    Nh_LinkedList *Images_p, int index)
{
NH_BEGIN()
NH_END(Nh_getFromLinkedList(Images_p, index))
}

void Nh_CSS_destroyImages(
    Nh_LinkedList *Images_p)
{
NH_BEGIN()

    Nh_destroyLinkedList(Images_p, true);
    
NH_SILENT_END()
}

