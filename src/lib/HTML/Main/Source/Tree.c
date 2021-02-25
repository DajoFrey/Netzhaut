// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Tree.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"
#include "../Header/Document.h"

#include "../../Arrange/Header/PreArrange.h"
#include "../../Arrange/Header/Arrange.h"
#include "../../Arrange/Header/PostArrange.h"

#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/String.h"
#include "../../../Netzhaut/Memory.h"
#include "../../../Graphics/Main/Header/Node.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// LISTS ===========================================================================================

static NH_RESULT Nh_HTML_createRenderArrays(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (!Nh_HTML_isMetaNode(Node_p) && Node_p->Properties.Position.display != NH_CSS_DISPLAY_NONE) {
        NH_CHECK(Nh_setListData(&Tree_p->Render, Node_p, -1))
        if (Node_p->tag != NH_HTML_TAG_CHAR) {
            NH_CHECK(Nh_setListData(&Tree_p->RenderNoChars, Node_p, -1))
        }
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_HTML_createRenderArrays(Tree_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_createAllArray(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CHECK(Nh_setListData(&Tree_p->All, Node_p, -1))
    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_HTML_createAllArray(Tree_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_recreateAllArray(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_freeList(&Tree_p->All, NH_FALSE);
    NH_CHECK(Nh_HTML_createAllArray(Tree_p, Node_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_recreateRenderArrays(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_freeList(&Tree_p->Render, NH_FALSE);
    Nh_freeList(&Tree_p->RenderNoChars, NH_FALSE);

    NH_CHECK(Nh_HTML_createRenderArrays(Tree_p, Node_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_recreateTreeArrays(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_recreateAllArray(Tree_p, Node_p))
    NH_CHECK(Nh_HTML_recreateRenderArrays(Tree_p, Node_p))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// INIT ============================================================================================

static NH_RESULT Nh_HTML_computeNodes(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_computeNode(Document_p, Node_p))
    for (int i = 0; i <= Node_p->Children.last; ++i) {
        NH_CHECK(Nh_HTML_computeNodes(Document_p, Node_p->Children.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_initTree(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_initList(&Document_p->Tree.All, 1024);
    Nh_initList(&Document_p->Tree.Render, 1024);
    Nh_initList(&Document_p->Tree.RenderNoChars, 1024);

    NH_CHECK(Nh_HTML_computeNodes(Document_p, Document_p->Tree.Root_p))
    NH_CHECK(Nh_HTML_recreateTreeArrays(&Document_p->Tree, Document_p->Tree.Root_p))

NH_END(NH_SUCCESS)
}

// COMPUTE =========================================================================================

NH_RESULT Nh_HTML_computeTreeGraphics(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    for (int i = 0; i <= Document_p->Tree.Render.last; ++i) {
        NH_CHECK(Nh_Gfx_createNode(Document_p->Content_p, Document_p->Tree.Render.data_pp[i]))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_resetTreeGraphics(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    for (int i = 0; i < Document_p->Tree.Render.last; ++i) {
        Nh_Gfx_destroyNode(
            &Document_p->Content_p->Tab_p->GPU, Document_p->Tree.Render.data_pp[i]
        );
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_computeTree(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_preArrange(Document_p))
    NH_CHECK(Nh_HTML_preLog(&Document_p->Tree))

    NH_CHECK(Nh_HTML_arrange(Document_p))

    NH_CHECK(Nh_HTML_postArrange(Document_p))
    NH_CHECK(Nh_HTML_postLog(&Document_p->Tree))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_resetTree(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    // TODO

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

NH_RESULT Nh_HTML_destroyTree(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Nh_HTML_destroyNode(Document_p->Tree.Root_p, NH_TRUE);
    Nh_free(Document_p->Tree.Root_p);

    Nh_freeList(&Document_p->Tree.Render, NH_FALSE);
    Nh_freeList(&Document_p->Tree.RenderNoChars, NH_FALSE);
    Nh_freeList(&Document_p->Tree.All, NH_FALSE);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// STRINGIFY =======================================================================================

static NH_RESULT Nh_HTML_stringifyElement(
    Nh_HTML_Node *Node_p, Nh_StringConstructor *String_p, int depth, int *number_p)
{
NH_BEGIN()

#define INDENT() for (int ind = 0; ind < depth; ++ind) {NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "  "))}

    if (Node_p->tag != NH_HTML_TAG_CHAR) {
        INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "\e[1;32m%s\e[0m #%d {\n", NH_HTML_TAGS_PP[Node_p->tag], *number_p))
    }
    else {
        char str_p[2] = {'\0'};
        str_p[0] = *Node_p->text_p;
        INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "\e[1;32m%s\e[0m #%d { \e[1;36m%s\e[0m ", NH_HTML_TAGS_PP[Node_p->tag], *number_p, str_p))
    }

    if (Node_p->text_p != NULL && Node_p->tag != NH_HTML_TAG_CHAR) 
    {
        char *text_p = Node_p->text_p;
        char *part_p = text_p;
        while (*text_p != '\0') 
        {
            text_p = text_p + 1;
            if (*text_p == '\0' || *text_p == '\n') 
            {
                char reverse = *text_p;
                *text_p = '\0';
                INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "  TEXT_LINE(%s)\n", part_p))
                part_p = text_p + 1;
                *text_p = reverse;
            }
        }
    }

    for (int i = 0; i < Node_p->Attributes.count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_getFromLinkedList(&Node_p->Attributes, i);
        INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "  ATTRIBUTE(\e[1;34m%s\e[0m: %s)\n", NH_HTML_ATTRIBUTES_PP[Attribute_p->type], Attribute_p->value_p))
    }

    if (!Nh_HTML_isMetaNode(Node_p)) {
        if (Node_p->tag == NH_HTML_TAG_CHAR) {
            NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "MARGIN_BOX(\e[1;33mx: %.5dpx, y: %.5dpx, width: %.5dpx, height: %.5dpx, depth: %f\e[0m) }\n", Node_p->Box.Position.x, Node_p->Box.Position.y, Node_p->Box.Size.width, Node_p->Box.Size.height, Node_p->Box.depth))
        }
        else {
            Nh_HTML_PixelBox *Box_p = &Node_p->Box;
            while (Box_p != NULL) {
                INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "  MARGIN_BOX(\e[1;33mx: %.5dpx, y: %.5dpx, width: %.5dpx, height: %.5dpx, depth: %f\e[0m)\n", Box_p->Position.x, Box_p->Position.y, Box_p->Size.width, Box_p->Size.height, Box_p->depth))
                Box_p = Box_p->Next_p;
            }
        }
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        *number_p += 1;
        NH_CHECK(Nh_HTML_stringifyElement(Node_p->Children.data_pp[i], String_p, depth + 1, number_p))
    }
        
    if (Node_p->tag != NH_HTML_TAG_CHAR) {INDENT() NH_CHECK(Nh_appendFormatToStringConstructor(String_p, "}\n"))}

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

char *Nh_HTML_stringifyTree(
    Nh_HTML_Tree *Tree_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_StringConstructor *String_p = Nh_allocateStringConstructor(1024);

    int number = 0;
    NH_CHECK(NULL, Nh_HTML_stringifyElement(Tree_p->Root_p, String_p, 0, &number))
    char *chars_p = Nh_getChars2(String_p);

    Nh_freeStringConstructor(String_p);

#include NH_DEFAULT_CHECK

NH_END(chars_p)
}

