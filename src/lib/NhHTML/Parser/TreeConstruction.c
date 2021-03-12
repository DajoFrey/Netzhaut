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

// PARSE ERROR =====================================================================================

static NH_HTML_RESULT Nh_HTML_newTreeConstructionError(
    Nh_HTML_Parser *Parser_p, NH_HTML_PARSE_ERROR error)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(Nh_HTML_newParseError(Parser_p, 0, error))
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

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            goto INITIAL_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            if (Parser_p->Token_p->DOCTYPE.Name_p == NULL 
            ||  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->bytes_p, "html") 
            ||  Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL
            ||  Parser_p->Token_p->DOCTYPE.SystemIdentifier_p != NULL
            &&  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->bytes_p, "about:legacy-compat"))
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
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
            break;
        }
        default : INITIAL_DEFAULT :
        {
            NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HTML))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// BEFORE HTML =====================================================================================

static NH_HTML_RESULT Nh_HTML_processBeforeHTML(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            goto BEFORE_HTML_DEFAULT;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) 
            {
                Nh_WebIDL_Object *HTMLHtmlElement_p = Nh_HTML_createElementForToken(Parser_p->Token_p, &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
                NH_HTML_CHECK_MEM(HTMLHtmlElement_p)
                NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, HTMLHtmlElement_p))
                NH_HTML_CHECK(Nh_HTML_pushOpenElement(Parser_p, HTMLHtmlElement_p))
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HEAD;
                break; 
            }
            goto BEFORE_HTML_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            // fallthrough
        }
        default : BEFORE_HTML_DEFAULT :
        {
            Nh_WebIDL_Object *HTMLHtmlElement_p = Nh_HTML_createElementForToken(Nh_HTML_getEmptyStartTagToken(NH_HTML_TAG_HTML), &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
            NH_HTML_CHECK_MEM(HTMLHtmlElement_p)
            NH_DOM_CHECK(NH_HTML_ERROR_BAD_STATE, Nh_DOM_appendToNode(Parser_p->Document_p->Child_p, HTMLHtmlElement_p))

            NH_HTML_CHECK(Nh_HTML_pushOpenElement(Parser_p, HTMLHtmlElement_p))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HEAD))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// BEFORE HEAD =====================================================================================

static NH_HTML_RESULT Nh_HTML_processBeforeHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                break;
            }
            goto BEFORE_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML)
            {
                Parser_p->HeadElement_p = Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_HTML_CHECK_MEM(Parser_p->HeadElement_p)
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
                break; 
            }
            goto BEFORE_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            // fallthrough
        }
        default : BEFORE_HEAD_DEFAULT :
        {
            Parser_p->HeadElement_p = Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_getEmptyStartTagToken(NH_HTML_TAG_HEAD));
            NH_HTML_CHECK_MEM(Parser_p->HeadElement_p)
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// IN HEAD =========================================================================================

static NH_HTML_RESULT Nh_HTML_processInBody(
    Nh_HTML_Parser *Parser_p
);

