// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TreeConstructionHelper.h"

#include "../Common/Macro.h"
#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include "../../NhDOM/Element.h"
#include "../../NhDOM/Node.h"
#include "../../NhDOM/NodeList.h"
#include "../../NhDOM/Comment.h"
#include "../../NhDOM/Document.h"
#include "../../NhDOM/DocumentType.h"
#include "../../NhDOM/Text.h"
#include "../../NhDOM/Common/Macro.h"
#include NH_DOM_CUSTOM_CHECK2
#include NH_DOM_FLOW2

#include "../../NhCore/Unicode.h"
#include "../../NhWebIDL/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HELPER ==========================================================================================

// https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
Nh_WebIDL_Object *Nh_HTML_createElementForToken(
    Nh_HTML_Token *Token_p, Nh_WebIDL_DOMString *Namespace_p, Nh_WebIDL_Object *IntendedParent_p)
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *Document_p = Nh_DOM_getNodeDocument(Nh_WebIDL_getObject(IntendedParent_p, "DOM", "Node"));
    Nh_WebIDL_DOMString *LocalName_p = &Token_p->StartOrEndTag.TagName;
    Nh_WebIDL_DOMString *Is_p = NULL;

    Nh_WebIDL_Interface *Interface_p = NULL;
    if (Namespace_p == &NH_WEBIDL_HTML_NAMESPACE) {
        Interface_p = Nh_HTML_getElementInterface(LocalName_p->bytes_p);
    }

    Nh_WebIDL_Object *Object_p = Nh_DOM_createElement(
        Document_p, LocalName_p, Namespace_p, NULL, Is_p, NH_FALSE, Interface_p 
    );

NH_HTML_END(Object_p)
}

// https://html.spec.whatwg.org/multipage/parsing.html#appropriate-place-for-inserting-a-node
NH_WEBIDL_UNSIGNED_LONG Nh_HTML_getAppropriatePlaceForInsertingNode(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object **Target_pp)
{
NH_HTML_BEGIN()

    if (*Target_pp == NULL) {*Target_pp = Nh_peekStack(&Parser_p->OpenElements);}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionPosition = 0;

    if (Parser_p->fosterParenting && (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableSectionElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableRowElement"))) {
        puts("TODO foster parenting");
        exit(0);
    }
    else {
        adjustedInsertionPosition = Nh_DOM_getNodeListLength(Nh_WebIDL_get(*Target_pp, "childNodes"));
    }

    if (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTemplateElement")) {
        // TODO
    }

NH_HTML_END(adjustedInsertionPosition)
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-comment
NH_HTML_RESULT Nh_HTML_insertCommentAtPosition(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position)
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *Comment_p = Nh_DOM_createComment(Parser_p->Token_p->CommentOrCharacter.Data);
    NH_HTML_CHECK_MEM(Comment_p)
    NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_insertIntoNode(Nh_WebIDL_getObject(Target_p, "DOM", "Node"), Comment_p, position))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT Nh_HTML_insertComment(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Node_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertCommentAtPosition(Parser_p, Node_p, Nh_HTML_getAppropriatePlaceForInsertingNode(Parser_p, &Node_p)))
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
Nh_WebIDL_Object *Nh_HTML_insertForeignElement(
    Nh_HTML_Parser *Parser_p, Nh_HTML_Token *Token_p, Nh_WebIDL_DOMString *Namespace_p)
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *Target_p = NULL;
    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = Nh_HTML_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    Nh_WebIDL_Object *Element_p = Nh_HTML_createElementForToken(Token_p, Namespace_p, Target_p);
    Nh_DOM_insertIntoNode(Nh_WebIDL_getObject(Target_p, "DOM", "Node"), Element_p, adjustedInsertionLocation);

    Nh_pushStack(&Parser_p->OpenElements, Element_p);

NH_HTML_END(Element_p)
}

