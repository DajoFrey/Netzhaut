// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "String.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Common/Macro.h"
#include NH_FLOW2
#include NH_CUSTOM_CHECK2

// NAMESPACES ======================================================================================

Nh_WebIDL_DOMString NH_WEBIDL_HTML_NAMESPACE;
Nh_WebIDL_DOMString NH_WEBIDL_MATHML_NAMESPACE;
Nh_WebIDL_DOMString NH_WEBIDL_SVG_NAMESPACE;
Nh_WebIDL_DOMString NH_WEBIDL_XLINK_NAMESPACE;
Nh_WebIDL_DOMString NH_WEBIDL_XML_NAMESPACE;
Nh_WebIDL_DOMString NH_WEBIDL_XMLNS_NAMESPACE;

// INIT NAMESPACES =================================================================================

NH_WEBIDL_RESULT Nh_WebIDL_initNamespaces()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_HTML_NAMESPACE   = Nh_WebIDL_initDOMString(29);
    NH_WEBIDL_MATHML_NAMESPACE = Nh_WebIDL_initDOMString(35);
    NH_WEBIDL_SVG_NAMESPACE    = Nh_WebIDL_initDOMString(27);
    NH_WEBIDL_XLINK_NAMESPACE  = Nh_WebIDL_initDOMString(29);
    NH_WEBIDL_XML_NAMESPACE    = Nh_WebIDL_initDOMString(36);
    NH_WEBIDL_XMLNS_NAMESPACE  = Nh_WebIDL_initDOMString(30);

    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_HTML_NAMESPACE,   "http://www.w3.org/1999/xhtml", 28))
    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_MATHML_NAMESPACE, "http://www.w3.org/1998/Math/MathML", 34))
    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_SVG_NAMESPACE,    "http://www.w3.org/2000/svg", 26))
    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_XLINK_NAMESPACE,  "http://www.w3.org/1999/xlink", 28))
    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_XML_NAMESPACE,    "http://www.w3.org/XML/1998/namespace", 36))
    NH_WEBIDL_CHECK(Nh_WebIDL_appendToDOMString(&NH_WEBIDL_XMLNS_NAMESPACE,  "http://www.w3.org/2000/xmlns/", 29))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

// DOM STRING ======================================================================================

Nh_WebIDL_DOMString Nh_WebIDL_initDOMString(
    int chunkSize)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(Nh_initString(chunkSize))
}

NH_WEBIDL_RESULT Nh_WebIDL_appendToDOMString(
    Nh_WebIDL_DOMString *String_p, NH_BYTE *bytes_p, int length)
{
NH_WEBIDL_BEGIN()

    NH_CHECK(NH_WEBIDL_ERROR_BAD_STATE, Nh_appendToString(String_p, bytes_p, length))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

NH_WEBIDL_RESULT Nh_WebIDL_appendUnicodeToDOMString(
    Nh_WebIDL_DOMString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length)
{
NH_WEBIDL_BEGIN()

    Nh_UTF8String String = Nh_encodeTextToUTF8(codepoints_p, length);
    NH_CHECK(NH_WEBIDL_ERROR_BAD_STATE, Nh_appendToString(String_p, String.bytes_p, String.length))
    Nh_freeString(&String);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

void Nh_WebIDL_freeDOMString(
    Nh_WebIDL_DOMString *String_p)
{
NH_WEBIDL_BEGIN()

    Nh_freeString(String_p);

NH_WEBIDL_SILENT_END()
}

Nh_WebIDL_DOMString Nh_WebIDL_encodeTextToDOMString(
    NH_UNICODE_CODEPOINT *unicodeCodepoints_p, size_t length)
{
NH_BEGIN()
NH_END(Nh_encodeTextToUTF8(unicodeCodepoints_p, length))
}

// USV STRING ======================================================================================

Nh_WebIDL_USVString Nh_WebIDL_initUSVString(
    int chunkSize)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(Nh_initUnicodeString(chunkSize))
}

NH_WEBIDL_RESULT Nh_WebIDL_appendToUSVString(
    Nh_WebIDL_USVString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length)
{
NH_WEBIDL_BEGIN()

    NH_CHECK(NH_WEBIDL_ERROR_BAD_STATE, Nh_appendToUnicodeString(String_p, codepoints_p, length))

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

void Nh_WebIDL_freeUSVString(
    Nh_WebIDL_USVString *String_p)
{
NH_WEBIDL_BEGIN()

    Nh_freeUnicodeString(String_p);

NH_WEBIDL_SILENT_END()
}

