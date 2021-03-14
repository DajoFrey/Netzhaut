#ifndef NH_WEBIDL_TOKENIZER_H
#define NH_WEBIDL_TOKENIZER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../NhCore/Unicode.h"

#endif

/** @addtogroup NhWebIDLEnums
 *  @{
 */

    typedef enum NH_WEBIDL_TOKEN {
        NH_WEBIDL_TOKEN_WHITE_SPACE = 0,
        NH_WEBIDL_TOKEN_LINE_TERMINATOR,
        NH_WEBIDL_TOKEN_COMMENT,
        NH_WEBIDL_TOKEN_IDENTIFIER,
        NH_WEBIDL_TOKEN_PUNCTUATOR,
        NH_WEBIDL_TOKEN_DECIMAL_LITERAL,
        NH_WEBIDL_TOKEN_DECIMAL_BIG_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_BINARY_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_OCTAL_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_NON_DECIMAL_HEX_INTEGER_LITERAL,
        NH_WEBIDL_TOKEN_STRING,
    } NH_WEBIDL_TOKEN;

/** @} */

/** @addtogroup NhWebIDLStructs
 *  @{
 */

    typedef struct Nh_WebIDL_Token {
        NH_WEBIDL_TOKEN type;
        Nh_Encoding_UTF8String String;
    } Nh_WebIDL_Token;

/** @} */

/** @addtogroup NhWebIDLVars
 *  @{
 */

    extern const NH_BYTE *NH_WEBIDL_TOKENS_PP[];

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    Nh_Array Nh_WebIDL_tokenizeFragment(
        NH_BYTE *logName_p, NH_BYTE *fragment_p
    );
    
    void Nh_WebIDL_freeTokens(
        Nh_Array Tokens
    );

    NH_BOOL Nh_WebIDL_isNumericToken(
        Nh_WebIDL_Token *Token_p
    );

/** @} */

#endif
