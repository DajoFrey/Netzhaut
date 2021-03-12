// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TreeConstruction.h"
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

// DECLARE =========================================================================================

static NH_HTML_RESULT Nh_HTML_reprocessToken(
    Nh_HTML_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode
);

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
        NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS))
        NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HTML))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// BEFORE HTML =====================================================================================

static NH_HTML_RESULT Nh_HTML_processBeforeHTML(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
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
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG && Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) 
    {
        Nh_WebIDL_Object *Element_p = Nh_HTML_createElementForToken(Parser_p->Token_p, &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
        NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, Element_p))
        Nh_pushStack(&Parser_p->OpenElements, Element_p);
        Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HEAD;
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
        {
            // parse error, ignore token
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }

    Nh_WebIDL_Object *HTMLHtmlElement_p = Nh_WebIDL_createObject("HTML", "HTMLHtmlElement");
    NH_HTML_CHECK_MEM(HTMLHtmlElement_p)

    NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_setNodeDocument(HTMLHtmlElement_p->Child_p->Child_p->Child_p, Parser_p->Document_p))
    NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, HTMLHtmlElement_p))

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
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG && Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML)
    {
        Parser_p->HeadElement_p = Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p);
        NH_HTML_CHECK_MEM(Parser_p->HeadElement_p)
        Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
        &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
        {
            // parse error, ignore token
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }

    Parser_p->HeadElement_p = Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_createEmptyStartTagToken(NH_HTML_TAG_HEAD));
    NH_HTML_CHECK_MEM(Parser_p->HeadElement_p)
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
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
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML : 
            {
                // TODO
                break;
            }
            case NH_HTML_TAG_BASE     :
            case NH_HTML_TAG_BASEFONT :
            case NH_HTML_TAG_BGSOUND  :
            case NH_HTML_TAG_LINK     :
            {
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                Nh_popStack(&Parser_p->OpenElements);
                // TODO Acknowledge the token's self-closing flag, if it is set.
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_META :
            {
                Nh_WebIDL_Object *Node_p = Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p);
                Nh_popStack(&Parser_p->OpenElements);

                // TODO
            }
            case NH_HTML_TAG_TITLE : 
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, *Parser_p->Token_p, NH_FALSE))
            }
            case NH_HTML_TAG_SCRIPT :
            {
                // TODO
            }
            case NH_HTML_TAG_TEMPLATE :
            {
                // TODO
            }
            case NH_HTML_TAG_HEAD :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_NOFRAMES :
            case NH_HTML_TAG_STYLE    :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, *Parser_p->Token_p, NH_TRUE))
            }
            case NH_HTML_TAG_NOSCRIPT :
            {
                if (Parser_p->scripting) {
                    NH_HTML_DIAGNOSTIC_END(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, *Parser_p->Token_p, NH_TRUE))
                }

                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {

            case NH_HTML_TAG_HEAD :
            {
                Nh_popStack(&Parser_p->OpenElements);
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_HEAD;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_BODY :
            case NH_HTML_TAG_HTML :
            case NH_HTML_TAG_BR   :
            {
                break;
            }
            case NH_HTML_TAG_TEMPLATE :
            {
                // TODO
                break;
            }
            default :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }

    Nh_popStack(&Parser_p->OpenElements);
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_HEAD))

NH_HTML_END(NH_HTML_SUCCESS)
}

// IN HEAD NOSCRIPT ================================================================================

static NH_HTML_RESULT Nh_HTML_processInBody(
    Nh_HTML_Parser *Parser_p
);

