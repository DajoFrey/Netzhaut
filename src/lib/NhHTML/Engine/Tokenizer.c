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

// INIT ============================================================================================

Nh_HTML_Tokenizer Nh_HTML_initTokenizer()
{
NH_HTML_BEGIN()

    Nh_HTML_Tokenizer Tokenizer;
    Tokenizer.state = NH_HTML_TOKENIZATION_STATE_DATA;
    Tokenizer.returnState = Tokenizer.state;
    Tokenizer.Tokens = Nh_initArray(sizeof(Nh_HTML_Token), 1024);
    Tokenizer.Next_p = Nh_getFromArray(&Tokenizer.Tokens, -1);

NH_HTML_END(Tokenizer)
}

// DATA STATE ======================================================================================

static NH_BOOL Nh_HTML_tokenizeDataState(
    Nh_HTML_Tokenizer *Tokenizer_p, Nh_UnicodeString Codepoints, unsigned int index, Nh_HTML_Token *Token_p)
{
NH_HTML_BEGIN()

    if (Codepoints.length == index) {
        Token_p->type = NH_HTML_TOKEN_END_OF_FILE;
        NH_HTML_END(NH_TRUE)
    }

    switch (((NH_UNICODE_CODEPOINT*)Codepoints.bytes_p)[index])
    {
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_DATA;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3c :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_TAG_OPEN;
            break;
        case 0x00 : ;
            Nh_HTML_ParseError Error = Nh_getFromArray(&Tokenizer_p->Parser_p->Errors, -1);
            Error.type  = NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER;
            Error.index = index;
        default :
            Token_p->type = NH_HTML_TOKEN_CHARACTER;
            Token_p->CommentOrCharacterToken.data_p = &((NH_UNICODE_CODEPOINT*)Codepoints.bytes_p)[index];
            Token_p->CommentOrCharacterToken.length = 1;
            NH_HTML_END(NH_TRUE)
    }

NH_HTML_END(NH_FALSE)
}

// CONSUME NEXT ====================================================================================

NH_BOOL Nh_HTML_tokenize(
    Nh_HTML_Tokenizer *Tokenizer_p, Nh_UnicodeString Codepoints, unsigned int index)
{
NH_HTML_BEGIN()

    if (Tokenizer_p->Next_p == NULL) {NH_HTML_END(NH_FALSE)} // unexpected

    *index_p++;
    NH_BOOL emit = NH_FALSE;

    switch (Tokenizer_p->state)
    {
        case NH_TTY_TOKENIZATION_STATE_DATA :
            emit = Nh_HTML_consumeNextDataState(Tokenizer_p, Codepoints, *index_p, )
            break;   
    }

    if (emit) {
        Tokenizer_p->Next_p = Nh_getFromArray(&Tokenizer_p->Tokens, -1);
    }

NH_HTML_END(NH_FALSE)
}

