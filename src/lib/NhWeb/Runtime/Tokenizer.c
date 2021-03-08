// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Tokenizer.h"

#include "../Common/Log.h"
#include "../Common/Macro.h"

#include NH_WEB_DEFAULT_CHECK
#include NH_WEB_FLOW

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const NH_BYTE *NH_WEB_TOKENS_PP[] = {
    "Whitespace",
    "LineTerminator",
    "Comment",
    "IdentifierName",
    "Punctuator",
    "DecimalLiteral",
    "DecimalBigIntegerLiteral",
    "NonDecimalBinaryIntegerLiteral",
    "NonDecimalOctalIntegerLiteral",
    "NonDecimalHexIntegerLiteral",
    "StringLiteral",
};

// TOKENIZER =======================================================================================

static NH_BOOL Nh_Web_isDigit(
    NH_UNICODE_CODEPOINT codepoint, NH_BOOL zero)
{
NH_WEB_BEGIN()

    if (codepoint == '1' 
    ||  codepoint == '2'
    ||  codepoint == '3'
    ||  codepoint == '4'
    ||  codepoint == '5'
    ||  codepoint == '6'
    ||  codepoint == '7'
    ||  codepoint == '8'
    ||  codepoint == '9')
    {
        NH_WEB_END(NH_TRUE)
    }

    if (zero && codepoint == '0') {NH_WEB_END(NH_TRUE)}

NH_WEB_END(NH_FALSE)
}

