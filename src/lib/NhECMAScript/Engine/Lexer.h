#ifndef NH_ECMASCRIPT_LEXER_H
#define NH_ECMASCRIPT_LEXER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Type.h"

#endif

/** @addtogroup NhECMAScriptEnums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_INPUT_ELEMENT {
        NH_ECMASCRIPT_INPUT_ELEMENT_WHITE_SPACE = 0,
        NH_ECMASCRIPT_INPUT_ELEMENT_LINE_TERMINATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_COMMENT,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_IDENTIFIER_NAME,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_STRING_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_TEMPLATE,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_DIV_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_REGULAR_EXPRESSION_LITERAL,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_RIGHT_BRACE_PUNCTUATOR,
        NH_ECMASCRIPT_INPUT_ELEMENT_TOKEN_TEMPLATE_SUBSTITUTION_TAIL,
    } NH_ECMASCRIPT_INPUT_ELEMENT;

/** @} */

/** @addtogroup NhECMAScriptStructs
 *  @{
 */

    typedef struct Nh_ECMAScript_InputElement {
        NH_ECMASCRIPT_INPUT_ELEMENT type;
        Nh_UTF8String String;
    } Nh_ECMAScript_InputElement;

/** @} */

/** @addtogroup NhECMAScriptVars
 *  @{
 */

    extern const NH_BYTE *NH_ECMASCRIPT_INPUT_ELEMENTS_PP[];

/** @} */

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    Nh_Array Nh_ECMAScript_getInputElements(
        Nh_Array UnicodeCodepoints
    );

    Nh_Array Nh_ECMAScript_discardRedundantInputElements(
        Nh_Array DirtyInputElements
    );

    NH_BOOL Nh_ECMAScript_isNumericToken(
        Nh_ECMAScript_InputElement *InputElement_p
    );

/** @} */

#endif
