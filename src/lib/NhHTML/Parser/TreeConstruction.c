// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TreeConstruction.h"
#include "Elements.h"

#include "../Common/Macro.h"
#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include "../../NhDOM/Element.h"
#include "../../NhDOM/Node.h"
#include "../../NhDOM/Comment.h"
#include "../../NhDOM/Document.h"
#include "../../NhDOM/DocumentType.h"
#include "../../NhDOM/Common/Macro.h"
#include NH_DOM_CUSTOM_CHECK2
#include NH_DOM_FLOW2

#include "../../NhCore/Unicode.h"
#include "../../NhWebIDL/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_HTML_RESULT Nh_HTML_reprocessToken(
    Nh_HTML_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode
);

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
static NH_WEBIDL_UNSIGNED_LONG Nh_HTML_getAppropriatePlaceForInsertingNode(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object **Target_pp)
{
NH_HTML_BEGIN()

    if (*Target_pp == NULL) {*Target_pp = Nh_peekStack(&Parser_p->OpenElements);}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionPosition = 0;

    if (Parser_p->fosterParenting) {
        puts("TODO foster parenting");
        exit(0);
    }
    else {
        adjustedInsertionPosition = *((NH_WEBIDL_UNSIGNED_LONG*)Nh_WebIDL_get(Nh_WebIDL_get(*Target_pp, "childNodes"), "length"));
    }

NH_HTML_END(adjustedInsertionPosition)
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-comment
static NH_HTML_RESULT Nh_HTML_insertCommentAtPosition(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Node_p, NH_WEBIDL_UNSIGNED_LONG position)
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *Comment_p = Nh_DOM_createComment(Parser_p->Token_p->CommentOrCharacter.Data);
    NH_HTML_CHECK_MEM(Comment_p)
    NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToNode(Nh_WebIDL_getObject(Node_p, "DOM", "Node"), Comment_p))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_insertComment(
    Nh_HTML_Parser *Parser_p, Nh_WebIDL_Object *Node_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertCommentAtPosition(Parser_p, Node_p, Nh_HTML_getAppropriatePlaceForInsertingNode(Parser_p, &Node_p)))
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
static Nh_WebIDL_Object *Nh_HTML_insertForeignElement(
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

static Nh_WebIDL_Object *Nh_HTML_insertHTMLElement(
    Nh_HTML_Parser *Parser_p, Nh_HTML_Token Token)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_HTML_insertForeignElement(Parser_p, &Token, &NH_WEBIDL_HTML_NAMESPACE))
}