static unsigned int Nh_Web_isDigits(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (Nh_Web_isDigit(codepoints_p[0], NH_TRUE)) {
        unsigned int i = 1;
        while (i < length && (Nh_Web_isDigit(codepoints_p[i], NH_TRUE) || codepoints_p[i] == '_')) {
            i++;
        }
        NH_WEB_END(i)
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isSignedInteger(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    int offset = 0;
    if (codepoints_p[0] == '+' || codepoints_p[0] == '-') {
        offset++;
    }
    int digits = Nh_Web_isDigits(offset == 0 ? codepoints_p : &codepoints_p[1], NH_TRUE);
    if (digits == 0) {NH_WEB_END(0)}

NH_WEB_END(offset + digits)
}

static unsigned int Nh_Web_isExponentPart(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (codepoints_p[0] != 'e' && codepoints_p[0] != 'E') {
        NH_WEB_END(0)
    }
    int digits = Nh_Web_isSignedInteger(&codepoints_p[1], NH_TRUE);

NH_WEB_END(digits)
}

static unsigned int Nh_Web_isDecimalIntegerLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (codepoints_p[0] == '0') {NH_WEB_END(1)}

    if (Nh_Web_isDigit(codepoints_p[0], NH_FALSE)) 
    {
        int offset = 1;
        if (length > 1 && codepoints_p[1] == '_') {
            offset++;
        }
        int digits = Nh_Web_isDigits(&codepoints_p[offset], length - offset);
        if (digits == 0 && offset == 2) {NH_WEB_END(0)}
        NH_WEB_END(digits + offset)
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isWhiteSpace(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_WEB_BEGIN()

    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020) // SPACE <SP> 
    {
        NH_WEB_END(1)
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isLineTerminator(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()


    if (codepoints_p[0] == 0x000A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x000D) // CARRIAGE RETURN (CR) <CR> 
    {
        if (length > 1 && codepoints_p[0] == 0x000D && codepoints_p[1] == 0x000A) {
            NH_WEB_END(2)
        }
        NH_WEB_END(1)
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isComment(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (codepoints_p[0] == 0x002F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x002F) 
        {
            unsigned int i = 2;
            while (i < length && !Nh_Web_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            NH_WEB_END(i)
        }

        // multi line comment 
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isPunctuator(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (length > 3) {
        if (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>' && codepoints_p[3] == '=')
        {
            NH_WEB_END(4)
        }
    }

    if (length > 2) {
        if ((codepoints_p[0] == '.' && codepoints_p[1] == '.' && codepoints_p[2] == '.')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '=' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '!' && codepoints_p[1] == '=' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '*' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '<' && codepoints_p[1] == '<' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '&' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '|' && codepoints_p[2] == '=')
        ||  (codepoints_p[0] == '?' && codepoints_p[1] == '?' && codepoints_p[2] == '='))
        {
            NH_WEB_END(3)
        }
    }

    if (length > 1) {
        if ((codepoints_p[0] == '<' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '!' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '*')
        ||  (codepoints_p[0] == '+' && codepoints_p[1] == '+')
        ||  (codepoints_p[0] == '-' && codepoints_p[1] == '-')
        ||  (codepoints_p[0] == '<' && codepoints_p[1] == '<')
        ||  (codepoints_p[0] == '>' && codepoints_p[1] == '>')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '&')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '|')
        ||  (codepoints_p[0] == '?' && codepoints_p[1] == '?')
        ||  (codepoints_p[0] == '+' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '-' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '*' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '%' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '&' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '|' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '^' && codepoints_p[1] == '=')
        ||  (codepoints_p[0] == '=' && codepoints_p[1] == '>'))
        {
            NH_WEB_END(2)
        }
        if (codepoints_p[0] == '?' && codepoints_p[1] == '.')
        { 
            if (length > 2 && Nh_Web_isDigit(codepoints_p[2], NH_TRUE)) {}
            else {NH_WEB_END(2)}
        }
    }

    if (codepoints_p[0] == '{'
    ||  codepoints_p[0] == '}'
    ||  codepoints_p[0] == '('
    ||  codepoints_p[0] == ')'
    ||  codepoints_p[0] == '['
    ||  codepoints_p[0] == ']'
    ||  codepoints_p[0] == '.'
    ||  codepoints_p[0] == ';'
    ||  codepoints_p[0] == ','
    ||  codepoints_p[0] == '<' 
    ||  codepoints_p[0] == '>' 
    ||  codepoints_p[0] == '+' 
    ||  codepoints_p[0] == '-' 
    ||  codepoints_p[0] == '*' 
    ||  codepoints_p[0] == '%' 
    ||  codepoints_p[0] == '&' 
    ||  codepoints_p[0] == '|' 
    ||  codepoints_p[0] == '^' 
    ||  codepoints_p[0] == '!' 
    ||  codepoints_p[0] == '~' 
    ||  codepoints_p[0] == '?' 
    ||  codepoints_p[0] == ':' 
    ||  codepoints_p[0] == '=') 
    {
        NH_WEB_END(1)
    }

NH_WEB_END(0)
}

static NH_BOOL Nh_Web_isHexDigit(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_WEB_BEGIN()

    if (codepoint == '0'
    ||  codepoint == '1'
    ||  codepoint == '2'
    ||  codepoint == '3'
    ||  codepoint == '4'
    ||  codepoint == '5'
    ||  codepoint == '6'
    ||  codepoint == '7'
    ||  codepoint == '8'
    ||  codepoint == '9'
    ||  codepoint == 'a'
    ||  codepoint == 'b'
    ||  codepoint == 'c'
    ||  codepoint == 'd'
    ||  codepoint == 'e'
    ||  codepoint == 'f'
    ||  codepoint == 'A'
    ||  codepoint == 'B'
    ||  codepoint == 'C'
    ||  codepoint == 'D'
    ||  codepoint == 'E'
    ||  codepoint == 'F') {NH_WEB_END(NH_TRUE)}

NH_WEB_END(NH_FALSE)
}

static unsigned int Nh_Web_isHexDigits(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    int digits = 0;

    while (length >= 1 && Nh_Web_isHexDigit(codepoints_p[digits])) {
        digits++;
        length--;
    }

NH_WEB_END(digits)
}

static unsigned int Nh_Web_isHexIntegerLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    int digits = 0;

    if (length > 2 && codepoints_p[0] == '0' && (codepoints_p[1] == 'x' || codepoints_p[1] == 'X')) {
        digits = Nh_Web_isHexDigits(&codepoints_p[2], length - 2);
    }
    if (digits > 0) {digits += 2;}

NH_WEB_END(digits)
}

// https://tc39.es/ecma262/#sec-literals-numeric-literals
static unsigned int Nh_Web_isNumericLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length, NH_WEB_TOKEN *type_p)
{
NH_WEB_BEGIN()

// NonDecimalHexIntegerLiteral

    int digits = Nh_Web_isHexIntegerLiteral(codepoints_p, length);
    if (digits > 0) {
        *type_p = NH_WEB_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL;
        NH_WEB_END(digits)
    }

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_WEB_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        NH_WEB_END(2)
    }

    if (Nh_Web_isDigit(codepoints_p[0], NH_FALSE) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            digits = Nh_Web_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_WEB_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                NH_WEB_END(digits + 3)
            } 
        }

        digits = Nh_Web_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_WEB_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            NH_WEB_END(digits + 2)
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        digits = Nh_Web_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += Nh_Web_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_WEB_TOKEN_DECIMAL_LITERAL;
            NH_WEB_END(digits)
        }
    }

    digits = Nh_Web_isDecimalIntegerLiteral(codepoints_p, length);
    if (digits > 0) 
    {
        *type_p = NH_WEB_TOKEN_DECIMAL_LITERAL;

        if (length > digits && codepoints_p[digits] == '.') {
            digits++;
            digits += Nh_Web_isDigits(&codepoints_p[digits], length - digits);
            digits += Nh_Web_isExponentPart(&codepoints_p[digits], length - digits);
            NH_WEB_END(digits)
        }

        digits += Nh_Web_isExponentPart(&codepoints_p[digits], length - digits);
        NH_WEB_END(digits)
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isStringLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    if (codepoints_p[0] == '"') {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == '"') {break;}
        }
        if (count > 1) {NH_WEB_END(count)}
    }

    if (codepoints_p[0] == 39) {
        int count = 1;
        while (count < length) {
            if (codepoints_p[count++] == 39) {break;}
        }
        if (count > 1) {NH_WEB_END(count)}
    }

