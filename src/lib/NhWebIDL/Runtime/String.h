#ifndef NH_WEBIDL_STRING_H
#define NH_WEBIDL_STRING_H

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

/** @addtogroup NhWebIDLVars
 *  @{
 */

    extern Nh_WebIDL_DOMString NH_WEBIDL_HTML_NAMESPACE;
    extern Nh_WebIDL_DOMString NH_WEBIDL_MATHML_NAMESPACE;
    extern Nh_WebIDL_DOMString NH_WEBIDL_SVG_NAMESPACE;
    extern Nh_WebIDL_DOMString NH_WEBIDL_XLINK_NAMESPACE;
    extern Nh_WebIDL_DOMString NH_WEBIDL_XML_NAMESPACE;
    extern Nh_WebIDL_DOMString NH_WEBIDL_XMLNS_NAMESPACE;

/** @} */

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    NH_WEBIDL_RESULT Nh_WebIDL_initNamespaces(
    );

    Nh_WebIDL_DOMString Nh_WebIDL_initDOMString(
        int chunkSize
    );

    NH_WEBIDL_RESULT Nh_WebIDL_appendToDOMString(
        Nh_WebIDL_DOMString *String_p, NH_BYTE *bytes_p, int length
    );

    NH_WEBIDL_RESULT Nh_WebIDL_appendUnicodeToDOMString(
        Nh_WebIDL_DOMString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length
    );

    void Nh_WebIDL_freeDOMString(
        Nh_WebIDL_DOMString *String_p
    );

    Nh_WebIDL_DOMString Nh_WebIDL_encodeTextToDOMString(
        NH_UNICODE_CODEPOINT *unicodeCodepoints_p, size_t length
    );

    Nh_WebIDL_USVString Nh_WebIDL_initUSVString(
        int chunkSize
    );
    
    NH_WEBIDL_RESULT Nh_WebIDL_appendToUSVString(
        Nh_WebIDL_USVString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length
    );
    
    void Nh_WebIDL_freeUSVString(
        Nh_WebIDL_USVString *String_p
    );

/** @} */

#endif 
