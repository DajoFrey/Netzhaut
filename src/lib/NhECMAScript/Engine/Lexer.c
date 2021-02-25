// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Lexer.h"

#include "../Common/Macro.h"
#include "../../NhCore/Unicode.h"

#include NH_ECMASCRIPT_DEFAULT_CHECK
#include NH_ECMASCRIPT_FLOW

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

typedef enum NH_ECMASCRIPT_GOAL_SYMBOL {
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_REG_EXP,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_REG_EXP_OR_TEMPLATE_TAIL,
    NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_TEMPLATE_TAIL,
} NH_ECMASCRIPT_GOAL_SYMBOL;

// DATA ============================================================================================

const NH_BYTE *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[] = {
    "Whitespace",
    "LineTerminator",
    "Comment",
    "TokenIdentifierName",
    "TokenPunctuator",
    "TokenDecimalLiteral",
    "TokenDecimalBigIntegerLiteral",
    "TokenNonDecimalBinaryIntegerLiteral",
    "TokenNonDecimalOctalIntegerLiteral",
    "TokenNonDecimalHexIntegerLiteral",
    "TokenStringLiteral",
    "TokenTemplate",
    "TokenDivPunctuator",
    "TokenRegularExpressionLiteral",
    "TokenRightBracePunctuator",
    "TokenTemplateSubstitutionTail",
};

// LEXER ===========================================================================================

static NH_BOOL Nh_ECMAScript_isDigit(
    NH_UNICODE_CODEPOINT codepoint, NH_BOOL zero)
{
NH_ECMASCRIPT_BEGIN()

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
        NH_ECMASCRIPT_END(NH_TRUE)
    }

    if (zero && codepoint == '0') {NH_ECMASCRIPT_END(NH_TRUE)}

NH_ECMASCRIPT_END(NH_FALSE)
}

static unsigned int Nh_ECMAScript_isDigits(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (Nh_ECMAScript_isDigit(codepoints_p[0], NH_TRUE)) {
        unsigned int i = 1;
        while (i < length && (Nh_ECMAScript_isDigit(codepoints_p[i], NH_TRUE) || codepoints_p[i] == '_')) {
            i++;
        }
        NH_ECMASCRIPT_END(i)
    }

NH_ECMASCRIPT_END(0)
}

static unsigned int Nh_ECMAScript_isSignedInteger(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int offset = 0;
    if (codepoints_p[0] == '+' || codepoints_p[0] == '-') {
        offset++;
    }
    int digits = Nh_ECMAScript_isDigits(offset == 0 ? codepoints_p : &codepoints_p[1], NH_TRUE);
    if (digits == 0) {NH_ECMASCRIPT_END(0)}

NH_ECMASCRIPT_END(offset + digits)
}

static unsigned int Nh_ECMAScript_isExponentPart(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] != 'e' && codepoints_p[0] != 'E') {
        NH_ECMASCRIPT_END(0)
    }
    int digits = Nh_ECMAScript_isSignedInteger(&codepoints_p[1], NH_TRUE);

NH_ECMASCRIPT_END(digits)
}