NH_WEB_END(0)
}

static unsigned int Nh_Web_isIdentifierStart(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_WEB_BEGIN()

    if (codepoint == '$' || codepoint == '_') {NH_WEB_END(NH_TRUE)}
    if (Nh_unicodeCodepointID_START(codepoint)) {NH_WEB_END(NH_TRUE)}
    if (codepoint == 92) { // 92 == \
        // unicode esc seq
    }

NH_WEB_END(NH_FALSE)
} 

static unsigned int Nh_Web_isIdentifierPart(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_WEB_BEGIN()

    if (codepoint == '$') {NH_WEB_END(NH_TRUE)}
    if (Nh_unicodeCodepointID_CONTINUE(codepoint)) {NH_WEB_END(NH_TRUE)}
    if (codepoint == 92) {
        // unicode esc seq
    }

NH_WEB_END(NH_FALSE)
} 

// https://tc39.es/ecma262/#sec-names-and-keywords
static unsigned int Nh_Web_isIdentifierName(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    int index = 0;
    while (Nh_Web_isIdentifierStart(codepoints_p[index]) && index < length) {
        index++;
    }
    
    if (index > 0) {
        while ((Nh_Web_isIdentifierStart(codepoints_p[index]) || Nh_Web_isIdentifierPart(codepoints_p[index])) && index < length) {
            index++;
        }
    }

NH_WEB_END(index)
}

