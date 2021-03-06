// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Tokenizer.h"

#include "../Common/Macro.h"
#include "../../NhCore/Unicode.h"

#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

static Nh_HTML_Token *Nh_HTML_consume(
    Nh_HTML_Tokenizer *Tokenizer_p, Nh_UnicodeString Codepoints
);

static NH_HTML_RESULT Nh_HTML_reconsume(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_HTML_TOKENIZATION_STATE newState
);

// INIT ============================================================================================

Nh_HTML_Tokenizer Nh_HTML_initTokenizer()
{
NH_HTML_BEGIN()

    Nh_HTML_Tokenizer Tokenizer;
    Tokenizer.state = NH_HTML_TOKENIZATION_STATE_DATA;
    Tokenizer.returnState = Tokenizer.state;
    Tokenizer.Tokens = Nh_initArray(sizeof(Nh_HTML_Token), 1024);
    Tokenizer.Emits = Nh_initList(1024);
    Tokenizer.index = -1;
    Tokenizer.codepoint = 0;
    Tokenizer.EOF = NH_FALSE;
    Tokenizer.Token_p = NULL;
    Tokenizer.TemporaryBuffer = Nh_Web_initUSVString(64);

NH_HTML_END(Tokenizer)
}

// HELPER ==========================================================================================

static NH_BOOL Nh_HTML_isASCIIUpperAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()
NH_HTML_END(codepoint >= 0x41 && codepoint <= 0x5A)
}

static NH_BOOL Nh_HTML_isASCIILowerAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()
NH_HTML_END(codepoint >= 0x61 && codepoint <= 0x7A)
}

static NH_BOOL Nh_HTML_isASCIIAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_HTML_isASCIIUpperAlpha(codepoint) && Nh_HTML_isASCIILowerAlpha(codepoint))
}

static NH_BOOL Nh_HTML_isAppropriateEndTagToken(
    Nh_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    for (int i = Tokenizer_p->Emits.size - 1; i >= 0; --i)
    {
        Nh_HTML_Token *Emit_p = Tokenizer_p->Emits.pp[i];
        if (Emit_p->type == NH_HTML_TOKEN_START_TAG) {
            if (!strcmp(Tokenizer_p->Token_p->StartOrEndTag.TagName.bytes_p, Emit_p->StartOrEndTag.TagName.bytes_p)) {
                NH_HTML_END(NH_TRUE)
            }
            break;
        }
    } 

NH_HTML_END(NH_FALSE)
}