static Nh_HTML_Token Nh_HTML_createEmptyStartTagToken(
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

// INITIAL =========================================================================================

static NH_BOOL Nh_HTML_startsWith(
    NH_BYTE *a, NH_BYTE *b)
{
NH_HTML_BEGIN()

   if(strncmp(a, b, strlen(b)) == 0) {NH_HTML_END(NH_TRUE)}

NH_HTML_END(NH_FALSE)
}

static NH_BYTE *doctypeIdentifierPrefixes_pp[] =
{
    "+//Silmaril//dtd html Pro v0r11 19970101//",
    "-//AS//DTD HTML 3.0 asWedit + extensions//",
    "-//AdvaSoft Ltd//DTD HTML 3.0 asWedit + extensions//",
    "-//IETF//DTD HTML 2.0 Level 1//",
    "-//IETF//DTD HTML 2.0 Level 2//",
    "-//IETF//DTD HTML 2.0 Strict Level 1//",
    "-//IETF//DTD HTML 2.0 Strict Level 2//",
    "-//IETF//DTD HTML 2.0 Strict//",
    "-//IETF//DTD HTML 2.0//",
    "-//IETF//DTD HTML 2.1E//",
    "-//IETF//DTD HTML 3.0//",
    "-//IETF//DTD HTML 3.2 Final//",
    "-//IETF//DTD HTML 3.2//",
    "-//IETF//DTD HTML 3//",
    "-//IETF//DTD HTML Level 0//",
    "-//IETF//DTD HTML Level 1//",
    "-//IETF//DTD HTML Level 2//",
    "-//IETF//DTD HTML Level 3//",
    "-//IETF//DTD HTML Strict Level 0//",
    "-//IETF//DTD HTML Strict Level 1//",
    "-//IETF//DTD HTML Strict Level 2//",
    "-//IETF//DTD HTML Strict Level 3//",
    "-//IETF//DTD HTML Strict//",
    "-//IETF//DTD HTML//",
    "-//Metrius//DTD Metrius Presentational//",
    "-//Microsoft//DTD Internet Explorer 2.0 HTML Strict//",
    "-//Microsoft//DTD Internet Explorer 2.0 HTML//",
    "-//Microsoft//DTD Internet Explorer 2.0 Tables//",
    "-//Microsoft//DTD Internet Explorer 3.0 HTML Strict//",
    "-//Microsoft//DTD Internet Explorer 3.0 HTML//",
    "-//Microsoft//DTD Internet Explorer 3.0 Tables//",
    "-//Netscape Comm. Corp.//DTD HTML//",
    "-//Netscape Comm. Corp.//DTD Strict HTML//",
    "-//O'Reilly and Associates//DTD HTML 2.0//",
    "-//O'Reilly and Associates//DTD HTML Extended 1.0//",
    "-//O'Reilly and Associates//DTD HTML Extended Relaxed 1.0//",
    "-//SQ//DTD HTML 2.0 HoTMetaL + extensions//",
    "-//SoftQuad Software//DTD HoTMetaL PRO 6.0::19990601::extensions to HTML 4.0//",
    "-//SoftQuad//DTD HoTMetaL PRO 4.0::19971010::extensions to HTML 4.0//",
    "-//Spyglass//DTD HTML 2.0 Extended//",
    "-//Sun Microsystems Corp.//DTD HotJava HTML//",
    "-//Sun Microsystems Corp.//DTD HotJava Strict HTML//",
    "-//W3C//DTD HTML 3 1995-03-24//",
    "-//W3C//DTD HTML 3.2 Draft//",
    "-//W3C//DTD HTML 3.2 Final//",
    "-//W3C//DTD HTML 3.2//",
    "-//W3C//DTD HTML 3.2S Draft//",
    "-//W3C//DTD HTML 4.0 Frameset//",
    "-//W3C//DTD HTML 4.0 Transitional//",
    "-//W3C//DTD HTML Experimental 19960712//",
    "-//W3C//DTD HTML Experimental 970421//",
    "-//W3C//DTD W3 HTML//",
    "-//W3O//DTD W3 HTML 3.0//",
    "-//WebTechs//DTD Mozilla HTML 2.0//",
    "-//WebTechs//DTD Mozilla HTML//",
    "-//W3C//DTD HTML 4.01 Frameset//",
    "-//W3C//DTD HTML 4.01 Transitional//",
};

static NH_BOOL Nh_HTML_setDocumentToQuirksModeFromDOCTYPE(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->DOCTYPE.forceQuirks 
    ||  Parser_p->Token_p->DOCTYPE.Name_p == NULL 
    ||  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->bytes_p, "html")) 
    {
        NH_HTML_END(NH_TRUE) 
    }
    if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p != NULL 
    && !strcmp(Parser_p->Token_p->DOCTYPE.SystemIdentifier_p->bytes_p, "http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd")) 
    {
        NH_HTML_END(NH_TRUE) 
    }
    if (Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL)
    {
        if (!strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3O//DTD W3 HTML Strict 3.0//EN//")
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-/W3C/DTD HTML 4.0 Transitional/EN")
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "HTML")) 
        {
            NH_HTML_END(NH_TRUE) 
        }
        for (int i = 0; i < sizeof(doctypeIdentifierPrefixes_pp) / sizeof(doctypeIdentifierPrefixes_pp[0]); ++i) 
        {
            if (Nh_HTML_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, doctypeIdentifierPrefixes_pp[i]))
            {
                NH_HTML_END(NH_TRUE)
            }
        }
        if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p == NULL) 
        {
            if (Nh_HTML_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD HTML 4.01 Frameset//")
            ||  Nh_HTML_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD HTML 4.01 Transitional//")) 
            {
                NH_HTML_END(NH_TRUE) 
            }
        }
    }

NH_HTML_END(NH_FALSE)
}

static NH_BOOL Nh_HTML_setDocumentToLimitedQuirksModeFromDOCTYPE(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL)
    {
        if (!strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD XHTML 1.0 Frameset//" )
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD XHTML 1.0 Transitional//"))
        {
            NH_HTML_END(NH_TRUE) 
        }
        if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p == NULL) 
        {
            if (Nh_HTML_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD HTML 4.01 Frameset//")
            ||  Nh_HTML_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->bytes_p, "-//W3C//DTD HTML 4.01 Transitional//")) 
            {
                NH_HTML_END(NH_TRUE) 
            }
        }
    }

NH_HTML_END(NH_FALSE)
}

static NH_HTML_RESULT Nh_HTML_processInitial(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER
    && (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
    ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
    ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
    ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
    ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20))
    { 
        // ignore token
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        if (Parser_p->Token_p->DOCTYPE.Name_p == NULL 
        ||  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->bytes_p, "html") 
        ||  Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL
        ||  Parser_p->Token_p->DOCTYPE.SystemIdentifier_p != NULL
        &&  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->bytes_p, "about:legacy-compat"))
        {
            // parse error
        }

        Nh_WebIDL_Object *DocumentType_p = Nh_DOM_createDocumentType(
            Parser_p->Token_p->DOCTYPE.Name_p, Parser_p->Token_p->DOCTYPE.PublicIdentifier_p, 
            Parser_p->Token_p->DOCTYPE.SystemIdentifier_p
        );

        Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, DocumentType_p);
        Nh_DOM_setDocumentType(Parser_p->Document_p, DocumentType_p);

        if (Nh_HTML_setDocumentToQuirksModeFromDOCTYPE(Parser_p)) {
            Nh_DOM_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS);
        }
        else if (Nh_HTML_setDocumentToLimitedQuirksModeFromDOCTYPE(Parser_p)) {
            Nh_DOM_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);
        }

        Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HTML;
    }
    else {
        Nh_DOM_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS);
        NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HTML))
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

