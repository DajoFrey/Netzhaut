#ifndef NH_UNICODE_H
#define NH_UNICODE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "String.h"

#include "Common/API.h"
#include "Common/Result.h"
#include "Common/API.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup NhCoreMacros
 *  @{
 */

    #define NH_UTF8_INVALID 0xFFFD
    #define NH_UTF8_SIZE    4

/** @} */

/** @addtogroup NhCoreEnums
 *  @{
 */

    typedef enum NH_UNICODE_ENCODING {
        NH_UNICODE_ENCODING_UTF8,
        NH_UNICODE_ENCODING_UTF16,
    } NH_UNICODE_ENCODING;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

// UTF8
    size_t Nh_validateUTF8(
        NH_UNICODE_CODEPOINT *u, size_t i
    );
    
    size_t Nh_decodeUTF8(
        const NH_BYTE *c, NH_UNICODE_CODEPOINT *u, size_t clen
    );
    
    size_t Nh_encodeToUTF8(
        NH_UNICODE_CODEPOINT u, NH_BYTE *c
    );

    Nh_UnicodeString Nh_decodeUTF8Text(
        NH_BYTE *bytes_p
    );

    Nh_UTF8String Nh_encodeTextToUTF8(
        NH_UNICODE_CODEPOINT *unicodeCodepoints_p, size_t length
    );

// Unicode String

    Nh_UnicodeString Nh_initUnicodeString(
        int chunkSize
    );
    
    NH_RESULT Nh_appendToUnicodeString(
        Nh_UnicodeString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, int length
    );
    
    void Nh_freeUnicodeString(
        Nh_UnicodeString *String_p
    );

    NH_RESULT Nh_removeTailFromUnicodeString(
        Nh_UnicodeString *String_p, unsigned int count
    );

// Unicode
    const NH_BYTE *Nh_getUnicodeCodepointDescription(
        NH_UNICODE_CODEPOINT codepoint
    );

    NH_BOOL Nh_unicodeCodepointID_START(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_unicodeCodepointID_CONTINUE(
        NH_UNICODE_CODEPOINT codepoint
    );

    NH_RESULT Nh_logUnicodeCodepoints(
    );

/** @} */

#endif 
