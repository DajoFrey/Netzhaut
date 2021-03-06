#ifndef NH_WEB_TOKENIZER_H
#define NH_WEB_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Unicode.h"

#endif

/** @addtogroup NhWebEnums
 *  @{
 */

    typedef enum NH_WEB_TOKEN {
        NH_WEB_TOKEN_WHITE_SPACE = 0,
        NH_WEB_TOKEN_LINE_TERMINATOR,
        NH_WEB_TOKEN_COMMENT,
        NH_WEB_TOKEN_IDENTIFIER,
        NH_WEB_TOKEN_PUNCTUATOR,
        NH_WEB_TOKEN_DECIMAL_LITERAL,
        NH_WEB_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
        NH_WEB_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
        NH_WEB_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
        NH_WEB_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
        NH_WEB_TOKEN_STRING,
    } NH_WEB_TOKEN;

/** @} */

/** @addtogroup NhWebStructs
 *  @{
 */

    typedef struct Nh_Web_Token {
        NH_WEB_TOKEN type;
        Nh_UTF8String String;
    } Nh_Web_Token;

/** @} */

/** @addtogroup NhWebVars
 *  @{
 */

    extern const NH_BYTE *NH_WEB_TOKENS_PP[];

/** @} */

/** @addtogroup NhWebFunctions
 *  @{
 */

    Nh_Array Nh_Web_tokenizeFragment(
        NH_BYTE *logName_p, NH_BYTE *fragment_p
    );
    
    void Nh_Web_freeTokens(
        Nh_Array Tokens
    );

    NH_BOOL Nh_Web_isNumericToken(
        Nh_Web_Token *Token_p
    );

/** @} */

#endif
