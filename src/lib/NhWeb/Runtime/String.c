// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "String.h"

#include "../Common/Macro.h"
#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Common/Macro.h"
#include NH_FLOW2
#include NH_CUSTOM_CHECK2

// DOM STRING ======================================================================================

Nh_Web_DOMString Nh_Web_initDOMString(
    int chunkSize)
{
NH_WEB_BEGIN()
NH_WEB_END(Nh_initString(chunkSize))
}

NH_WEB_RESULT Nh_Web_appendToDOMString(
    Nh_Web_DOMString *String_p, NH_BYTE *bytes_p, int length)
{
NH_WEB_BEGIN()

    NH_CHECK(NH_WEB_ERROR_BAD_STATE, Nh_appendToString(String_p, bytes_p, length))

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

NH_WEB_RESULT Nh_Web_appendUnicodeToDOMString(
    Nh_Web_DOMString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length)
{
NH_WEB_BEGIN()

    Nh_UTF8String String = Nh_encodeTextToUTF8(codepoints_p, length);
    NH_CHECK(NH_WEB_ERROR_BAD_STATE, Nh_appendToString(String_p, String.bytes_p, String.length))
    Nh_freeString(&String);

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

void Nh_Web_freeDOMString(
    Nh_Web_DOMString *String_p)
{
NH_WEB_BEGIN()

    Nh_freeString(String_p);

NH_WEB_SILENT_END()
}

Nh_Web_DOMString Nh_Web_encodeTextToDOMString(
    NH_UNICODE_CODEPOINT *unicodeCodepoints_p, size_t length)
{
NH_BEGIN()
NH_END(Nh_encodeTextToUTF8(unicodeCodepoints_p, length))
}

// USV STRING ======================================================================================

Nh_Web_USVString Nh_Web_initUSVString(
    int chunkSize)
{
NH_WEB_BEGIN()
NH_WEB_END(Nh_initUnicodeString(chunkSize))
}

NH_WEB_RESULT Nh_Web_appendToUSVString(
    Nh_Web_USVString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length)
{
NH_WEB_BEGIN()

    NH_CHECK(NH_WEB_ERROR_BAD_STATE, Nh_appendToUnicodeString(String_p, codepoints_p, length))

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

void Nh_Web_freeUSVString(
    Nh_Web_USVString *String_p)
{
NH_WEB_BEGIN()

    Nh_freeUnicodeString(String_p);

NH_WEB_SILENT_END()
}

