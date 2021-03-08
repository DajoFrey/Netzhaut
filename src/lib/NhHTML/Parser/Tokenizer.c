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

static NH_BOOL Nh_HTML_isASCIIDigit(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()
NH_HTML_END(codepoint >= 0x30 && codepoint <= 0x39)
}

static NH_BOOL Nh_HTML_isASCIIAlphaNumeric(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_HTML_BEGIN()
NH_HTML_END(Nh_HTML_isASCIIAlpha(codepoint) || Nh_HTML_isASCIIDigit(codepoint))
}

static NH_BOOL Nh_HTML_isASCIICaseInsensitiveMatch(
    NH_BYTE *str1_p, NH_BYTE *str2_p)
{
NH_HTML_BEGIN()

     for (;; str1_p++, str2_p++) {
        int d = tolower((unsigned char)*str1_p) - tolower((unsigned char)*str2_p);
        if (d != 0 || !*str1_p) {
            NH_HTML_END(NH_FALSE)
        }
    }
   
NH_HTML_END(NH_TRUE)
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

static NH_HTML_RESULT Nh_HTML_resetTemporaryBuffer(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_Web_freeUSVString(&Tokenizer_p->TemporaryBuffer);
    Tokenizer_p->TemporaryBuffer = Nh_Web_initUSVString(64);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_newParseError(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_HTML_TOKEN type)
{
NH_HTML_BEGIN()

    Nh_HTML_ParseError *Error_p = Nh_incrementArray(&Tokenizer_p->Parser_p->Errors);
    NH_HTML_CHECK_MEM(Error_p)

    Error_p->type  = type;
    Error_p->index = Tokenizer_p->index;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static Nh_HTML_Attribute *Nh_HTML_newAttribute(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

#include NH_HTML_CUSTOM_CHECK

    Nh_HTML_Attribute *Attribute_p = Nh_incrementArray(&Tokenizer_p->Token.StartOrEndTag.Attributes);
    NH_HTML_CHECK_MEM(NULL, Attribute_p)

    Attribute_p->Name = Nh_Web_initDOMString(16);
    Attribute_p->Value = Nh_Web_initDOMString(16);

#include NH_HTML_DEFAULT_CHECK

NH_HTML_END(Attribute_p)
}

static Nh_HTML_Attribute *Nh_HTML_getCurrentAttribute(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()
NH_HTML_END(&((Nh_HTML_Attribute*)Tokenizer_p->Token_p->StartOrEndTag.Attributes.bytes_p)[Tokenizer_p->Token_p->StartOrEndTag.Attributes.length - 1])
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

    Nh_HTML_Token *Token_p = Nh_incrementArray(&Tokenizer_p->Tokens);
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
            Token_p->StartOrEndTag.Attributes = Nh_initArray(sizeof(Nh_HTML_Attribute), 8);
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

static NH_HTML_RESULT Nh_HTML_newDOCTYPEToken(
    Nh_HTML_Tokenizer *Tokenizer_p, NH_BOOL forceQuirks)
{
NH_HTML_BEGIN()

    Nh_HTML_Token *Token_p = Nh_HTML_newToken(Tokenizer_p, NH_HTML_TOKEN_COMMENT);
    NH_HTML_CHECK_MEM(Token_p)
    Token_p->DOCTYPE.forceQuirks = forceQuirks;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
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
        Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
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

    if (Tokenizer_p->codepoint == 0x2F) {
        Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
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

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) {
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
            Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
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

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEndTagOpen(
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

    if (Tokenizer_p->codepoint == 0x2D) {
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

    if (Tokenizer_p->codepoint == 0x2D) {
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH;
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
    } 
    else {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscaped(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapedDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapedDashDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3E))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapedLessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) {
        Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_START))
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_OPEN;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapedEndTagOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_HTML_CHECK(Nh_HTML_newEndTagToken(Tokenizer_p))
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_NAME))
    }
    else {
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
        NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataEscapedEndTagName(
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

    NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscapeStart(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &lower, 1);
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    if (Nh_HTML_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
        {
            Nh_Web_DOMString TemporaryBuffer = Nh_Web_encodeTextToDOMString(Tokenizer_p->TemporaryBuffer.bytes_p, Tokenizer_p->TemporaryBuffer.length);
            if (TemporaryBuffer.length > 0 && !strcmp(TemporaryBuffer.bytes_p, "script")) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            }
            else {Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;}
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            Nh_Web_freeDOMString(&TemporaryBuffer);
            break;
        }
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscaped(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscapedDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscapedDashDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3E))
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscapedLessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_END;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeScriptDataDoubleEscapeEnd(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &lower, 1);
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    if (Nh_HTML_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        Nh_Web_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
        {
            Nh_Web_DOMString TemporaryBuffer = Nh_Web_encodeTextToDOMString(Tokenizer_p->TemporaryBuffer.bytes_p, Tokenizer_p->TemporaryBuffer.length);
            if (TemporaryBuffer.length > 0 && !strcmp(TemporaryBuffer.bytes_p, "script")) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            }
            else {Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;}
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            Nh_Web_freeDOMString(&TemporaryBuffer);
            break;
        }
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBeforeAttributeName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x2F :
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;
        case 0x3D :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME))
            Nh_HTML_Attribute *Attribute_p = Nh_HTML_newAttribute(Tokenizer_p);
            NH_HTML_CHECK_MEM(Attribute_p)
            Nh_Web_appendToDOMString(&Attribute_p->Name, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME;
            break;
        default :
            NH_HTML_CHECK_MEM(Nh_HTML_newAttribute(Tokenizer_p))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// TODO 13.2.5.33 When the user agent leaves the attribute name state (and before emitting the tag token, if appropriate), the complete attribute's name must be compared to the other attributes on the same token; if there is already an attribute on the token with the exact same name, then this is a duplicate-attribute parse error and the new attribute must be removed from the token.
static NH_HTML_RESULT Nh_HTML_consumeAttributeName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Name, &lower, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
 
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;
        case 0x3D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Name, &replace, 1);
            break;
        case 0x22 :
        case 0x27 :
        case 0x3C :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME))
        default :
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Name, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterAttributeName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x2F :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG))
            break;
        case 0x3D :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        default :
            NH_HTML_CHECK_MEM(Nh_HTML_newAttribute(Tokenizer_p))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBeforeAttributeValue(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
            break;
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_ATTRIBUTE_VALUE))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAttributeValueDoubleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        default :
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAttributeValueSingleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        default :
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAttributeValueUnquoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        case 0x22 :
        case 0x27 :
        case 0x3C :
        case 0x3D :
        case 0x60 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE))
            // fallthrough
        default :
            Nh_Web_appendToDOMString(&Nh_HTML_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAttributeValueQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

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
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_BETWEEN_ATTRIBUTES))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeSelfClosingStartTag(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->StartOrEndTag.selfClosing = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_SOLIDUS_IN_TAG))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBogusComment(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &replace, 1);
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeMarkupDeclarationOpen(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->length - (Tokenizer_p->index + 1) > 1) 
    {
        if (((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 0] == 0x2D
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 1] == 0x2D) 
        {
            Tokenizer_p->index += 2;
            NH_HTML_CHECK(Nh_HTML_newComment(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_START;
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }

    if (Tokenizer_p->length - (Tokenizer_p->index + 1) > 6) 
    {
        if (((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 0] == '['
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 1] == 'C'
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 2] == 'D'
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 3] == 'A'
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 4] == 'T'
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 5] == 'A'
        &&  ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index + 6] == '['') 
        {
            Tokenizer_p->index += 7;
            printf("TODO HTML PARSER 13.2.5.42 Markup declaration open state\n");
            exit(0);
        }

        Nh_Web_DOMString Temporary = Nh_Web_initDOMString(7);
        Nh_Web_appendUnicodeToDOMString(&Temporary, &((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index], 7);

        if (Nh_HTML_isASCIICaseInsensitiveMatch(Temporary.bytes_p, "DOCTYPE")) {
            Tokenizer_p->index += 7;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE;
            Nh_Web_freeDOMString(&Temporary);
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }

        Nh_Web_freeDOMString(&Temporary);
    }

    NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_OPENED_COMMENT))
    NH_HTML_CHECK(Nh_HTML_newCommentToken(Tokenizer_p))
    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentStart(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_START_DASH;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_CLOSING_OF_EMPTY_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentStartDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_CLOSING_OF_EMPTY_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_UNICODE_CODEPOINT append = 0x2D;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeComment(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN;
            break;
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &replace, 1);
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentLessThanSign(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x21 :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG;
            break;
        case 0x3C :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentLessThanSignBang(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentLessThanSignBangDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentLessThanSignBangDashDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_NESTED_COMMENT))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentEndDash(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END;
            break;
        default :
            NH_UNICODE_CODEPOINT append = 0x2D;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentEnd(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_BANG;
            break;
        case 0x2D :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
        default :
            NH_UNICODE_CODEPOINT append = 0x2D;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCommentEndBang(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            NH_UNICODE_CODEPOINT exclamation = 0x21;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &exclamation, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_CLOSED_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_UNICODE_CODEPOINT minus = 0x2D, exclamation = 0x21;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &minus, 1);
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &minus, 1);
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &exclamation, 1);
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPE(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_BEFORE_DOCTYPE_NAME))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBeforeDOCTYPEName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        NH_HTML_CHECK(Nh_HTML_createDOCTYPEToken(Tokenizer_p, NH_FALSE))
        Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &lower, 1);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Nh_HTML_CHECK(Nh_HTML_createDOCTYPEToken(Tokenizer_p, NH_FALSE))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &replace, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_NAME))
            NH_HTML_CHECK(Nh_HTML_createDOCTYPEToken(Tokenizer_p, NH_TRUE))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_createDOCTYPEToken(Tokenizer_p, NH_FALSE))
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPEName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Nh_HTML_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_UNICODE_CODEPOINT lower = Tokenizer_p->codepoint + 0x20;
        Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &lower, 1);
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_NAME;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &replace, 1);
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterDOCTYPEName(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.Name, &replace, 1);
            break;
        default :
            if (Tokenizer_p->length - (Tokenizer_p->index + 1) > 5) 
            {
                Nh_Web_DOMString Temporary = Nh_Web_initDOMString(6);
                Nh_Web_appendUnicodeToDOMString(&Temporary, &((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index], 6);

                NH_BOOL isPublic = Nh_HTML_isASCIICaseInsensitiveMatch(Temporary.bytes_p, "PUBLIC");
                NH_BOOL isSystem = Nh_HTML_isASCIICaseInsensitiveMatch(Temporary.bytes_p, "SYSTEM");

                Nh_Web_freeDOMString(&Temporary);
 
                if (isPublic) {
                    Tokenizer_p->index += 6;
                    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
                if (isSystem) {
                    Tokenizer_p->index += 6;
                    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
                    NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
                }
            }
 
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))

            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterDOCTYPEPublicKeyword(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x22 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBeforeDOCTYPEPublicIdentifier(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPEPublicIdentifierDoubleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier, &replace, 1);
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPEPublicIdentifierSingleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier, &replace, 1);
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterDOCTYPEPublicIdentifier(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x22 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBetweenDOCTYPEPublicAndSystemIdentifiers(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterDOCTYPESystemKeyword(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x22 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITE_SPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBeforeDOCTYPESystemIdentifier(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPESystemIdentifierDoubleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier, &replace, 1);
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeDOCTYPESystemIdentifierSingleQuoted(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_UNICODE_CODEPOINT replace = 0xFFFD;
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier, &replace, 1);
            break;
        case 0x3E :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_APRUBT_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            Nh_Web_appendToDOMString(&Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier, &Tokenizer_p->codepoint, 1);
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeAfterDOCTYPESystemIdentifier(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeBogusDOCTYPE(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            break;
        default :
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCDATASection(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_BRACKET;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCDATASectionBracket(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_END;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCDATASectionEnd(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            break;
        default :
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_flushCharacterReference(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED
    ||  Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED
    ||  Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED) 
    {
        Nh_Web_appendUnicodeToDOMString(&((Nh_HTML_Attribute*)Tokenizer_p->StartOrEndTag.Attributes.bytes_p)[Tokenizer_p->StartOrEndTag.Attributes.length - 1].Value, Tokenizer_p->TemporaryBuffer.bytes_p, Tokenizer_p->TemporaryBuffer.length);
    }
    else {
        for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))
        }
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeCharacterReference(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Nh_HTML_resetTemporaryBuffer(Tokenizer_p);
    NH_UNICODE_CODEPOINT ambersand = 0x26;
    Nh_Web_appendToUSFString(&Tokenizer_p->TemporaryBuffer, &ambersand, 1); 

    if (Nh_HTML_isAlphaNumeric(Tokenizer_p->codepoint)) {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_NAMED_CHARACTER_REFERENCE))
    }

    if (Tokenizer_p->codepoint == 0x23) {
        Nh_Web_appendToUSFString(&Tokenizer_p->TemporaryBuffer, Tokenizer_p->codepoint, 1); 
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_NUMERIC_CHARACTER_REFERENCE;
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    NH_HTML_CHECK(Nh_HTML_flushCharacterReference(Tokenizer_p))
    NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, Tokenizer_p->returnState))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

static NH_HTML_RESULT Nh_HTML_consumeNamedCharacterReference(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// 13.2.5.74

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

            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
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

        case NH_HTML_TOKENIZATION_STATE_RCDATA_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
            }
            NH_HTML_DIAGNOSTIC_END(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
            }
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(NH_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
            }
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START_DASH :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_OPEN :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_NAME :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, ((NH_UNICODE_CODEPOINT*)Tokenizer_p->TemporaryBuffer.bytes_p)[i]))    
            }
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_START :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_END :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;

        case NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE :
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED))
            break;

        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT :

            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_MARKUP_DECLARATION_OPEN :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_OPENED_COMMENT))
            NH_HTML_CHECK(Nh_HTML_newCommentToken(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT;
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_START :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_START_DASH :
        case NH_HTML_TOKENIZATION_STATE_COMMENT :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_COMMENT))
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN :
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH :

            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH :
        case NH_HTML_TOKENIZATION_STATE_COMMENT_END :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_COMMENT))
            NH_HTML_CHECK(Nh_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_DOCTYPE :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_DOCTYPE))
            NH_HTML_CHECK(Nh_HTML_newDOCTYPEToken(Tokenizer_p, NH_TRUE))
            NH_HTML_CHECK(NH_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_NAME :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_KEYWORD :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_KEYWORD :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_DOCTYPE))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = NH_TRUE;
            NH_HTML_CHECK(NH_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE :

            NH_HTML_CHECK(NH_HTML_emit(Tokenizer_p))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION :

            NH_HTML_CHECK(Nh_HTML_newParseError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_CDATA))
            NH_HTML_CHECK(NH_HTML_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_BRACKET :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_END :

            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;

        case NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE :
        
            NH_HTML_CHECK(Nh_HTML_flushCharacterReference(Tokenizer_p))
            NH_HTML_CHECK(Nh_HTML_reconsume(Tokenizer_p, Tokenizer_p->returnState))
            break;
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

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

    if (Tokenizer_p->index == Tokenizer_p->Codepoints.length) {
        NH_HTML_DIAGNOSTIC_END(Nh_HTML_handleEOF(Tokenizer_p))
    }

    switch (Tokenizer_p->state)
    {
        case NH_TTY_TOKENIZATION_STATE_DATA : NH_HTML_END(Nh_HTML_consumeData(Tokenizer_p))
        case NH_TTY_TOKENIZATION_STATE_TAG_OPEN : NH_HTML_END(Nh_HTML_consumeTagOpen(Tokenizer_p))
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_ERROR_BAD_STATE)
}

NH_HTML_RESULT Nh_HTML_consumeNext(
    Nh_HTML_Tokenizer *Tokenizer_p)
{
NH_HTML_BEGIN()

    Tokenizer_p->index++;

    if (Tokenizer_p->index < Tokenizer_p->Codepoints.length) {
        Tokenizer_p->codepoint = ((NH_UNICODE_CODEPOINT*)Tokenizer_p->Codepoints.bytes_p)[Tokenizer_p->index];
    }

NH_HTML_DIAGNOSTIC_END(Nh_HTML_consume(Tokenizer_p))
}

