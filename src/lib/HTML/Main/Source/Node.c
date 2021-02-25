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

#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Tab.h"
#include "../../../Netzhaut/LinkedList.h"
#include "../../../Netzhaut/HashMap.h"
#include "../../../Netzhaut/String.h"

#include "../../../CSS/Header/Associate.h"
#include "../../../CSS/Header/Log.h"
#include "../../../CSS/Header/Properties.h"

#include "../../../Graphics/Main/Header/Node.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// DATA ============================================================================================

const char *NH_HTML_TAGS_PP[] = 
{
    "a",           
    "abbr",       
    "acronym",    
    "address",    
    "applet",     
    "area",       
    "article",    
    "aside",      
    "audio",      
    "b",          
    "base",       
    "basefont",   
    "bdi",        
    "bdo",        
    "big",        
    "blockquote", 
    "body",       
    "br",         
    "button",     
    "canvas",     
    "caption",    
    "center",     
    "cite",       
    "code",       
    "col",        
    "colgroup",   
    "data",       
    "datalist",   
    "dd",         
    "del",        
    "details",    
    "dfn",        
    "dialog",     
    "dir",        
    "div",        
    "dl",         
    "dt",         
    "em",         
    "embed",      
    "fieldset",   
    "figcaption", 
    "figure",     
    "font",       
    "footer",     
    "form",       
    "frame",      
    "frameset",   
    "h1",         
    "h2",         
    "h3",         
    "h4",         
    "h5",         
    "h6",         
    "head",       
    "header",      
    "hr",          
    "html",       
    "i",          
    "iframe",     
    "img",       
    "input",       
    "ins",        
    "kbd",        
    "label",      
    "legend",     
    "li",       
    "link",     
    "main",        
    "map",        
    "mark",       
    "meta",       
    "meter",      
    "nav",        
    "noframes",   
    "noscript",   
    "object",     
    "ol",         
    "optgroup",   
    "option",  
    "output",  
    "p",       
    "param",       
    "picture",    
    "pre",        
    "progress",   
    "q",          
    "rp",         
    "rt",         
    "ruby",       
    "s",          
    "samp",       
    "script",     
    "section",    
    "select", 
    "small",      
    "source",     
    "span",       
    "strike",     
    "strong",     
    "style",      
    "sub",        
    "summary",    
    "sup",        
    "svg",        
    "table",      
    "tbody",      
    "td",         
    "template",   
    "textarea",   
    "tfoot",      
    "th",         
    "thead",      
    "time",       
    "title",      
    "tr",         
    "track",      
    "tt",         
    "u",          
    "ul",    
    "var",         
    "video",      
    "wbr",        
    "menu",
    "hgroup",
    "math",
    "text",
    "char",
    "undefined",
};

size_t NH_HTML_TAGS_PP_COUNT = sizeof(NH_HTML_TAGS_PP) / sizeof(NH_HTML_TAGS_PP[0]);

// INIT ============================================================================================

NH_RESULT Nh_HTML_initNode(
    Nh_HTML_Node *Node_p, Nh_HTML_Node *Parent_p, NH_HTML_TAG tag)
{
NH_BEGIN()

    if (tag < 0) {NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)}

    Node_p->tag = tag;

    NH_INIT_LINKED_LIST(Node_p->Attributes)

    NH_INIT_LINKED_LIST(Node_p->Vulkan.Buffers)
    NH_INIT_LINKED_LIST(Node_p->Vulkan.Uniforms)
    NH_INIT_LINKED_LIST(Node_p->Vulkan.Descriptors)

    Nh_initList(&Node_p->Children, 128);

    Node_p->Parent_p = Parent_p;
    Node_p->text_p   = NULL;

    Node_p->Box.Position.x = 0;
    Node_p->Box.Position.y = 0;

    Node_p->Box.Size.width  = 0;
    Node_p->Box.Size.height = 0;

    Node_p->Box.depth  = 1.0f;
    Node_p->Box.Next_p = NULL;

    NH_CHECK(Nh_CSS_initProperties(Node_p))
    NH_CHECK(Nh_CSS_initPseudo(&Node_p->Pseudo))

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// COMPUTE =========================================================================================

