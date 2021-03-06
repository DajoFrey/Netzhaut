#ifndef NH_WEB_STRING_H
#define NH_WEB_STRING_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Common/API.h"
#include "../Common/Result.h"

#include <stdint.h>
#include <stddef.h>

#endif

/** @addtogroup NhWebFunctions
 *  @{
 */

    Nh_Web_DOMString Nh_Web_initDOMString(
        int chunkSize
    );

    NH_WEB_RESULT Nh_Web_appendToDOMString(
        Nh_Web_DOMString *String_p, NH_BYTE *bytes_p, int length
    );

    NH_WEB_RESULT Nh_Web_appendUnicodeToDOMString(
        Nh_Web_DOMString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length
    );

    void Nh_Web_freeDOMString(
        Nh_Web_DOMString *String_p
    );

    Nh_Web_DOMString Nh_Web_encodeTextToDOMString(
        NH_UNICODE_CODEPOINT *unicodeCodepoints_p, size_t length
    );

    Nh_Web_USVString Nh_Web_initUSVString(
        int chunkSize
    );
    
    NH_WEB_RESULT Nh_Web_appendToUSVString(
        Nh_Web_USVString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length
    );
    
    void Nh_Web_freeUSVString(
        Nh_Web_USVString *String_p
    );

/** @} */

#endif 
