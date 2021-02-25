// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Document.h"
#include "../Header/Parser.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"
#include "../Header/Node.h"

#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/LinkedList.h"
#include "../../../Netzhaut/String.h"
#include "../../../Netzhaut/HashMap.h"

#include "../../../CSS/Header/Associate.h"
#include "../../../CSS/Header/Properties.h"

//#include "../../../JavaScript/Main/Header/Runtime.h"

#include "../../../Graphics/Main/Header/Node.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// INIT ============================================================================================

static NH_RESULT Nh_HTML_initDocument(
    Nh_HTML_Document *Document_p, Nh_Content *Content_p)
{
NH_BEGIN()

    if (Nh_getMediaType(&Content_p->URI) != NH_MEDIA_TEXT_HTML) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    Document_p->Tree.Root_p = NULL;
    Document_p->Selection.update = NH_FALSE;
    Document_p->Content_p = Content_p;

    NH_INIT_LINKED_LIST(Document_p->Sheets)

    NH_CHECK(Nh_HTML_initInput(&Document_p->Input))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// CREATE ===========================================================================================

static NH_RESULT Nh_HTML_startJavaScript(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

//    Nh_DL *JavaScript_p = Nh_openDL("libNetzhautJavaScript.so");
//    NH_CHECK_MEM(JavaScript_p)
//
//    Nh_JS_Interface Interface = {0};
//    Interface.type   = NH_JS_INTERFACE_HTML;
//    Interface.data_p = Document_p;
//    Interface.Mark   = Nh_IO_initInputMark();
//
//    NH_CHECK_NULL(Nh_JS_startJavaScript(&Interface))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_createDocument(
    Nh_HTML_Document *Document_p, Nh_Content *Content_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_initDocument(Document_p, Content_p))
    NH_CHECK(Nh_HTML_parseDocument(Document_p))

    NH_CHECK(Nh_CSS_addSheets(Document_p, NH_TRUE, NH_CONFIG.Flags.Enabled.CSS))
    NH_CHECK(Nh_CSS_parseSheets(&Document_p->Sheets))
    NH_CHECK(Nh_CSS_associateSheets(Document_p, NULL))

    NH_CHECK(Nh_HTML_initTree(Document_p))
    NH_CHECK(Nh_HTML_computeTree(Document_p))
    NH_CHECK(Nh_HTML_computeTreeGraphics(Document_p))

    NH_CHECK(Nh_HTML_startJavaScript(Document_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

NH_RESULT Nh_HTML_destroyDocument(
    Nh_HTML_Document *Document_p)
{   
NH_BEGIN()

    NH_CHECK(Nh_HTML_destroyTree(Document_p))

    Nh_CSS_destroySheetList(&Document_p->Sheets);
    Nh_HTML_freeInput(&Document_p->Input);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

static Nh_PixelSize Nh_HTML_getCropSize(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_PixelSize Size = {0, 0};

    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.Render.data_pp[i];

        if (Node_p->Box.Position.x + Node_p->Box.Size.width > Size.width)   {
            Size.width = Node_p->Box.Position.x + Node_p->Box.Size.width;
        }
        if (Node_p->Box.Position.y + Node_p->Box.Size.height > Size.height) {
            Size.height = Node_p->Box.Position.y + Node_p->Box.Size.height;
        }
    }

NH_END(Size)
}

NH_RESULT Nh_HTML_updateDocument(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

// HTML
    NH_CHECK(Nh_HTML_processInput(Document_p))

// CSS
    NH_BOOL recomputeTree = NH_FALSE;
    NH_CHECK(Nh_CSS_computeProperties(Document_p, &recomputeTree, NH_TRUE))

    if (recomputeTree) {
        exit(0);
//        Nh_CSS_updateInputUnrelatedPseudos(&Document_p->Tree);
//        NH_CHECK(Nh_HTML_resetTreeGraphics(Document_p))
//        NH_CHECK(Nh_HTML_computeTree(Document_p))
    }
    else {Nh_Gfx_updateHTMLDocumentNodes(Document_p);}

// crop
    if (Document_p->Content_p->Tab_p->Flags.crop) 
    { 
        Nh_PixelSize Size = Nh_HTML_getCropSize(Document_p);
        if ((Size.width != 0  && Document_p->Content_p->Tab_p->Size.width  != Size.width) 
        ||  (Size.height != 0 && Document_p->Content_p->Tab_p->Size.height != Size.height)) {
            Document_p->Content_p->Tab_p->Size = Size; 
            NH_CHECK(Nh_resizeTab(Document_p->Content_p->Tab_p))
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// TITLE ===========================================================================================

char *Nh_HTML_getTitle(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    for (int i = 0; i <= Document_p->Tree.All.last; ++i) 
    {
        Nh_HTML_Node *Node_p = Document_p->Tree.All.data_pp[i]; 
        if (Node_p != NULL && Node_p->tag == NH_HTML_TAG_TITLE) {
//            Nh_HTML_Node *Text_p = Nh_getFromLinkedList(&Node_p->Children, 0);
//            if (Text_p != NULL && Text_p->text_p != NULL) {
//                NH_END(Text_p->text_p)
//            }
        }
    }

NH_END(NULL)
}

// BASE ============================================================================================

void Nh_HTML_getBase(
    Nh_HTML_Document *Document_p, char *set_p)
{
NH_BEGIN()

//    for (int i = 0; i < Document_p->Tree.All.count; ++i) 
//    {
//        Nh_HTML_Node *Node_p = Nh_getFromLinkedList(&Document_p->Tree.All, i);
//        
//        if (Node_p->tag == NH_HTML_TAG_BASE) {
//            for (int j = 0; j < Node_p->Attributes.count; ++j) {
//                Nh_HTML_Attribute *Attr_p = Nh_getFromLinkedList(&Node_p->Attributes, j);
//                if (Attr_p->type == NH_HTML_ATTRIBUTE_HREF) {
//                    strcpy(set_p, Attr_p->value_p);
//                    NH_SILENT_END()
//                }
//            }
//        }
//    }
//
//    Nh_stringifyURI(&Document_p->Content_p->URI, set_p);

NH_SILENT_END()
}