static unsigned int Nh_ECMAScript_isDecimalIntegerLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == '0') {NH_ECMASCRIPT_END(1)}

    if (Nh_ECMAScript_isDigit(codepoints_p[0], NH_FALSE)) 
    {
        int offset = 1;
        if (length > 1 && codepoints_p[1] == '_') {
            offset++;
        }
        int digits = Nh_ECMAScript_isDigits(&codepoints_p[offset], length - offset);
        if (digits == 0 && offset == 2) {NH_ECMASCRIPT_END(0)}
        NH_ECMASCRIPT_END(digits + offset)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-white-space
static unsigned int Nh_ECMAScript_isWhiteSpace(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == 0x0009  // CHARACTER TABULATION <TAB> 
    ||  codepoint == 0x000B  // LINE TABULATION <VT> 
    ||  codepoint == 0x000C  // FORM FEED (FF) 
    ||  codepoint == 0x0020  // SPACE <SP> 
    ||  codepoint == 0x00A0  // NO-BREAK SPACE <NBSP>
    ||  codepoint == 0xFEFF) // ZERO WIDTH NO-BREAK SPACE <ZWNBSP>
    {
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-line-terminators
static unsigned int Nh_ECMAScript_isLineTerminator(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()


    if (codepoints_p[0] == 0x000A  // LINE FEED <LF> 
    ||  codepoints_p[0] == 0x000D  // CARRIAGE RETURN (CR) <CR> 
    ||  codepoints_p[0] == 0x2028  // LINE SEPERATOR (LS) <LS>
    ||  codepoints_p[0] == 0x2029) // PARAGRAPH SEPERATOR <PS> 
    {
        if (length > 1 && codepoints_p[0] == 0x000D && codepoints_p[1] == 0x000A) {
            NH_ECMASCRIPT_END(2)
        }
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-comments
static unsigned int Nh_ECMAScript_isComment(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == 0x002F) // '/'
    {
        // single line comment
        if (length > 1 && codepoints_p[1] == 0x002F) 
        {
            unsigned int i = 2;
            while (i < length && !Nh_ECMAScript_isLineTerminator(&codepoints_p[i], length - i)) {
                i++;
            }
            NH_ECMASCRIPT_END(i)
        }

        // multi line comment 
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-punctuators
static unsigned int Nh_ECMAScript_isPunctuator(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (length > 3) {
        if (codepoints_p[0] == '>' && codepoints_p[1] == '>' && codepoints_p[2] == '>' && codepoints_p[3] == '=')
        {
            NH_ECMASCRIPT_END(4)
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
            NH_ECMASCRIPT_END(3)
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
            NH_ECMASCRIPT_END(2)
        }
        if (codepoints_p[0] == '?' && codepoints_p[1] == '.')
        { 
            if (length > 2 && Nh_ECMAScript_isDigit(codepoints_p[2], NH_TRUE)) {}
            else {NH_ECMASCRIPT_END(2)}
        }
    }

    if (codepoints_p[0] == '{'
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
        NH_ECMASCRIPT_END(1)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-literals-numeric-literals
static unsigned int Nh_ECMAScript_isNumericLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length, NH_ECMASCRIPT_INPUT_ELEMENT *type_p)
{
NH_ECMASCRIPT_BEGIN()

// DecimalBigIntegerLiteral
    if (length > 1 && codepoints_p[0] == '0' && codepoints_p[1] == 'n') 
    {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
        NH_ECMASCRIPT_END(2)
    }

    if (Nh_ECMAScript_isDigit(codepoints_p[0], NH_FALSE) && length > 1) 
    {
        if (codepoints_p[1] == '_') {
            int digits = Nh_ECMAScript_isDigits(&codepoints_p[2], length - 2);
            if (digits > 0 && length > digits + 2 && codepoints_p[digits + 2] == 'n') {
                *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
                NH_ECMASCRIPT_END(digits + 3)
            } 
        }

        int digits = Nh_ECMAScript_isDigits(&codepoints_p[1], length - 1);
        if (length > digits + 1 && codepoints_p[digits + 1] == 'n') {
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL;
            NH_ECMASCRIPT_END(digits + 2)
        } 
    }

// DecimalLiteral
    if (codepoints_p[0] == '.')
    {
        int digits = Nh_ECMAScript_isDigits(&codepoints_p[1], length - 1) + 1;
        if (digits > 1) {
            digits += Nh_ECMAScript_isExponentPart(&codepoints_p[digits], length - digits);
            *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL;
            NH_ECMASCRIPT_END(digits)
        }
    }

    int digits = Nh_ECMAScript_isDecimalIntegerLiteral(codepoints_p, length);
    if (digits > 0) 
    {
        *type_p = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL;

        if (length > digits && codepoints_p[digits] == '.') {
            digits++;
            digits += Nh_ECMAScript_isDigits(&codepoints_p[digits], length - digits);
            digits += Nh_ECMAScript_isExponentPart(&codepoints_p[digits], length - digits);
            NH_ECMASCRIPT_END(digits)
        }

        digits += Nh_ECMAScript_isExponentPart(&codepoints_p[digits], length - digits);
        NH_ECMASCRIPT_END(digits)
    }

NH_ECMASCRIPT_END(0)
}

// https://tc39.es/ecma262/#sec-literals-string-literals
static unsigned int Nh_ECMAScript_isStringLiteral(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoints_p[0] == '"') {
        int count = 1;
        while (count < length && codepoints_p[count] != '"') {
            ++count;
        }
        NH_ECMASCRIPT_END(count)
    }

    if (codepoints_p[0] == 39) { // 39 == '
        int count = 1;
        while (count < length && codepoints_p[count] != 39) {
            ++count;
        }
        NH_ECMASCRIPT_END(count)
    }

NH_ECMASCRIPT_END(0)
}

static unsigned int Nh_ECMAScript_isIdentifierStart(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == '$' || codepoint == '_') {NH_ECMASCRIPT_END(NH_TRUE)}
    if (Nh_unicodeCodepointID_START(codepoint)) {NH_ECMASCRIPT_END(NH_TRUE)}
    if (codepoint == 92) { // 92 == \
        // unicode esc seq
    }

NH_ECMASCRIPT_END(NH_FALSE)
} 

static unsigned int Nh_ECMAScript_isIdentifierPart(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_ECMASCRIPT_BEGIN()

    if (codepoint == '$') {NH_ECMASCRIPT_END(NH_TRUE)}
    if (Nh_unicodeCodepointID_CONTINUE(codepoint)) {NH_ECMASCRIPT_END(NH_TRUE)}
    if (codepoint == 92) {
        // unicode esc seq
    }

NH_ECMASCRIPT_END(NH_FALSE)
} 

// https://tc39.es/ecma262/#sec-names-and-keywords
static unsigned int Nh_ECMAScript_isIdentifierName(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    int index = 0;
    while (Nh_ECMAScript_isIdentifierStart(codepoints_p[index]) && index < length) {
        index++;
    }
    
    if (index > 0) {
        while ((Nh_ECMAScript_isIdentifierStart(codepoints_p[index]) || Nh_ECMAScript_isIdentifierPart(codepoints_p[index])) && index < length) {
            index++;
        }
    }

NH_ECMASCRIPT_END(index)
}

static unsigned int Nh_ECMAScript_isRightBracePunctuator(
    NH_UNICODE_CODEPOINT *codepoints_p, unsigned int length)
{
NH_ECMASCRIPT_BEGIN()
NH_ECMASCRIPT_END(length > 0 && codepoints_p[0] == '}' ? 1 : 0)
}

static unsigned int Nh_ECMAScript_getInputElement(
    Nh_Array *InputElements_p, NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol, NH_UNICODE_CODEPOINT *codepoints_p, 
    unsigned int length)
{
NH_ECMASCRIPT_BEGIN()

    NH_ECMASCRIPT_INPUT_ELEMENT type;
    unsigned int count = 0;

    if (goalSymbol == NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV) 
    {
        count = Nh_ECMAScript_isWhiteSpace(codepoints_p[0]);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isLineTerminator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_LINE_TERMINATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isComment(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_COMMENT;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isPunctuator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_PUNCTUATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isNumericLiteral(codepoints_p, length, &type);
        if (count) {
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isStringLiteral(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isIdentifierName(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME;
            goto DEFINE_INPUT_ELEMENT;
        } 

        count = Nh_ECMAScript_isRightBracePunctuator(codepoints_p, length);
        if (count) {
            type = NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_RIGHT_BRACE_PUNCTUATOR;
            goto DEFINE_INPUT_ELEMENT;
        } 
    }

DEFINE_INPUT_ELEMENT: ;

    Nh_ECMAScript_InputElement *InputElement_p = Nh_getFromArray(InputElements_p, -1);
    InputElement_p->type = type;
    InputElement_p->String = Nh_encodeTextToUTF8(codepoints_p, count);

NH_ECMASCRIPT_END(count)
}

Nh_Array Nh_ECMAScript_getInputElements(
    Nh_Array UnicodeCodepoints)
{
NH_ECMASCRIPT_BEGIN()

    Nh_Array InputElements = Nh_initArray(sizeof(Nh_ECMAScript_InputElement), 64);

    int index = 0;
    while (index < UnicodeCodepoints.length) 
    {
        NH_ECMASCRIPT_GOAL_SYMBOL goalSymbol = NH_ECMASCRIPT_GOAL_SYMBOL_INPUT_ELEMENT_DIV;

        index += Nh_ECMAScript_getInputElement(
            &InputElements, goalSymbol, &((NH_UNICODE_CODEPOINT*)UnicodeCodepoints.bytes_p)[index], 
            UnicodeCodepoints.length - index 
        );
    }

NH_ECMASCRIPT_END(InputElements)
}

// DISCARD REDUNDANT INPUT ELEMENTS ================================================================

// https://tc39.es/ecma262/#sec-lexical-and-regexp-grammars
Nh_Array Nh_ECMAScript_discardRedundantInputElements(
    Nh_Array DirtyInputElements)
{
NH_ECMASCRIPT_BEGIN()

    Nh_Array CleanInputElements = Nh_initArray(sizeof(Nh_ECMAScript_InputElement), 64);

    for (int i = 0; i < DirtyInputElements.length; ++i) 
    {
        Nh_ECMAScript_InputElement *InputElement_p = &((Nh_ECMAScript_InputElement*)DirtyInputElements.bytes_p)[i];
        if (InputElement_p->type == NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE) {
            continue;
        }

        Nh_ECMAScript_InputElement *InputElementCopy_p = Nh_getFromArray(&CleanInputElements, -1);
        InputElementCopy_p->type = InputElement_p->type;
        InputElementCopy_p->String = Nh_initString(32);

        Nh_appendToString(
            &InputElementCopy_p->String, InputElement_p->String.bytes_p, InputElement_p->String.length
        );
    }

NH_ECMASCRIPT_END(CleanInputElements)
}

// IS NUMERIC LITERAL ==============================================================================

NH_BOOL Nh_ECMAScript_isNumericToken(
    Nh_ECMAScript_InputElement *InputElement_p)
{
NH_ECMASCRIPT_BEGIN()

    if (InputElement_p->type >= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL && InputElement_p->type <= NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL) {
        NH_ECMASCRIPT_END(NH_TRUE)
    }

NH_ECMASCRIPT_END(NH_FALSE)
}