Nh_WebIDL_Object *Nh_HTML_insertHTMLElement(
    Nh_HTML_Parser *Parser_p, Nh_HTML_Token Token)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_HTML_insertForeignElement(Parser_p, &Token, &NH_WEBIDL_HTML_NAMESPACE))
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-character
NH_HTML_RESULT Nh_HTML_insertCharacter(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Target_p, Nh_WebIDL_DOMString *Data_p)
{
NH_HTML_BEGIN()

    if (!Data_p) {Data_p = &Parser_p->Token_p->CommentOrCharacter.Data;}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = Nh_HTML_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    if (!strcmp(Target_p->Interface_p->name_p, "Document")) {
        NH_HTML_END(NH_HTML_SUCCESS)
    } 

    if (adjustedInsertionLocation > 0) {
        Nh_WebIDL_Object *Sibling_p = Nh_DOM_getFromNodeList(Nh_WebIDL_get(Target_p, "childNodes"), adjustedInsertionLocation - 1);
        if (!strcmp(Sibling_p->Interface_p->name_p, "Text")) {
            NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToText(Sibling_p, *Data_p))
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }

    Nh_WebIDL_Object *TargetNode_p = Nh_WebIDL_getObject(Target_p, "DOM", "Node");
    NH_HTML_CHECK_NULL(TargetNode_p)

    Nh_WebIDL_Object *Text_p = Nh_DOM_createText(*Data_p, Nh_DOM_getNodeDocument(TargetNode_p)); 
    NH_HTML_CHECK_MEM(Text_p)

    NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_insertIntoNode(TargetNode_p, Text_p, adjustedInsertionLocation))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// https://html.spec.whatwg.org/multipage/parsing.html#generic-rcdata-element-parsing-algorithm
NH_HTML_RESULT Nh_HTML_parseRAWTEXTOrRCDATA(
    Nh_HTML_Parser *Parser_p, Nh_HTML_Token Token, NH_BOOL RAWTEXT)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK_MEM(Nh_HTML_insertHTMLElement(Parser_p, Token))
    Parser_p->Tokenizer_p->state = RAWTEXT ? NH_HTML_TOKENIZATION_STATE_RAWTEXT : NH_HTML_TOKENIZATION_STATE_RCDATA;
    Parser_p->originalInsertionMode = Parser_p->insertionMode;
    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_TEXT;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// https://html.spec.whatwg.org/multipage/parsing.html#reconstruct-the-active-formatting-elements
NH_HTML_RESULT Nh_HTML_reconstructActiveFormattingElements(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->ActiveFormattingElements.size == 0) {
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    // TODO

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

Nh_HTML_Token Nh_HTML_createEmptyStartTagToken(
    NH_HTML_TAG tag)
{
NH_HTML_BEGIN()

    Nh_HTML_Token Token;

    Token.type = NH_HTML_TOKEN_START_TAG;
    Token.StartOrEndTag.TagName.bytes_p = (NH_BYTE*)NH_HTML_TAG_NAMES_PP[tag];
    Token.StartOrEndTag.TagName.length  = strlen(NH_HTML_TAG_NAMES_PP[tag]);
    Token.StartOrEndTag.Attributes      = Nh_initArray(1, 1);
    Token.StartOrEndTag.selfClosing     = NH_FALSE;

NH_HTML_END(Token)
}

// IMPLIED END TAGS ================================================================================

NH_HTML_RESULT Nh_HTML_generateImpliedEndTags(
    Nh_HTML_Parser *Parser_p, NH_BYTE *exclude_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *elements_pp[] = 
    {
        "dd",
        "dt",
        "li",
        "optgroup",
        "option",
        "p",
        "rb",
        "rp",
        "rt",
        "rtc",
    };

    Nh_WebIDL_Object *CurrentNode_p = Nh_HTML_getCurrentNode(Parser_p);

    while (CurrentNode_p)
    {
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(CurrentNode_p, "DOM", "Node"));
        NH_BOOL pop = NH_FALSE;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->bytes_p, elements_pp[i])) {
                if (exclude_p == NULL || strcmp(elements_pp[i], exclude_p)) {
                    Nh_HTML_popCurrentNode(Parser_p);
                    pop = NH_TRUE;
                    break;
                }
            }
        }

        if (!pop) {break;}

        CurrentNode_p = Nh_HTML_getCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT Nh_HTML_generateAllImpliedEndTags(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *elements_pp[] = 
    {
        "caption",
        "colgroup",
        "dd",
        "dt",
        "li",
        "optgroup",
        "option",
        "p",
        "rb",
        "rp",
        "rt",
        "rtc",
        "tbody",
        "td",
        "tfoot",
        "th",
        "thead",
        "tr",
    };

    Nh_WebIDL_Object *CurrentNode_p = Nh_HTML_getCurrentNode(Parser_p);

    while (CurrentNode_p)
    {
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(CurrentNode_p, "DOM", "Node"));
        NH_BOOL pop = NH_FALSE;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->bytes_p, elements_pp[i])) {
                Nh_HTML_popCurrentNode(Parser_p);
                pop = NH_TRUE;
                break;
            }
        }

        if (!pop) {break;}

        CurrentNode_p = Nh_HTML_getCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// STACK OF OPEN ELEMENTS ==========================================================================
