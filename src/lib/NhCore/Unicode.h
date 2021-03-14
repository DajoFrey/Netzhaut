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

/** @addtogroup NhCoreFunctions
 *  @{
 */

// UNICODE STRING

    Nh_UnicodeString Nh_initUnicodeString(
        int chunkSize
    );
    
    NH_RESULT Nh_appendToUnicodeString(
        Nh_UnicodeString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length
    );
    
    NH_UNICODE_CODEPOINT *Nh_incrementUnicodeString(
        Nh_UnicodeString *String_p
    );

    void Nh_freeUnicodeString(
        Nh_UnicodeString *String_p
    );

    NH_RESULT Nh_removeTailFromUnicodeString(
        Nh_UnicodeString *String_p, unsigned long count
    );

// UNICODE LOOKUP

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

// CHECKS 

    NH_BOOL Nh_isASCIIWhitespace(
        NH_UNICODE_CODEPOINT codepoint
    );

    NH_BOOL Nh_isASCIIUpperAlpha(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isASCIILowerAlpha(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isASCIIAlpha(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isASCIIDigit(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isASCIIAlphaNumeric(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isASCIICaseInsensitiveMatch(
        NH_BYTE *str1_p, NH_BYTE *str2_p
    );

    NH_BOOL Nh_isSurrogate(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isScalarValue(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isNonCharacter(
        NH_UNICODE_CODEPOINT codepoint
    );

    NH_BOOL Nh_isC0Control(
        NH_UNICODE_CODEPOINT codepoint
    );
    
    NH_BOOL Nh_isControl(
        NH_UNICODE_CODEPOINT codepoint
    );

/** @} */

#endif 
