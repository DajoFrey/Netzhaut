// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "TreeConstruction.h"

#include "../Common/Macro.h"
#include "../../NhCore/Unicode.h"

#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TREE CONSTRUCTION ===============================================================================

static NH_HTML_RESULT Nh_HTML_processInitialToken(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->Token.type == NH_HTML_TOKEN_CHARACTER)
    { 
        switch (Parser_p->Token_p->CommentOrCharacter.data_p[0])
        {
            case 0x09 :
            case 0x0A :
            case 0x0C :
            case 0x0D :
            case 0x20 :
                break; // ignore token
        }
    }
    else if (Parser_p->Token.type == NH_HTML_TOKEN_COMMENT)
    {
        Nh_Web_Object *Comment_p = Nh_DOM_createComment(Parser_p->Token.CommentOrCharacter.Data);
        Nh_DOM_appendChildToNode(Nh_Web_decompose(Parser_p->Document_p, "DOM")->Inherit_p, Comment_p);
    }
    else if (Parser_p->Token.type == NH_HTML_TOKEN_DOCTYPE)
    {
    }
    else {
        Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HTML;
        Nh_DOM_setDocumentMode(Nh_Web_decompose(Parser_p->Document_p, "DOM"), "quirks");
        Nh_HTML_dispatchTreeConstruction(Parser_p);
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_dispatchInsertionMode(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    switch (Parser_p->insertionMode)
    {
        case NH_HTML_INSERTION_MODE_INITIAL :
            NH_HTML_END(Nh_HTML_processInitialToken(Parser_p))
    }

NH_HTML_END(NH_HTML_ERROR_BAD_STATE)
}

NH_HTML_RESULT Nh_HTML_dispatchTreeConstruction(
    Nh_HTML_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->OpenElements.size == 0
    ||  Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE) {
        Nh_HTML_dispatchInsertionMode(Parser_p);
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