// https://html.spec.whatwg.org/multipage/parsing.html#the-stack-of-open-elements

Nh_WebIDL_Object *Nh_HTML_getCurrentNode(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_peekStack(&Parser_p->OpenElements))
}

Nh_WebIDL_Object *Nh_HTML_popCurrentNode(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_popStack(&Parser_p->OpenElements))
}

Nh_WebIDL_Object *Nh_HTML_getAdjustedCurrentNode(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    // TODO

NH_HTML_END(Nh_HTML_getCurrentNode(Parser_p))
}

NH_BOOL Nh_HTML_hasOpenElement(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        Nh_WebIDL_Object *Node_p = Nh_getFromList(&Parser_p->OpenElements, i);
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(Node_p, "DOM", "Element"));
        if (!strcmp(LocalName_p->bytes_p, target_p)) {NH_HTML_END(NH_TRUE)}
    }

NH_HTML_END(NH_FALSE)
}

static NH_BOOL Nh_HTML_hasElementInSpecificScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p, NH_BYTE **scope_pp, unsigned int items)
{
NH_HTML_BEGIN()

    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        Nh_WebIDL_Object *Node_p = Nh_getFromList(&Parser_p->OpenElements, i);
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(Node_p, "DOM", "Element"));
        if (!strcmp(LocalName_p->bytes_p, target_p)) {NH_HTML_END(NH_TRUE)}

        for (int j = 0; j < items; ++j) {
            if (!strcmp(LocalName_p->bytes_p, scope_pp[j])) {
                NH_HTML_END(NH_FALSE)
            }
        }
    }

NH_HTML_END(NH_FALSE)
}

NH_BOOL Nh_HTML_hasElementInScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *defaultScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
    };

NH_HTML_END(Nh_HTML_hasElementInSpecificScope(Parser_p, target_p, defaultScope_pp, sizeof(defaultScope_pp)/sizeof(defaultScope_pp[0])))
}

NH_BOOL Nh_HTML_hasElementInListItemScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *listItemScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
        "ol",
        "ul",
    };

NH_HTML_END(Nh_HTML_hasElementInSpecificScope(Parser_p, target_p, listItemScope_pp, sizeof(listItemScope_pp)/sizeof(listItemScope_pp[0])))
}

NH_BOOL Nh_HTML_hasElementInButtonScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *buttonScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
        "ol",
        "ul",
    };

NH_HTML_END(Nh_HTML_hasElementInSpecificScope(Parser_p, target_p, buttonScope_pp, sizeof(buttonScope_pp)/sizeof(buttonScope_pp[0])))
}

NH_BOOL Nh_HTML_hasElementInTableScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *tableScope_pp[] =
    {
        "html",
        "table",
        "template",
    };

NH_HTML_END(Nh_HTML_hasElementInSpecificScope(Parser_p, target_p, tableScope_pp, sizeof(tableScope_pp)/sizeof(tableScope_pp[0])))
}

NH_BOOL Nh_HTML_hasElementInSelectScope(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *selectScope_pp[] =
    {
        "optgroup",
        "option",
    };

NH_HTML_END(Nh_HTML_hasElementInSpecificScope(Parser_p, target_p, selectScope_pp, sizeof(selectScope_pp)/sizeof(selectScope_pp[0])))
}

NH_BOOL Nh_HTML_hasOtherOpenElements1(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *elements_pp[] = {
        "dd",
        "dt",
        "li",
        "optgroup",
        "option",
        "p",
        "rb",
        "rp",
        "rt",
        "rtc",
        "tbody",
        "td",
        "tfoot",
        "th",
        "thead",
        "tr",
        "body",
        "html",
    };

    for (int i = 0; i < Parser_p->OpenElements.size; ++i) 
    {
        Nh_WebIDL_Object *Node_p = Nh_getFromList(&Parser_p->OpenElements, i);
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(Node_p, "DOM", "Element"));

        NH_BOOL match = NH_FALSE;
        for (int j = 0; j < sizeof(elements_pp)/sizeof(elements_pp[0]); ++j) {
            if (strcmp(LocalName_p->bytes_p, elements_pp[j])) {match = NH_TRUE; break;}
        }

        if (!match) {NH_HTML_END(NH_TRUE)}
    }

NH_HTML_END(NH_FALSE)
}