static NH_HTML_RESULT Nh_HTML_processInHeadNoscript(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p))
        }
        // fallthrough
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p)) 
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML     :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInBody(Parser_p)) 
            }
            case NH_HTML_TAG_BASEFONT :
            case NH_HTML_TAG_BGSOUND  :
            case NH_HTML_TAG_LINK     :
            case NH_HTML_TAG_META     :
            case NH_HTML_TAG_NOFRAMES :
            case NH_HTML_TAG_STYLE    :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p)) 
            }
            case NH_HTML_TAG_HEAD     :
            case NH_HTML_TAG_NOSCRIPT :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_NOSCRIPT :
            {
                Nh_popStack(&Parser_p->OpenElements);
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_BR :
            {
                break;
            }
            default :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }

    // parse error
    Nh_popStack(&Parser_p->OpenElements);
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
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
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInBody(Parser_p)) 
            }
            case NH_HTML_TAG_BODY :
            {
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                Parser_p->framesetOk = NH_FALSE;
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_BODY;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_FRAMESET :
            {
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_FRAMESET;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_BASE     :
            case NH_HTML_TAG_BASEFONT :
            case NH_HTML_TAG_BGSOUND  :
            case NH_HTML_TAG_LINK     :
            case NH_HTML_TAG_META     :
            case NH_HTML_TAG_NOFRAMES :
            case NH_HTML_TAG_SCRIPT   :
            case NH_HTML_TAG_STYLE    :
            case NH_HTML_TAG_TEMPLATE :
            case NH_HTML_TAG_TITLE    :
            {
                // parse error
                Nh_pushStack(&Parser_p->OpenElements, Parser_p->HeadElement_p);
                NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p)) 
                Nh_removeFromList2(&Parser_p->OpenElements, NH_FALSE, Parser_p->HeadElement_p);
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_HEAD :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_TEMPLATE :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p)) 
            }
            case NH_HTML_TAG_BODY :
            case NH_HTML_TAG_HTML :
            case NH_HTML_TAG_BR   :
            {
                break;
            }
            default :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }
    }

    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_createEmptyStartTagToken(NH_HTML_TAG_BODY)))
    NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// IN BODY =========================================================================================