static NH_HTML_RESULT Nh_HTML_processInHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
                break;
            }
            goto IN_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML : 
                {
                    NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BASE     :
                case NH_HTML_TAG_BASEFONT :
                case NH_HTML_TAG_BGSOUND  :
                case NH_HTML_TAG_LINK     :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                    // TODO Acknowledge the token's self-closing flag, if it is set.
                    break; 
                }
                case NH_HTML_TAG_META :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                    // TODO
                    break;
                }
                case NH_HTML_TAG_TITLE : 
                {
                    NH_HTML_CHECK(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, NH_FALSE))
                    break;
                }
                case NH_HTML_TAG_NOFRAMES :
                case NH_HTML_TAG_STYLE    :
                {
                    NH_HTML_CHECK(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, NH_TRUE))
                    break;
                }
                case NH_HTML_TAG_SCRIPT :
                {
                    // TODO
                    break;
                }
                case NH_HTML_TAG_TEMPLATE :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_HTML_CHECK(Nh_HTML_insertMarker(Parser_p))
                    Parser_p->framesetOk = NH_FALSE;
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_TEMPLATE;
                    // TODO Push "in template" onto the stack of template insertion modes so that it is the new current template insertion mode.
                    break;
                }
                case NH_HTML_TAG_HEAD :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                case NH_HTML_TAG_NOSCRIPT :
                {
                    if (Parser_p->scripting) {
                        NH_HTML_CHECK(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, NH_TRUE))
                        break;
	    	    }

                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT;
                    break;
                }
                default :
                {
                    goto IN_HEAD_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HEAD :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_HEAD;
                    break; 
                }
                case NH_HTML_TAG_BODY :
                case NH_HTML_TAG_HTML :
                case NH_HTML_TAG_BR   :
                {
                    goto IN_HEAD_DEFAULT;
                }
                case NH_HTML_TAG_TEMPLATE :
                {
                    // TODO
                    break;
                }
                default :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : IN_HEAD_DEFAULT :
        {
            NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_HEAD))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// IN HEAD NOSCRIPT ================================================================================

static NH_HTML_RESULT Nh_HTML_processInHeadNoscript(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p))
                break;
            }
            goto IN_HEAD_NOSCRIPT_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML     :
                {
                    NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BASEFONT :
                case NH_HTML_TAG_BGSOUND  :
                case NH_HTML_TAG_LINK     :
                case NH_HTML_TAG_META     :
                case NH_HTML_TAG_NOFRAMES :
                case NH_HTML_TAG_STYLE    :
                {
                    NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p))
                    break;
                }
                case NH_HTML_TAG_HEAD     :
                case NH_HTML_TAG_NOSCRIPT :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                default :
                {
                    goto IN_HEAD_NOSCRIPT_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_NOSCRIPT :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
                    break; 
                }
                case NH_HTML_TAG_BR :
                {
                    goto IN_HEAD_NOSCRIPT_DEFAULT;
                }
                default :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : IN_HEAD_NOSCRIPT_DEFAULT :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// AFTER HEAD =======================================================================================

static NH_HTML_RESULT Nh_HTML_processAfterHead(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
                break;
            }
            goto AFTER_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML : 
                {
                    NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BODY :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->framesetOk = NH_FALSE;
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_BODY;
                    break; 
                }
                case NH_HTML_TAG_FRAMESET :
                {
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_FRAMESET;
                    break; 
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
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_HTML_CHECK(Nh_HTML_pushOpenElement(Parser_p, Parser_p->HeadElement_p))
                    NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p)) 
                    Nh_removeFromList2(&Parser_p->OpenElements, NH_FALSE, Parser_p->HeadElement_p);
                    break;
                }
                case NH_HTML_TAG_HEAD :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                default :
                {
                    goto AFTER_HEAD_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_TEMPLATE :
                {
                    NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BODY :
                case NH_HTML_TAG_HTML :
                case NH_HTML_TAG_BR   :
                {
                    goto AFTER_HEAD_DEFAULT;
                }
                default :
                {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : AFTER_HEAD_DEFAULT :
        {
            NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_getEmptyStartTagToken(NH_HTML_TAG_BODY)))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// IN BODY =========================================================================================

static NH_HTML_RESULT Nh_HTML_closeElement(
    Nh_HTML_Parser *Parser_p, NH_BYTE *tagName_p)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, tagName_p))

    if (strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Nh_HTML_getCurrentNode(Parser_p)))->bytes_p, tagName_p)) {
        // parse error
    }

    Nh_WebIDL_Object *Pop_p = Nh_HTML_popCurrentNode(Parser_p);
    while (Pop_p != NULL && strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Pop_p))->bytes_p, tagName_p)) {
        Pop_p = Nh_HTML_popCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_closePElement(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(Nh_HTML_closeElement(Parser_p, "p"))
}

static NH_BOOL Nh_HTML_hasOtherOpenElements(
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
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_DOM_getElement(Parser_p->OpenElements.pp[i]));

        NH_BOOL match = NH_FALSE;
        for (int j = 0; j < sizeof(elements_pp)/sizeof(elements_pp[0]); ++j) {
            if (!strcmp(LocalName_p->bytes_p, elements_pp[j])) {match = NH_TRUE; break;}
        }

        if (!match) {NH_HTML_END(NH_TRUE)}
    }