static NH_HTML_RESULT Nh_HTML_newParseError(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_HTML_TOKEN type)
{
NH_HTML_BEGIN()

    Nh_HTML_ParseError *Error_p = Nh_getFromArray(&Tokenizer_p->Parser_p->Errors, -1);
    NH_HTML_CHECK_MEM(Error_p)

    Error_p->type  = type;
    Error_p->index = Tokenizer_p->index;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_emit(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_appendToList(&Tokenizer_p->Emits, Tokenizer_p->Token_p);
    Tokenizer_p->Parser_p->Token_p = Tokenizer_p->Token_p;
    NH_HTML_CHECK(Nh_HTML_dispatchTreeConstruction(Tokenizer_p->Parser_p))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static Nh_HTML_Token *Nh_HTML_newToken(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_HTML_PARSE_ERROR type)
{
NH_HTML_BEGIN()

#include NH_HTML_CUSTOM_CHECK

    Nh_HTML_Token *Token_p = Nh_getFromArray(&Tokenizer_p->Tokens, -1);
    NH_HTML_CHECK_MEM(NULL, Token_p)

    Token_p->type = type;

    switch (type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
            Token_p->DOCTYPE.Name = Nh_Web_initDOMString(32);
            Token_p->DOCTYPE.PublicIdentifier = Nh_Web_initDOMString(32);
            Token_p->DOCTYPE.SystemIdentifier = Nh_Web_initDOMString(32);
            break;

        case NH_HTML_TOKEN_START_TAG :
        case NH_HTML_TOKEN_END_TAG :
            Token_p->StartOrEndTag.TagName = Nh_Web_initDOMString(8);
            break;

        case NH_HTML_TOKEN_COMMENT :
            Token_p->CommentOrCharacter.Data = Nh_Web_initDOMString(64);
            break;

        case NH_HTML_TOKEN_CHARACTER :
            Token_p->CommentOrCharacter.Data = Nh_Web_initDOMString(1);
            break;
    }

    Tokenizer_p->Token_p = Token_p;

#include NH_HTML_DEFAULT_CHECK

NH_HTML_END(Token_p)
}

static NH_HTML_RESULT Nh_HTML_newCommentToken(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Token *Token_p = Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_COMMENT);
    NH_HTML_CHECK_MEM(Token_p)

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_newStartTagToken(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Token *Token_p = Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_START_TAG);
    NH_HTML_CHECK_MEM(Token_p)

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_newEndTagToken(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Token *Token_p = Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_END_TAG);
    NH_HTML_CHECK_MEM(Token_p)

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_emitCharacterToken(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()

    Nh_HTML_Token *Token_p = Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_CHARACTER, NH_TRUE);
    NH_HTML_CHECK_MEM(Token_p)
    Nh_Web_appendUnicodeToDOMString(&Token_p->CommentOrCharacter.Data, &codepoint, 1);
    Nh_HTML_emit(Tokenizer_p);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_emitEOFToken(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK_MEM(Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_END_OF_FILE, NH_TRUE))
    Nh_HTML_emit(Tokenizer_p);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// DATA STATE ======================================================================================

static NH_HTML_RESULT Nh_HTML_handleEOF(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->state)
    {
        case NH_HTML_TOKENIZATION_STATE_DATA        :
        case NH_HTML_TOKENIZATION_STATE_RCDATA      :
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT     :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA :
        case NH_HTML_TOKENIZATION_STATE_PLAINTEXT   :

            NH_HTML_DIAGNOSTIC_END(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_BEFORE_TAG_NAME))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_BEFORE_TAG_NAME))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_HTML_CHECK(Nh_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_RCDATA_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_RCDATA_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_RCDATA_END_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
            }
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_RAWTEXT_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_RAWTEXT_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeData(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_DATA;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3c :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_TAG_OPEN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
        default :
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRCDATA(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_RCDATA;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3c :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
        default :
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRAWTEXT(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RAWTEXT_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
        default :
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptData(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
        default :
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumePLAINTEXT(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
        default :
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeTagOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_HTML_CHECK(Nh_HTML_newStartTagToken(Tokenizer_p))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_TAG_NAME))
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_MARKUP_DECLARATION_OPEN;
            break;
        case 0x2f :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_END_TAG_OPEN;
            break;
        case 0x3f :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME))
            NH_HTML_CHECK(Nh_HTML_newCommentToken(Tokenizer_p))
            NH_HTML_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT))
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_FIRST_CHARACTER_OF_TAG_NAME))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_END(NH_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeEndTagOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_HTML_CHECK_MEM(Nh_HTML_newEndTagToken(Tokenizer_p))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_TAG_NAME))
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_END_TAG_NAME))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_FIRST_CHARACTER_OF_TAG_NAME))
            NH_HTML_CHECK(Nh_HTML_newCommentToken(Tokenizer_p))
            NH_HTML_END(NH_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeTagName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
            break;
        case 0x2F :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
            break;
        case 0x3E:
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRCDATALessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->codepoint == 0x2F) {
        Nh_Web_freeUSVString(&Tokenizer_p->TemporaryBuffer);
        Tokenizer_p->TemporaryBuffer = Nh_Web_initUSVString(64);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_OPEN;
    }
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRCDATAEndTagOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) 
    {
        NH_HTML_CHECK(Nh_HTML_newEndTagToken(Tokenizer_p))
        NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_NAME))
    } 
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRCDATAEndTagName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    if (Nh_HTML_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x2F :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x3E:
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_emit(Tokenizer_p))
            }
            break;
    }

    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
    }

    NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRAWTEXTLessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->codepoint == 0x2F) 
    {
        Nh_Web_freeUSVString(&Tokenizer_p->TemporaryBuffer);
        Tokenizer_p->TemporaryBuffer = Nh_Web_initUSVString(64);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_OPEN;
    } 
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRAWTEXTEndTagOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) 
    {
        NH_HTML_CHECK(Nh_HTML_newEndTagToken(Tokenizer_p))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_NAME))
    } 
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeRAWTEXTEndTagName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    if (Nh_HTML_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x2F :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x3E:
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_emit(Tokenizer_p))
            }
            break;
    }

    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
    }

    NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataLessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            Nh_Web_freeUSVString(&Tokenizer_p->TemporaryBuffer);
            Tokenizer_p->TemporaryBuffer = Nh_Web_initUSVString(64);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_OPEN;
            break;
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x21))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_END(NH_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEndTagOpenState(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) 
    {
        NH_HTML_CHECK(Nh_HTML_newEndTagToken(Tokenizer_p))
        NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_NAME))
    } 
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEndTagName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    if (Nh_HTML_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        Nh_Web_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x2F :
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
            }
            break;
        case 0x3E:
            if (Nh_HTML_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                NH_HTML_DIAGNOSTIC_END(Nh_HTML_emit(Tokenizer_p))
            }
            break;
    }

    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
    }

    NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapeStart(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->codepoint == 0x2D) 
    {
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START_DASH;
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
    } 
    else {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapeStartDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->codepoint == 0x2D) 
    {
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH;
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
    } 
    else {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// 13.2.5.20

// CONSUME NEXT ====================================================================================

static NH_HTML_RESULT Nh_HTML_reconsume(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_HTML_TOKENIZATION_STATE newState)
{
NH_HTML_BEGIN()

    Tokenizer_p->state = newState;

NH_HTML_DIAGNOSTIC_END(Nh_HTML_consume(Tokenizer_p))
}

static NH_HTML_RESULT Nh_HTML_consume(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->state)
    {
        case NH_TTY_TOKENIZATION_STATE_DATA :
            NH_HTML_END(Nh_HTML_consumeData(Tokenizer_p, Codepoints))
        case NH_TTY_TOKENIZATION_STATE_TAG_OPEN :
            NH_HTML_END(Nh_HTML_consumeTagOpen(Tokenizer_p, Codepoints))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)
}

Nh_HTML_Token *Nh_HTML_consumeNext(
    Nh_HTML_Tokenizer *Tokenizer_p, Nh_UnicodeString Codepoints)
{
NH_HTML_BEGIN()

    Tokenizer_p->index++;

    if (Tokenizer_p->index == Codepoints.length) {Tokenizer_p->EOF = NH_TRUE;}
    else {
        Tokenizer_p->codepoint = ((NH_UNICODE_CODEPOINT*)Codepoints.bytes_p[Tokenizer_p->index];
    }

NH_HTML_END(Nh_HTML_consume(Tokenizer_p, Codepoints))
}