static NH_RESULT Nh_HTML_computeCharNode(
    Nh_HTML_Node *Char_p)
{
NH_BEGIN()

    Nh_loadFont(Char_p->Parent_p->Properties.Text.Font_p);

    Nh_Glyph Glyph = Nh_getGlyph(Char_p->Parent_p->Properties.Text.Font_p, Char_p->text_p, Char_p->Parent_p->Properties.Text.fontSize);

    Char_p->Box.Size.width  = *Char_p->text_p == ' ' ? Glyph.xAdvance : Glyph.width;  
    Char_p->Box.Size.height = Glyph.height;  

    Char_p->Properties.Padding.top    = ((int)Char_p->Parent_p->Properties.Text.Font_p->ascender) - Glyph.yOffset;
    Char_p->Properties.Padding.bottom = abs(((int)Char_p->Parent_p->Properties.Text.Font_p->descender)) - (Glyph.height - Glyph.yOffset);
    Char_p->Properties.Padding.right  = Glyph.xOffset;
    Char_p->Properties.Padding.left   = 0;

    Char_p->Properties.Border.Style.left   = NH_CSS_BORDER_STYLE_NONE;
    Char_p->Properties.Border.Style.right  = NH_CSS_BORDER_STYLE_NONE;
    Char_p->Properties.Border.Style.bottom = NH_CSS_BORDER_STYLE_NONE;
    Char_p->Properties.Border.Style.top    = NH_CSS_BORDER_STYLE_NONE;

    Char_p->Properties.Margin.left   = 0;
    Char_p->Properties.Margin.right  = 0;
    Char_p->Properties.Margin.bottom = 0;
    Char_p->Properties.Margin.top    = 0;

    Char_p->Box.Size.width  += Char_p->Properties.Padding.right;  
    Char_p->Box.Size.height += Char_p->Properties.Padding.top + Char_p->Properties.Padding.bottom;

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_loadNodeResources(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    switch (Node_p->tag)
    {
        case NH_HTML_TAG_IMG :
        {
            char *src_p = Nh_HTML_getSrc(Node_p);
            if (src_p != NULL) {NH_CHECK_NULL(Nh_Gfx_initTexture(Document_p->Content_p, NULL, src_p))}
            break;
        }
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_computeNode(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p) 
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_CHAR) {NH_CHECK(Nh_HTML_computeCharNode(Node_p))}
    else {
        NH_CHECK(Nh_CSS_computeNodeProperties(Document_p->Content_p, Node_p, NULL, NH_FALSE))
        NH_CHECK(Nh_HTML_loadNodeResources(Document_p, Node_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// MANIPULATE ======================================================================================

NH_RESULT Nh_HTML_replaceChild(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Replace_p, Nh_HTML_Node *Replacement_p)
{
NH_BEGIN()

//    NH_CHECK(Nh_HTML_resetTreeGraphics(Document_p))
//
//    NH_CHECK(Nh_replaceInLinkedList(&Replace_p->Parent_p->Children, Replace_p, Replacement_p))
//    Nh_HTML_destroyNode(Replace_p, NH_TRUE);
//
//    NH_CHECK(Nh_CSS_associateSheets(Document_p, Replacement_p))
//    NH_CHECK(Nh_HTML_computeNode(Document_p, Replacement_p))
//
//    NH_CHECK(Nh_HTML_computeTree(Document_p))
//
//    if (Document_p->Scripts.count > 0) {
//        NH_CHECK(Nh_JS_updateHTMLDocumentObject(Nh_getFromLinkedList(&Document_p->Scripts, 0)))
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_replaceChildren(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Replace_p)
{
NH_BEGIN()

//    NH_CHECK(Nh_HTML_resetTreeGraphics(Document_p))
//
//    for (int i = 0; i < Node_p->Children.count; ++i) {
//        Nh_HTML_destroyNode(Nh_getFromLinkedList(&Node_p->Children, i), NH_TRUE);
//    }
//
//    Nh_destroyLinkedList(&Node_p->Children, true);
//    NH_CHECK(Nh_appendToLinkedList(&Node_p->Children, Replace_p))
//
//    NH_CHECK(Nh_CSS_associateSheets(Document_p, Replace_p))
//    NH_CHECK(Nh_HTML_computeNode(Document_p, Replace_p))
//
//    NH_CHECK(Nh_HTML_computeTree(Document_p))
//
//    if (Document_p->Scripts.count > 0) {
//        NH_CHECK(Nh_JS_updateHTMLDocumentObject(Nh_getFromLinkedList(&Document_p->Scripts, 0)))
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_prependChild(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Prepend_p)
{
NH_BEGIN()

//    NH_CHECK(Nh_HTML_resetTreeGraphics(Document_p))
//
//    NH_CHECK(Nh_prependToLinkedList(&Node_p->Children, Prepend_p))
//
//    NH_CHECK(Nh_CSS_associateSheets(Document_p, Prepend_p))
//    NH_CHECK(Nh_HTML_computeNode(Document_p, Prepend_p))
//
//    NH_CHECK(Nh_HTML_computeTree(Document_p))
//
//    if (Document_p->Scripts.count > 0) {
//        NH_CHECK(Nh_JS_updateHTMLDocumentObject(Nh_getFromLinkedList(&Document_p->Scripts, 0)))
//    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_HTML_destroyNode(
    Nh_HTML_Node *Node_p, NH_BOOL destroyTextData)
{
NH_BEGIN()

    Nh_HTML_destroyAttributes(&Node_p->Attributes);
    Nh_CSS_destroyProperties(&Node_p->Properties);

    if (destroyTextData && Node_p->tag != NH_HTML_TAG_CHAR) {
        Nh_free(Node_p->text_p);
    }

    for (int i = 0; i <= Node_p->Children.last; ++i) {
        Nh_HTML_destroyNode(Node_p->Children.data_pp[i], destroyTextData);
    }

    Nh_freeList(&Node_p->Children, NH_TRUE);

NH_SILENT_END()
}

// GET NODE ========================================================================================

static Nh_HTML_Node *Nh_HTML_getNodeRecursively(
    Nh_HTML_Node *Node_p, NH_HTML_TAG tag)
{
NH_BEGIN()

    if (Node_p->tag == tag) {NH_END(Node_p)}
    else {
        for (int i = 0; i <= Node_p->Children.last; ++i) {
            Nh_HTML_Node *Child_p = Nh_HTML_getNodeRecursively(Node_p->Children.data_pp[i], tag);
            if (Child_p != NULL) {NH_END(Child_p)}
        }
    }

NH_END(NULL)
}

Nh_HTML_Node *Nh_HTML_getNode(
    Nh_HTML_Document *Document_p, NH_HTML_TAG tag)
{
NH_BEGIN()
NH_END(Nh_HTML_getNodeRecursively(Document_p->Tree.Root_p, tag))
}

// IS? =============================================================================================

NH_BOOL Nh_HTML_isMetaNode(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    while (Node_p != NULL) {
        if (NH_HTML_METADATA(Node_p->tag)) {NH_DIAGNOSTIC_END(NH_TRUE)}
        Node_p = Node_p->Parent_p;
    }

NH_END(NH_FALSE)
}

NH_BOOL Nh_HTML_isVisibleTextNode(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()
NH_END(Node_p != NULL && Node_p->text_p != NULL && Node_p->tag == NH_HTML_TAG_TEXT && !Nh_HTML_isMetaNode(Node_p) && Node_p->Children.last != -1)
}

NH_BOOL Nh_HTML_isInFlow(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_BOOL flow = Node_p->Properties.Position.position != NH_CSS_POSITION_ABSOLUTE && Node_p->Properties.Position.position != NH_CSS_POSITION_FIXED;
    if (!flow) {NH_DIAGNOSTIC_END(NH_FALSE)}

    Nh_HTML_Node *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL) {
        flow = Parent_p->Properties.Position.position != NH_CSS_POSITION_ABSOLUTE && Parent_p->Properties.Position.position != NH_CSS_POSITION_FIXED;
        if (!flow) {NH_DIAGNOSTIC_END(NH_FALSE)}
        Parent_p = Parent_p->Parent_p;
    }

NH_DIAGNOSTIC_END(NH_TRUE)
}

// GET TEXT ========================================================================================

static NH_RESULT Nh_HTML_getTextRecursively(
    Nh_HTML_Node *Node_p, Nh_String *String_p)
{
NH_BEGIN()

    for (int i = 0; i <= Node_p->Children.last; ++i)  
    {
        Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
        if (Nh_HTML_isVisibleTextNode(Child_p)) {NH_CHECK(Nh_appendFormatToString(String_p, Child_p->text_p))}
        NH_CHECK(Nh_HTML_getTextRecursively(Child_p, String_p))
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

char *Nh_HTML_getText(
    Nh_HTML_Node *Node_p, NH_BOOL recursive)
{
NH_BEGIN()

    char *result_p = NULL;

    Nh_String *String_p = Nh_allocateString(NULL);

    if (!recursive) {    
        for (int i = 0; i <= Node_p->Children.last; ++i) {
            Nh_HTML_Node *Child_p = Node_p->Children.data_pp[i];
            if (Nh_HTML_isVisibleTextNode(Child_p)) {
                if (Nh_appendFormatToString(String_p, Child_p->text_p) != NH_SUCCESS) {NH_END(NULL)}
            }
        }
    }
    else if (Nh_HTML_getTextRecursively(Node_p, String_p) != NH_SUCCESS) {NH_END(NULL)}

    result_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);
   
NH_END(result_p)
}