static NH_HTML_RESULT Nh_HTML_closePElement(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, "p"))

    if (!strcmp(Nh_HTML_getCurrentNode(Parser_p)->Interface_p->name_p, "HTMLParagraphElement")) {
        // parser error
    }

    Nh_WebIDL_Object *Pop_p = Nh_HTML_popCurrentNode(Parser_p);
    while (Pop_p != NULL && strcmp(Pop_p->Interface_p->name_p, "HTMLParagraphElement")) {
        Pop_p = Nh_HTML_popCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_processInBody(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0)
        {
            // parse error, ignore token
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
        else
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
            NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
        }
        else 
        {
            NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
            NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
            Parser_p->framesetOk = NH_FALSE;
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT)
    {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE)
    {
        // parse error, ignore token
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML :
            {
                // parse error

                for (int i = 0; i < Parser_p->OpenElements.size; ++i) {
                    Nh_WebIDL_Object *Element_p = Parser_p->OpenElements.pp[i];
                    if (!strcmp(Element_p->Interface_p->name_p, "HTMLTemplateElement")) {
                        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                    }
                }

                // TODO

                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_BASE     :
            case NH_HTML_TAG_BASEFONT :
            case NH_HTML_TAG_BGSOUND  :
            case NH_HTML_TAG_LINK     :
            case NH_HTML_TAG_META     :
            case NH_HTML_TAG_NOFRAMES :
            case NH_HTML_TAG_SCRIPT   :
            case NH_HTML_TAG_STYLE    :
            case NH_HTML_TAG_TEMPLATE :
            case NH_HTML_TAG_TITLE    :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p)) 
            }
            case NH_HTML_TAG_BODY :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, "html")) {
                    // parse error, ignore token 
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
            }
            case NH_HTML_TAG_FRAMESET :
            {
                // parse error
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_ADDRESS    :
            case NH_HTML_TAG_ARTICLE    :
            case NH_HTML_TAG_ASIDE      :
            case NH_HTML_TAG_BLOCKQUOTE :
            case NH_HTML_TAG_CENTER     :
            case NH_HTML_TAG_DETAILS    :
            case NH_HTML_TAG_DIALOG     :
            case NH_HTML_TAG_DIR        :
            case NH_HTML_TAG_DIV        :
            case NH_HTML_TAG_DL         :
            case NH_HTML_TAG_FIELDSET   :
            case NH_HTML_TAG_FIGCAPTION :
            case NH_HTML_TAG_FIGURE     :
            case NH_HTML_TAG_FOOTER     :
            case NH_HTML_TAG_HEADER     :
            case NH_HTML_TAG_HGROUP     :
            case NH_HTML_TAG_MAIN       :
            case NH_HTML_TAG_MENU       :
            case NH_HTML_TAG_NAV        :
            case NH_HTML_TAG_OL         :
            case NH_HTML_TAG_P          :
            case NH_HTML_TAG_SECTION    :
            case NH_HTML_TAG_SUMMARY    :
            case NH_HTML_TAG_UL         :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_H1 :
            case NH_HTML_TAG_H2 :
            case NH_HTML_TAG_H3 :
            case NH_HTML_TAG_H4 :
            case NH_HTML_TAG_H5 :
            case NH_HTML_TAG_H6 :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }

                Nh_WebIDL_Object *Current_p = Nh_HTML_getCurrentNode(Parser_p);
                Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_WebIDL_getObject(Current_p, "DOM", "Node"));

                if (!strcmp(LocalName_p->bytes_p, "h1")
                ||  !strcmp(LocalName_p->bytes_p, "h2")
                ||  !strcmp(LocalName_p->bytes_p, "h3")
                ||  !strcmp(LocalName_p->bytes_p, "h4")
                ||  !strcmp(LocalName_p->bytes_p, "h5")
                ||  !strcmp(LocalName_p->bytes_p, "h6")) 
                {
                    // parse error
                    Nh_HTML_popCurrentNode(Parser_p);
                }

                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_PRE     :
            case NH_HTML_TAG_LISTING :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
                // TODO If the next token is a U+000A LINE FEED (LF) character token, then ignore that token and move on to the next one. (Newlines at the start of pre blocks are ignored as an authoring convenience.)
                Parser_p->framesetOk = NH_FALSE;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_FORM :
            {
                if (Parser_p->FormElement_p && !Nh_HTML_hasOpenElement(Parser_p, "template")) {
                    // parse error, ignore token
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                Nh_WebIDL_Object *Form_p = Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p);
                if (!Nh_HTML_hasOpenElement(Parser_p, "template")) {
                    Parser_p->FormElement_p = Form_p;
                }
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_LI :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_DD :
            case NH_HTML_TAG_DT :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_PLAINTEXT :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_BUTTON :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_A :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_B      :
            case NH_HTML_TAG_BIG    :
            case NH_HTML_TAG_CODE   :
            case NH_HTML_TAG_EM     :
            case NH_HTML_TAG_FONT   :
            case NH_HTML_TAG_I      :
            case NH_HTML_TAG_S      :
            case NH_HTML_TAG_SMALL  :
            case NH_HTML_TAG_STRIKE :
            case NH_HTML_TAG_STRONG :
            case NH_HTML_TAG_TT     :
            case NH_HTML_TAG_U      :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_NOBR :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_TABLE : 
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_AREA   :
            case NH_HTML_TAG_BR     :
            case NH_HTML_TAG_EMBED  :
            case NH_HTML_TAG_IMG    :
            case NH_HTML_TAG_KEYGEN :
            case NH_HTML_TAG_WBR    :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_INPUT :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_PARAM  :
            case NH_HTML_TAG_SOURCE :
            case NH_HTML_TAG_TRACK  :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_HR :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
//            case NH_HTML_TAG_IMAGE :
//            {
//                // TODO
//                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
//            }
            case NH_HTML_TAG_TEXTAREA :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_XMP :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_IFRAME :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
//        else 
//        if (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "noembed")
//        || (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "noscript") && Parser_p->scripting))
//        {
//            // TODO
//            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
//        }
            case NH_HTML_TAG_SELECT :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_OPTGROUP :
            case NH_HTML_TAG_OPTION   :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_RB :
            case NH_HTML_TAG_RT :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_MATH :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_SVG :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            case NH_HTML_TAG_CAPTION  :
            case NH_HTML_TAG_COL      :
            case NH_HTML_TAG_COLGROUP :
            case NH_HTML_TAG_FRAME    :
            case NH_HTML_TAG_HEAD     :
            case NH_HTML_TAG_TBODY    :
            case NH_HTML_TAG_TD       :
            case NH_HTML_TAG_TFOOT    :
            case NH_HTML_TAG_TH       :
            case NH_HTML_TAG_THEAD    :
            case NH_HTML_TAG_TR       :
            {
                // parse error, ignore token
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
        }

        NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
        NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, *Parser_p->Token_p))
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {

            case NH_HTML_TAG_TEMPLATE :
            {
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p)) 
            }
            case NH_HTML_TAG_BODY :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, "body")) {
                    // parse error, ignore token 
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
                if (Nh_HTML_hasOtherOpenElements1(Parser_p)) {
                    // parse error
                }
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_BODY;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_HTML :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, "body")) {
                    // parse error, ignore token 
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
                if (Nh_HTML_hasOtherOpenElements1(Parser_p)) {
                    // parse error
                }
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_BODY)) 
            }
            case NH_HTML_TAG_ADDRESS    :
            case NH_HTML_TAG_ARTICLE    :
            case NH_HTML_TAG_ASIDE      :
            case NH_HTML_TAG_BLOCKQUOTE :
            case NH_HTML_TAG_BUTTON     :
            case NH_HTML_TAG_CENTER     :
            case NH_HTML_TAG_DETAILS    :
            case NH_HTML_TAG_DIALOG     :
            case NH_HTML_TAG_DIR        :
            case NH_HTML_TAG_DIV        :
            case NH_HTML_TAG_DL         :
            case NH_HTML_TAG_FIELDSET   :
            case NH_HTML_TAG_FIGCAPTION :
            case NH_HTML_TAG_FIGURE     :
            case NH_HTML_TAG_FOOTER     :
            case NH_HTML_TAG_HEADER     :
            case NH_HTML_TAG_HGROUP     :
            case NH_HTML_TAG_LISTING    :
            case NH_HTML_TAG_MAIN       :
            case NH_HTML_TAG_MENU       :
            case NH_HTML_TAG_NAV        :
            case NH_HTML_TAG_OL         :
            case NH_HTML_TAG_PRE        :
            case NH_HTML_TAG_SECTION    :
            case NH_HTML_TAG_SUMMARY    :
            case NH_HTML_TAG_UL         :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_FORM :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_P :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_LI :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_DD : 
            case NH_HTML_TAG_DT : 
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_H1 :
            case NH_HTML_TAG_H2 :
            case NH_HTML_TAG_H3 :
            case NH_HTML_TAG_H4 :
            case NH_HTML_TAG_H5 :
            case NH_HTML_TAG_H6 :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
