// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Unicode.h"
#include "UnicodeLookup.h"

#include "Common/Log.h"
#include "Common/Macro.h"
#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdarg.h>
#include <ctype.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// UNICODE CODEPOINT STRING ========================================================================

Nh_UnicodeString Nh_initUnicodeString(
    int chunkSize)
{
NH_BEGIN()

    Nh_UnicodeString String;
    String.Array  = Nh_initArray(sizeof(NH_UNICODE_CODEPOINT), chunkSize);
    String.p      = (NH_UNICODE_CODEPOINT*)String.Array.bytes_p; 
    String.length = 0;

NH_END(String)
}

NH_RESULT Nh_appendToUnicodeString(
    Nh_UnicodeString *String_p, NH_UNICODE_CODEPOINT *codepoints_p, unsigned long length)
{
NH_BEGIN()

    NH_CHECK(Nh_appendToArray(&String_p->Array, codepoints_p, length))
    String_p->p = (NH_UNICODE_CODEPOINT*)String_p->Array.bytes_p; 
    String_p->length = String_p->Array.length;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_UNICODE_CODEPOINT *Nh_incrementUnicodeString(
    Nh_UnicodeString *String_p)
{
NH_BEGIN()

    NH_UNICODE_CODEPOINT *codepoint_p = Nh_incrementArray(&String_p->Array);
    String_p->p = (NH_UNICODE_CODEPOINT*)String_p->Array.bytes_p; 
    String_p->length = String_p->Array.length;

NH_END(codepoint_p)
}

void Nh_freeUnicodeString(
    Nh_UnicodeString *String_p)
{
NH_BEGIN()

    Nh_freeArray(&String_p->Array);
    String_p->p = NULL;
    String_p->length = 0;

NH_SILENT_END()
}

NH_RESULT Nh_removeTailFromUnicodeString(
    Nh_UnicodeString *String_p, unsigned long count)
{
NH_BEGIN()

    NH_CHECK(Nh_removeTailFromArray(&String_p->Array, count))
    String_p->p = (NH_UNICODE_CODEPOINT*)String_p->Array.bytes_p;
    String_p->length = String_p->Array.length;

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

// CHECKS ==========================================================================================

NH_BOOL Nh_isASCIIWhitespace(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint == 0x09 || codepoint == 0x0A || codepoint == 0x0C || codepoint == 0x0D || codepoint == 0x20)
}

NH_BOOL Nh_isASCIIUpperAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint >= 0x41 && codepoint <= 0x5A)
}

NH_BOOL Nh_isASCIILowerAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint >= 0x61 && codepoint <= 0x7A)
}

NH_BOOL Nh_isASCIIAlpha(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(Nh_isASCIIUpperAlpha(codepoint) || Nh_isASCIILowerAlpha(codepoint))
}

NH_BOOL Nh_isASCIIDigit(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint >= 0x30 && codepoint <= 0x39)
}

NH_BOOL Nh_isASCIIAlphaNumeric(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(Nh_isASCIIAlpha(codepoint) || Nh_isASCIIDigit(codepoint))
}

NH_BOOL Nh_isASCIICaseInsensitiveMatch(
    NH_BYTE *str1_p, NH_BYTE *str2_p)
{
NH_BEGIN()

    if (strlen(str1_p) != strlen(str2_p)) {NH_END(NH_FALSE)}

    for (int i = 0; i < strlen(str1_p); ++i) {
        int d = tolower((unsigned char)str1_p[i]) - tolower((unsigned char)str2_p[i]);
        if (d != 0) {
            NH_END(NH_FALSE)
        }
    }
   
NH_END(NH_TRUE)
}

NH_BOOL Nh_isSurrogate(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint >= 0xD800 && codepoint <= 0xDFFF)
}

NH_BOOL Nh_isScalarValue(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(!Nh_isSurrogate(codepoint))
}