static unsigned int Nh_Web_getToken(
    Nh_Array *Tokens_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_WEB_BEGIN()

    NH_WEB_TOKEN type;
    unsigned int count = 0;

    count = Nh_Web_isWhiteSpace(codepoints_p[0]);
    if (count) {
        type = NH_WEB_TOKEN_WHITE_SPACE;
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isLineTerminator(codepoints_p, length);
    if (count) {
        type = NH_WEB_TOKEN_LINE_TERMINATOR;
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isComment(codepoints_p, length);
    if (count) {
        type = NH_WEB_TOKEN_COMMENT;
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isPunctuator(codepoints_p, length);
    if (count) {
        type = NH_WEB_TOKEN_PUNCTUATOR;
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isNumericLiteral(codepoints_p, length, &type);
    if (count) {
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isStringLiteral(codepoints_p, length);
    if (count) {
        type = NH_WEB_TOKEN_STRING;
        goto DEFINE_TOKEN;
    } 

    count = Nh_Web_isIdentifierName(codepoints_p, length);
    if (count) {
        type = NH_WEB_TOKEN_IDENTIFIER;
        goto DEFINE_TOKEN;
    } 

DEFINE_TOKEN: ;

    Nh_Web_Token *Token_p = Nh_incrementArray(Tokens_p);
    Token_p->type = type;
    Token_p->String = Nh_encodeTextToUTF8(codepoints_p, count);

NH_WEB_END(count)
}

static Nh_Array Nh_Web_getTokens(
    Nh_UnicodeString Codepoints)
{
NH_WEB_BEGIN()

    Nh_Array Tokens = Nh_initArray(sizeof(Nh_Web_Token), 64);

    int index = 0;
    while (index < Codepoints.length) {
        index += Nh_Web_getToken(&Tokens, &Codepoints.p[index], Codepoints.length - index);
    }

NH_WEB_END(Tokens)
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

static Nh_Array Nh_Web_discardRedundantTokens(
    Nh_Array DirtyTokens)
{
NH_WEB_BEGIN()

    Nh_Array CleanTokens = Nh_initArray(sizeof(Nh_Web_Token), 64);

    for (int i = 0; i < DirtyTokens.length; ++i) 
    {
        Nh_Web_Token *Token_p = &((Nh_Web_Token*)DirtyTokens.bytes_p)[i];
        if (Token_p->type == NH_WEB_TOKEN_WHITE_SPACE || Token_p->type == NH_WEB_TOKEN_COMMENT || Token_p->type == NH_WEB_TOKEN_LINE_TERMINATOR) {
            continue;
        }

        Nh_Web_Token *TokenCopy_p = Nh_incrementArray(&CleanTokens);
        TokenCopy_p->type   = Token_p->type;
        TokenCopy_p->String = Nh_initString(32);

        Nh_appendToString(
            &TokenCopy_p->String, Token_p->String.bytes_p, Token_p->String.length
        );
    }

NH_WEB_END(CleanTokens)
}

// TOKENIZE ========================================================================================

Nh_Array Nh_Web_tokenizeFragment(
    NH_BYTE *logName_p, NH_BYTE *fragment_p)
{
NH_WEB_BEGIN()

    Nh_UnicodeString Codepoints = Nh_decodeUTF8Text(fragment_p);

    Nh_Array DirtyTokens = Nh_Web_getTokens(Codepoints);
    Nh_Web_logTokens(logName_p, &DirtyTokens, NH_TRUE);

    Nh_Array CleanTokens = Nh_Web_discardRedundantTokens(DirtyTokens);
    Nh_Web_logTokens(logName_p, &CleanTokens, NH_FALSE);

    Nh_Web_freeTokens(DirtyTokens);
    Nh_freeUnicodeString(&Codepoints);

NH_WEB_END(CleanTokens)
}

void Nh_Web_freeTokens(
    Nh_Array Tokens)
{
NH_WEB_BEGIN()

    for (int i = 0; i < Tokens.length; ++i)
    {
        Nh_Web_Token *Token_p = &((Nh_Web_Token*)Tokens.bytes_p)[i];
        Nh_freeString(&Token_p->String);
    }

    Nh_freeArray(&Tokens);

NH_WEB_SILENT_END()
}

// IS NUMERIC LITERAL ==============================================================================

NH_BOOL Nh_Web_isNumericToken(
    Nh_Web_Token *Token_p)
{
NH_WEB_BEGIN()

    if (Token_p->type >= NH_WEB_TOKEN_DECIMAL_LITERAL && Token_p->type <= NH_WEB_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
        NH_WEB_END(NH_TRUE)
    }

NH_WEB_END(NH_FALSE)
}