//            case NH_HTML_TAG_SARCASM : 
//            {
//                // TODO
//                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
//            }
            case NH_HTML_TAG_A      :
            case NH_HTML_TAG_B      :
            case NH_HTML_TAG_BIG    :
            case NH_HTML_TAG_CODE   :
            case NH_HTML_TAG_EM     :
            case NH_HTML_TAG_FONT   :
            case NH_HTML_TAG_I      :
            case NH_HTML_TAG_NOBR   :
            case NH_HTML_TAG_S      :
            case NH_HTML_TAG_SMALL  :
            case NH_HTML_TAG_STRIKE :
            case NH_HTML_TAG_STRONG :
            case NH_HTML_TAG_TT     :
            case NH_HTML_TAG_U      :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS) 
            }
            case NH_HTML_TAG_BR :
            {
                // TODO
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
        }

        // TODO
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE)
    {
        // TODO
        if (Nh_HTML_hasOtherOpenElements1(Parser_p)) {
            // parse error
        }
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
        case NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT : NH_HTML_END(Nh_HTML_processInHeadNoscript(Parser_p)) 
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

    Nh_WebIDL_Object *AdjustedCurrentNode_p = Nh_HTML_getAdjustedCurrentNode(Parser_p);

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG || Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG) {
        Parser_p->Token_p->StartOrEndTag.tag = Nh_HTML_getTagIndex(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p);
    }

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

