// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Content.h"
#include "Sync.h"
#include "Time.h"
#include "Memory.h"
#include "Callbacks.h"
#include "String.h"

//#include "../../IO/Mouse.h"
//#include "../../IO/Keyboard.h"
//
//#include "../../HTML/Main/Header/Document.h"

#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>

// INIT ============================================================================================

static NH_RESULT Nh_initContent(
    Nh_Content *Content_p, char *URI_p)
{
NH_BEGIN()

//    Content_p->type   = NH_CONTENT_UNDEFINED;
//    Content_p->data_p = NULL; 
//
//    Nh_URI URI = Nh_createURI(URI_p, NULL, NULL, -1);
//
//    switch (Nh_getMediaType(&URI))
//    {
//        case NH_MEDIA_TEXT_HTML :
//        {
//            Content_p->type   = NH_CONTENT_HTML_DOCUMENT;
//            Content_p->data_p = Nh_allocate(sizeof(Nh_HTML_Document));
//            NH_CHECK_MEM(Content_p->data_p)
//            break;
//        }
//    }
//
//    NH_INIT_LINKED_LIST(Content_p->Callbacks)
//    NH_INIT_LINKED_LIST(Content_p->Textures)
//
//    Content_p->URI         = URI;
//    Content_p->Tab_p       = NULL;
//    Content_p->Size.width  = 0;
//    Content_p->Size.height = 0;
//    Content_p->Position.x  = 0;
//    Content_p->Position.y  = 0;
//
//    Content_p->Flags.visible = NH_FALSE;
//
//    Content_p->Settings.scale = 1.0f;
//
//    NH_CHECK(Nh_IO_initInput(&Content_p->Input))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// CREATE/TRANSITION ===============================================================================

static NH_RESULT Nh_createContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    NH_CHECK_NULL(Content_p)
//    NH_CHECK_NULL(Content_p->Tab_p)
//
//    NH_CHECK(Nh_Gfx_initDefaultTextures(Content_p))
//
//    switch (Content_p->type) 
//    {
//        case NH_CONTENT_HTML_DOCUMENT :
//        {
//            NH_CHECK(Nh_HTML_createDocument(Content_p->data_p, Content_p))
//            break;
//        }
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_transitionContent(
    Nh_Content *Content_p, Nh_Tab *New_p)
{
NH_BEGIN()

    puts("transition content");
    exit(0);

//    if (Content_p->Tab_p == New_p) {NH_DIAGNOSTIC_END(NH_SUCCESS)}
//
//    if (Content_p->Tab_p->GPU.Pointer == New_p->Window_p->GPU.Pointer) {
//        NH_CHECK(Nh_removeContent(Content_p))
//        NH_CHECK(Nh_addContent(Content_p, New_p))
//    }
//    else {
//        puts("transition content");
//        exit(0);
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_openContent(
    Nh_Tab *Tab_p, char *URI_p)
{
NH_BEGIN()

    Nh_Content *Content_p = Nh_allocate(sizeof(Nh_Content));
    NH_CHECK_MEM(Content_p)

    NH_CHECK(Nh_initContent(Content_p, URI_p))
    NH_CHECK(Nh_addContent(Content_p, Tab_p))
    NH_CHECK(Nh_createContent(Content_p))
    NH_CHECK(Nh_showContent(Content_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// CLOSE ===========================================================================================

NH_RESULT Nh_destroyContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    Nh_IO_freeInput(&Content_p->Input);
//    Nh_deleteCallbacks(&Content_p->Callbacks);
//
//    if (Content_p->Tab_p != NULL) 
//    {
//        switch (Content_p->type)
//        {
//            case NH_CONTENT_HTML_DOCUMENT : 
//            {
//                NH_CHECK(Nh_HTML_destroyDocument(Content_p->data_p))
//                break;
//            }
//        }
//
//        NH_CHECK(Nh_Gfx_deleteTextures(Content_p))
//    }
//
//    NH_CHECK(Nh_removeContent(Content_p))
//    Nh_free(Content_p);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_destroyContents(
    Nh_Tab *Tab_p)
{
NH_BEGIN()
 
    for (int i = 0; i < Tab_p->Contents.count; ++i) {
        NH_CHECK(Nh_destroyContent(Nh_getFromLinkedList(&Tab_p->Contents, i)))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_updateContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    switch (Content_p->type)
//    {
//        case NH_CONTENT_HTML_DOCUMENT :
//        {
//            NH_CHECK(Nh_HTML_updateDocument(Content_p->data_p))
//            break;
//        }
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_resizeContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    switch (Content_p->type)
//    {
//        case NH_CONTENT_HTML_DOCUMENT :
//        {
//            NH_CHECK(Nh_HTML_resetTreeGraphics(Content_p->data_p))
//            NH_CHECK(Nh_HTML_computeTree(Content_p->data_p))
//            break;
//        }
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_showContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Content_p) 
    NH_CHECK_NULL(Content_p->Tab_p)

    for (int i = 0; i < Content_p->Tab_p->Contents.count; ++i) {
        Nh_Content *Tmp_p = Nh_getFromLinkedList(&Content_p->Tab_p->Contents, i);
        Tmp_p->Flags.visible = NH_FALSE;
    }

    Content_p->Flags.visible = NH_TRUE;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_Content *Nh_getVisibleContent(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Contents.count; ++i) {
        Nh_Content *Content_p = Nh_getFromLinkedList(&Tab_p->Contents, i);
        if (Content_p->Flags.visible) {NH_END(Content_p)}
    }

NH_END(NULL)
}

char *Nh_getTitle(
    Nh_Content *Content_p)
{
NH_BEGIN()

//    switch (Content_p->type)
//    {
//        case NH_CONTENT_HTML_DOCUMENT :
//        {
//            NH_END(Nh_HTML_getTitle(Content_p->data_p))
//            break;
//        }
//    }

NH_END(NULL)
}

Nh_Content *Nh_getContent(
    Nh_Tab *Tab_p, int index)
{
NH_BEGIN()
NH_END(Nh_getFromLinkedList(&Tab_p->Contents, index))
}

// ADD/REMOVE ======================================================================================

NH_RESULT Nh_addContent(
    Nh_Content *Content_p, Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Content_p)
    NH_CHECK_NULL(Tab_p)

    if (Content_p->Tab_p != NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    for (int i = 0; i < Tab_p->Contents.count; ++i) {
        if (Nh_getFromLinkedList(&Tab_p->Contents, i) == Content_p) {
            NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
            break;
        }
    }

    NH_CHECK(Nh_appendToLinkedList(&Tab_p->Contents, Content_p))
    Content_p->Tab_p = Tab_p;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_removeContent(
    Nh_Content *Content_p)
{
NH_BEGIN()

    if (Content_p->Tab_p == NULL) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    for (int i = 0; i < Content_p->Tab_p->Contents.count; ++i) {
        if (Nh_getFromLinkedList(&Content_p->Tab_p->Contents, i) == Content_p) {
            Nh_removeFromLinkedList(&Content_p->Tab_p->Contents, i, false);
            break;
        }
    }

    Content_p->Tab_p = NULL;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