NH_HTML_END(NH_FALSE)
}

static NH_BOOL Nh_HTML_hasOpenElement(
    Nh_HTML_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        Nh_WebIDL_Object *Node_p = Nh_getFromList(&Parser_p->OpenElements, i);
        Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p));
        if (!strcmp(LocalName_p->bytes_p, target_p)) {NH_HTML_END(NH_TRUE)}
    }

NH_HTML_END(NH_FALSE)
}

static NH_HTML_RESULT Nh_HTML_processInBody(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0)
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
        }
        else
        if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
        { 
            NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
            NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
        }
        else 
        {
            NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
            NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
            Parser_p->framesetOk = NH_FALSE;
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT) {
        NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE) {
        NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML :
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))

                for (int i = 0; i < Parser_p->OpenElements.size; ++i) {
                    Nh_WebIDL_Object *Element_p = Parser_p->OpenElements.pp[i];
                    if (!strcmp(Element_p->Interface_p->name_p, "HTMLTemplateElement")) {
                        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                    }
                }

                // TODO

                break;
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
                NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p)) 
                break;
            }
            case NH_HTML_TAG_BODY :
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                if (Parser_p->OpenElements.size > 1 && !strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Parser_p->OpenElements.pp[1]))->bytes_p, "body") && !Nh_HTML_hasOpenElement(Parser_p, "template")) {
                    Parser_p->framesetOk = NH_FALSE;
                    // TODO
                }
                break;
            }
            case NH_HTML_TAG_FRAMESET :
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                if (Parser_p->OpenElements.size > 1 && !strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Parser_p->OpenElements.pp[1]))->bytes_p, "body") && Parser_p->framesetOk) {
                    // TODO
                }
                break;
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
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
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
                Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_DOM_getElement(Current_p));

                if (!strcmp(LocalName_p->bytes_p, "h1")
                ||  !strcmp(LocalName_p->bytes_p, "h2")
                ||  !strcmp(LocalName_p->bytes_p, "h3")
                ||  !strcmp(LocalName_p->bytes_p, "h4")
                ||  !strcmp(LocalName_p->bytes_p, "h5")
                ||  !strcmp(LocalName_p->bytes_p, "h6")) 
                {
                    // parse error
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                }

                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_PRE     :
            case NH_HTML_TAG_LISTING :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                // TODO If the next token is a U+000A LINE FEED (LF) character token, then ignore that token and move on to the next one. (Newlines at the start of pre blocks are ignored as an authoring convenience.)
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_FORM :
            {
                if (Parser_p->FormElement_p && !Nh_HTML_hasOpenElement(Parser_p, "template")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                }
                else {
                    if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                        NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                    }
                    Nh_WebIDL_Object *Form_p = Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p);
                    if (!Nh_HTML_hasOpenElement(Parser_p, "template")) {
                        Parser_p->FormElement_p = Form_p;
                    }
                }
                break;
            }
            case NH_HTML_TAG_LI :
            {
                Parser_p->framesetOk = NH_FALSE;
                Nh_WebIDL_Object *Node_p = Nh_HTML_getCurrentNode(Parser_p);

                while (1)
                {
                    if (!strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p))->bytes_p, "li")) {
                        NH_HTML_CHECK(Nh_HTML_closeElement(Parser_p, "li"))
                        break;
                    }
                    Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p));
                    if (Nh_HTML_inSpecialCategory(Node_p) && strcmp(LocalName_p->bytes_p, "address") && strcmp(LocalName_p->bytes_p, "div") && strcmp(LocalName_p->bytes_p, "p")) {
                        break;
                    }
                    else {
                        Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
                    }
                }

                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_DD :
            case NH_HTML_TAG_DT :
            {
                Parser_p->framesetOk = NH_FALSE;
                Nh_WebIDL_Object *Node_p = Nh_HTML_getCurrentNode(Parser_p);

                while (1)
                {
                    if (!strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p))->bytes_p, "dd")) {
                        NH_HTML_CHECK(Nh_HTML_closeElement(Parser_p, "dd"))
                        break;
                    }
                    else if (!strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p))->bytes_p, "dt")) {
                        NH_HTML_CHECK(Nh_HTML_closeElement(Parser_p, "dt"))
                        break;
                    }
                    Nh_WebIDL_DOMString *LocalName_p = Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p));
                    if (Nh_HTML_inSpecialCategory(Node_p) && strcmp(LocalName_p->bytes_p, "address") && strcmp(LocalName_p->bytes_p, "div") && strcmp(LocalName_p->bytes_p, "p")) {
                        break;
                    }
                    else {
                        Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
                    }
                }

                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_PLAINTEXT :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_PLAINTEXT;
                break;
            }
            case NH_HTML_TAG_BUTTON :
            {
                if (Nh_HTML_hasElementInScope(Parser_p, "button")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, NULL))
                    Nh_WebIDL_Object *Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                    while (Pop_p != NULL && strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Pop_p))->bytes_p, "button")) {
                        Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                    }
                }
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_A :
            {
                // TODO
                break;
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
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                Nh_WebIDL_Object *Element_p = Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_HTML_CHECK_NULL(Element_p)
                NH_HTML_CHECK(Nh_HTML_pushActiveFormattingElement(Parser_p, Element_p))
                break;
            }
            case NH_HTML_TAG_NOBR :
            {
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                if (Nh_HTML_hasElementInScope(Parser_p, "nobr")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    // TODO
                }
                Nh_WebIDL_Object *Element_p = Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_HTML_CHECK_NULL(Element_p)
                NH_HTML_CHECK(Nh_HTML_pushActiveFormattingElement(Parser_p, Element_p))
                break;
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_HTML_CHECK(Nh_HTML_insertMarker(Parser_p))
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_TABLE : 
            {
                // TODO
                break;
            }
            case NH_HTML_TAG_AREA   :
            case NH_HTML_TAG_BR     :
            case NH_HTML_TAG_EMBED  :
            case NH_HTML_TAG_IMG    :
            case NH_HTML_TAG_KEYGEN :
            case NH_HTML_TAG_WBR    :
            {
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_INPUT :
            {
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                // TODO If the token does not have an attribute with the name "type", or if it does, but that attribute's value is not an ASCII case-insensitive match for the string "hidden", then: set the frameset-ok flag to "not ok".
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_PARAM  :
            case NH_HTML_TAG_SOURCE :
            case NH_HTML_TAG_TRACK  :
            {
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                break;
            }
            case NH_HTML_TAG_HR :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                Parser_p->framesetOk = NH_FALSE;
                break;
            }
            case NH_HTML_TAG_TEXTAREA :
            {
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                // TODO If the next token is a U+000A LINE FEED (LF) character token, then ignore that token and move on to the next one. (Newlines at the start of textarea elements are ignored as an authoring convenience.)
                Parser_p->Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA;
                Parser_p->originalInsertionMode = Parser_p->insertionMode;
                Parser_p->framesetOk = NH_FALSE;
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_TEXT;
                break;
            }
            case NH_HTML_TAG_XMP :
            {
                if (Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                }
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                Parser_p->framesetOk = NH_FALSE;
                NH_HTML_CHECK(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, NH_TRUE))
                break;
            }
            case NH_HTML_TAG_IFRAME :
            {
                Parser_p->framesetOk = NH_FALSE;
                NH_HTML_CHECK(Nh_HTML_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, NH_TRUE))
                break;
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
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->framesetOk = NH_FALSE;

                if (Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_TABLE
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_CAPTION
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_TABLE_BODY
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_ROW
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_CELL) {
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_SELECT_IN_TABLE;
                }
                else {Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_SELECT;}

                break;
            }
            case NH_HTML_TAG_OPTGROUP :
            case NH_HTML_TAG_OPTION   :
            {
                if (!strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Nh_HTML_getCurrentNode(Parser_p)))->bytes_p, "option")) {
                    NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                }
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_RB  :
            case NH_HTML_TAG_RTC :
            {
                if (Nh_HTML_hasElementInScope(Parser_p, "ruby")) {
                    NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, NULL))
                    if (strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Nh_HTML_getCurrentNode(Parser_p)))->bytes_p, "ruby")) {
                        // parse error
                    }
                }
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_RP :
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
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            default :
            {
                // catch image
                if (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.bytes_p, "image")) 
                {
                    // parse error
                    Nh_WebIDL_freeDOMString(&Parser_p->Token_p->StartOrEndTag.TagName);
                    Parser_p->Token_p->StartOrEndTag.TagName = Nh_WebIDL_initDOMString(3);
                    Nh_WebIDL_appendToDOMString(&Parser_p->Token_p->StartOrEndTag.TagName, "img", 3);
                    Parser_p->Token_p->StartOrEndTag.tag = NH_HTML_TAG_IMG;
                    NH_HTML_DIAGNOSTIC_END(Nh_HTML_reprocessToken(Parser_p, Parser_p->insertionMode))
                }
        
                NH_HTML_CHECK(Nh_HTML_reconstructActiveFormattingElements(Parser_p))
                NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Parser_p->Token_p))
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_TEMPLATE :
            {
                NH_HTML_CHECK(Nh_HTML_processInHead(Parser_p))
                break;
            }
            case NH_HTML_TAG_BODY :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, "body")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                if (Nh_HTML_hasOtherOpenElements(Parser_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_BODY;
                break;
            }
            case NH_HTML_TAG_HTML :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, "body")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                if (Nh_HTML_hasOtherOpenElements(Parser_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_BODY))
                break;
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
                if (!Nh_HTML_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }

                NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, NULL))

                if (strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Nh_HTML_getCurrentNode(Parser_p)))->bytes_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }

                Nh_WebIDL_Object *Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                while (Pop_p != NULL && strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Pop_p))->bytes_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                }

                break; 
            }
            case NH_HTML_TAG_FORM :
            {
                // TODO
                break; 
            }
            case NH_HTML_TAG_P :
            {
                if (!Nh_HTML_hasElementInButtonScope(Parser_p, "p")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_HTML_CHECK_NULL(Nh_HTML_insertHTMLElement(Parser_p, Nh_HTML_getEmptyStartTagToken(NH_HTML_TAG_P)))
                }
                NH_HTML_CHECK(Nh_HTML_closePElement(Parser_p))
                break;
            }
            case NH_HTML_TAG_LI :
            {
                if (!Nh_HTML_hasElementInListItemScope(Parser_p, "li")) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_HTML_CHECK(Nh_HTML_closeElement(Parser_p, "li"))
                break;
            }
            case NH_HTML_TAG_DD : 
            case NH_HTML_TAG_DT : 
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_HTML_CHECK(Nh_HTML_closeElement(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p))
                break;
            }
            case NH_HTML_TAG_H1 :
            case NH_HTML_TAG_H2 :
            case NH_HTML_TAG_H3 :
            case NH_HTML_TAG_H4 :
            case NH_HTML_TAG_H5 :
            case NH_HTML_TAG_H6 :
            {
                if (!Nh_HTML_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, NULL))
                if (strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Nh_HTML_getCurrentNode(Parser_p)))->bytes_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                Nh_WebIDL_Object *Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                while (Pop_p != NULL && strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Pop_p))->bytes_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) {
                    Pop_p = Nh_HTML_popCurrentNode(Parser_p);
                }
                break;
            }
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
                break;
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                // TODO
                break;
            }
            case NH_HTML_TAG_BR :
            {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                Parser_p->Token_p->type = NH_HTML_TOKEN_START_TAG;
                NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                break;
            }
            default :
            {
//                Nh_WebIDL_Object *Node_p = Nh_HTML_getCurrentNode(Parser_p); 
//
//                if (!strcmp(Nh_DOM_getLocalName(Nh_DOM_getElement(Node_p))->bytes_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p)) i
//                {
//                    NH_HTML_CHECK(Nh_HTML_generateImpliedEndTags(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.bytes_p))
//
//                    if (Node_p != Nh_HTML_getCurrentNode(Parser_p)) {
//                        NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
//                    }
//
//                    while (Nh_HTML_popCurrentNode(Parser_p) != Node_p) {}
//                }
//
//                if (Nh_HTML_inSpecialCategory(Node_p)) {
//                    NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
//                    break;
//                }
//                else {
//                    Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
//                }
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE)
    {
        if (Parser_p->TemplateInsertionModes.size > 0) {
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_TEMPLATE))
        }
        else {
            if (Nh_HTML_hasOtherOpenElements(Parser_p)) {
                NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            }
            Parser_p->stop = NH_TRUE;
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// TEXT ============================================================================================

static NH_HTML_RESULT Nh_HTML_processText(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            NH_HTML_CHECK(Nh_HTML_insertCharacter(Parser_p, NULL, NULL))
            break;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, Parser_p->originalInsertionMode))
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_SCRIPT)
            {
                // TODO
            }
            else {
                NH_HTML_CHECK_NULL(Nh_HTML_popCurrentNode(Parser_p))
                Parser_p->insertionMode = Parser_p->originalInsertionMode;
            }
            break;
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// AFTER BODY =======================================================================================