// BEFORE HTML =====================================================================================

static NH_HTML_RESULT Nh_HTML_processBeforeHTML(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
            // ignore token
            NH_HTML_END(NH_HTML_SUCCESS)
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG && !strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "html")) 
    {
        Nh_WebIDL_Object *Element_p = Nh_HTML_createElementForToken(Parser_p->Token_p, &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
        Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, Element_p);
        Nh_pushStack(&Parser_p->OpenElements, Element_p);
        Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HEAD;
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        if (strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "head")
        &&  strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "body")
        &&  strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "html")
        &&  strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "br")) 
        {
            // parse error, ignore token
            NH_HTML_END(NH_HTML_SUCCESS)
        }
    }

    Nh_WebIDL_Object *HTMLHtmlElement_p = Nh_WebIDL_createObject("HTML", "HTMLHtmlElement");
    NH_DOM_CHECK_MEM(NH_HTML_ERROR_BAD_STATE, HTMLHtmlElement_p)

    Nh_DOM_setNodeDocument(HTMLHtmlElement_p->Child_p->Child_p->Child_p, Parser_p->Document_p);
    Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, HTMLHtmlElement_p);

    Nh_pushStack(&Parser_p->OpenElements, HTMLHtmlElement_p);
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HEAD))

NH_HTML_END(NH_HTML_SUCCESS)
}

// BEFORE HEAD =====================================================================================

static NH_HTML_RESULT Nh_HTML_processBeforeHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
            // ignore token
            NH_HTML_END(NH_HTML_SUCCESS)
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_END(NH_HTML_SUCCESS)
    }

    Parser_p->HeadElement_p = Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_createEmptyStartTagToken(NH_HTML_TAG_HEAD));
    NH_HTML_CHECK_MEM(Parser_p->HeadElement_p)
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))

NH_HTML_END(NH_HTML_SUCCESS)
}

// IN HEAD =========================================================================================

static NH_HTML_RESULT Nh_HTML_processInHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_END(NH_HTML_SUCCESS)
    }

    Nh_popStack(&Parser_p->OpenElements);
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_HEAD))

NH_HTML_END(NH_HTML_SUCCESS)
}

// IN HEAD NOSCRIPT ================================================================================

static NH_HTML_RESULT Nh_HTML_processInHeadNoscript(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()
NH_HTML_END(NH_HTML_SUCCESS)
}

// AFTER HEAD =======================================================================================

static NH_HTML_RESULT Nh_HTML_processAfterHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_END(NH_HTML_SUCCESS)
    }

    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_createEmptyStartTagToken(NH_HTML_TAG_BODY)))
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))

NH_HTML_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_processInBody(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        NH_HTML_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE)
    {
        Parser_p->stop = NH_TRUE;
        NH_HTML_END(NH_HTML_SUCCESS)
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

// DISPATCH ========================================================================================

static NH_HTML_RESULT Nh_HTML_processToken(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->insertionMode)
    {
        case NH_HTML_INSERTION_MODE_INITIAL          : NH_HTML_END(Nh_HTML_processInitial(Parser_p))
        case NH_HTML_INSERTION_MODE_BEFORE_HTML      : NH_HTML_END(Nh_HTML_processBeforeHTML(Parser_p))
        case NH_HTML_INSERTION_MODE_BEFORE_HEAD      : NH_HTML_END(Nh_HTML_processBeforeHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_HEAD          : NH_HTML_END(Nh_HTML_processInHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT : break; 
        case NH_HTML_INSERTION_MODE_AFTER_HEAD       : NH_HTML_END(Nh_HTML_processAfterHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_BODY          : NH_HTML_END(Nh_HTML_processInBody(Parser_p))
    }

NH_HTML_END(NH_HTML_ERROR_BAD_STATE)
}

static NH_HTML_RESULT Nh_HTML_reprocessToken(
    Nh_HTML_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode)
{
NH_HTML_BEGIN()

    Parser_p->insertionMode = insertionMode;
    NH_HTML_CHECK(Nh_HTML_processToken(Parser_p))

NH_HTML_END(NH_HTML_ERROR_BAD_STATE)
}

// https://html.spec.whatwg.org/multipage/parsing.html#tree-construction-dispatcher
NH_HTML_RESULT Nh_HTML_dispatchTreeConstruction(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    Nh_WebIDL_Object *AdjustedCurrentNode_p = Nh_peekStack(&Parser_p->OpenElements);

    if (Parser_p->OpenElements.size == 0
    ||  Nh_DOM_getNamespaceURI(Nh_WebIDL_getObject(AdjustedCurrentNode_p, "DOM", "Element")) == &NH_WEBIDL_HTML_NAMESPACE
    ||  Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE) {
        NH_HTML_CHECK(Nh_HTML_processToken(Parser_p))
    }
    else {
        exit(0);
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