NH_BOOL Nh_isNonCharacter(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END((codepoint >= 0xFDD0 && codepoint <= 0xFDEF) || codepoint == 0xFFFE || codepoint == 0xFFFF || codepoint == 0x1FFFE || codepoint == 0x1FFFF || codepoint == 0x2FFFE || codepoint == 0x2FFFF || codepoint == 0x3FFFE || codepoint == 0x3FFFF || codepoint == 0x4FFFE || codepoint == 0x4FFFF || codepoint == 0x5FFFE || codepoint == 0x5FFFF || codepoint == 0x6FFFE || codepoint == 0x6FFFF || codepoint == 0x7FFFE || codepoint == 0x7FFFF || codepoint == 0x8FFFE || codepoint == 0x8FFFF || codepoint == 0x9FFFE || codepoint == 0x9FFFF || codepoint == 0xAFFFE || codepoint == 0xAFFFF || codepoint == 0xBFFFE || codepoint == 0xBFFFF || codepoint == 0xCFFFE || codepoint == 0xCFFFF || codepoint == 0xDFFFE || codepoint == 0xDFFFF || codepoint == 0xEFFFE || codepoint == 0xEFFFF || codepoint == 0xFFFFE || codepoint == 0xFFFFF || codepoint == 0x10FFFE || codepoint == 0x10FFFF)
}

NH_BOOL Nh_isC0Control(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(codepoint >= 0x0000 && codepoint <= 0x001F)
}

NH_BOOL Nh_isControl(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()
NH_END(Nh_isC0Control(codepoint) || (codepoint >= 0x007F && codepoint <= 0x009F))
}

// LOOKUP ==========================================================================================

static int Nh_getUnicodeLookupIndex(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()

    for (int i = 0; i < sizeof(NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP) / sizeof(NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[i][0] && codepoint < NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[i][1]) {
            NH_END(-1)
        }
    }

    for (int i = 0; i < sizeof(NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP) / sizeof(NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[0]); ++i) {
        if (codepoint > NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[i][0]) {
            codepoint -= NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[i][1] - NH_UNICODE_LOOKUP_UNDEFINED_RANGES_PP[i][0] - 1;
        }
        else {break;}
    }

NH_END(codepoint)
}

const NH_BYTE *Nh_getUnicodeCodepointDescription(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()

    codepoint = Nh_getUnicodeLookupIndex(codepoint);
    if (codepoint == -1) {NH_END(NULL)}

NH_END(NH_UNICODE_LOOKUP_PP[codepoint])
}

static NH_BOOL Nh_unicodeCodepointInCategory(
    const NH_BYTE *codepointDescription_p, const NH_BYTE *categoryAbbreviation_p)
{
NH_BEGIN()

    NH_BYTE copy_p[512] = {'\0'};
    strcpy(copy_p, codepointDescription_p);

    NH_BYTE *p = copy_p;
    for (int i = 0; i < strlen(copy_p); ++i) {
        if (copy_p[i] == ';') {
            copy_p[i] = '\0';
            if (!strcmp(p, categoryAbbreviation_p)) {
                NH_END(NH_TRUE)
            }
            copy_p[i] = ';';
            p = &copy_p[i + 1];
        }
    }

NH_END(NH_FALSE)
}

// relevant: https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
NH_BOOL Nh_unicodeCodepointID_START(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()

    const NH_BYTE *descr_p = Nh_getUnicodeCodepointDescription(codepoint);
    if (descr_p == NULL) {NH_END(NH_FALSE)}

    if (Nh_unicodeCodepointInCategory(descr_p, "L")
    ||  Nh_unicodeCodepointInCategory(descr_p, "Nl")) {
        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

// relevant: https://www.unicode.org/reports/tr41/tr41-26.html#UAX31
NH_BOOL Nh_unicodeCodepointID_CONTINUE(
    NH_UNICODE_CODEPOINT codepoint)
{
NH_BEGIN()

    if (Nh_unicodeCodepointID_START(codepoint)) {
        NH_END(NH_TRUE)
    }

    const NH_BYTE *descr_p = Nh_getUnicodeCodepointDescription(codepoint);
    if (descr_p == NULL) {NH_END(NH_FALSE)}

    if (Nh_unicodeCodepointInCategory(descr_p, "Mn")
    ||  Nh_unicodeCodepointInCategory(descr_p, "Mc")
    ||  Nh_unicodeCodepointInCategory(descr_p, "Nd")
    ||  Nh_unicodeCodepointInCategory(descr_p, "Pc")) {
        NH_END(NH_TRUE)
    }

NH_END(NH_FALSE)
}

// LOG =============================================================================================

NH_RESULT Nh_logUnicodeCodepoints()
{
NH_BEGIN()

    for (int i = 0; i < NH_UNICODE_LOOKUP_COUNT; ++i) {
        Nh_logUnicodeCodepoint(NH_UNICODE_LOOKUP_PP[i]);
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