static NH_HTML_RESULT Nh_HTML_processAfterBody(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->OpenElements.pp[0]))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                // TODO If the parser was created as part of the HTML fragment parsing algorithm, this is a parse error; ignore the token. (fragment case)
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_AFTER_BODY;
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            Parser_p->stop = NH_TRUE;
            break;
        }
        default : AFTER_BODY_DEFAULT :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// AFTER AFTER BODY ================================================================================

static NH_HTML_RESULT Nh_HTML_processAfterAfterBody(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_HTML_CHECK(Nh_HTML_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.bytes_p[0] == 0x20)
            { 
                NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                break;
            }
            goto AFTER_AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                NH_HTML_CHECK(Nh_HTML_processInBody(Parser_p))
                break;
            }
            goto AFTER_AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            Parser_p->stop = NH_TRUE;
            break;
        }
        default : AFTER_AFTER_BODY_DEFAULT :
        {
            NH_HTML_CHECK(Nh_HTML_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_HTML_CHECK(Nh_HTML_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// DISPATCH ========================================================================================

static NH_HTML_RESULT Nh_HTML_processToken(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->insertionMode)
    {
        case NH_HTML_INSERTION_MODE_INITIAL          : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInitial(Parser_p))
        case NH_HTML_INSERTION_MODE_BEFORE_HTML      : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processBeforeHTML(Parser_p))
        case NH_HTML_INSERTION_MODE_BEFORE_HEAD      : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processBeforeHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_HEAD          : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInHeadNoscript(Parser_p)) 
        case NH_HTML_INSERTION_MODE_AFTER_HEAD       : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processAfterHead(Parser_p))
        case NH_HTML_INSERTION_MODE_IN_BODY          : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processInBody(Parser_p))
        case NH_HTML_INSERTION_MODE_TEXT             : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processText(Parser_p))
        case NH_HTML_INSERTION_MODE_AFTER_BODY       : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processAfterBody(Parser_p))
        case NH_HTML_INSERTION_MODE_AFTER_AFTER_BODY : NH_HTML_DIAGNOSTIC_END(Nh_HTML_processAfterAfterBody(Parser_p))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)
}

static NH_HTML_RESULT Nh_HTML_reprocessToken(
    Nh_HTML_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode)
{
NH_HTML_BEGIN()

    Parser_p->insertionMode = insertionMode;
    NH_HTML_CHECK(Nh_HTML_processToken(Parser_p))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
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

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

