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

// UTF8 =============================================================================================

#define BETWEEN(x, a, b) ((a) <= (x) && (x) <= (b))
#define LEN(a)           (sizeof(a) / sizeof(a)[0])

static NH_UNSIGNED_BYTE utfbyte[NH_UTF8_SIZE + 1] = {0x80,    0, 0xC0, 0xE0, 0xF0};
static NH_UNSIGNED_BYTE utfmask[NH_UTF8_SIZE + 1] = {0xC0, 0x80, 0xE0, 0xF0, 0xF8};
static NH_UNICODE_CODEPOINT utfmin[NH_UTF8_SIZE + 1] = {       0,    0,  0x80,  0x800,  0x10000};
static NH_UNICODE_CODEPOINT utfmax[NH_UTF8_SIZE + 1] = {0x10FFFF, 0x7F, 0x7FF, 0xFFFF, 0x10FFFF};

size_t Nh_validateUTF8(
    NH_UNICODE_CODEPOINT *u, size_t i)
{
    if (!BETWEEN(*u, utfmin[i], utfmax[i]) || BETWEEN(*u, 0xD800, 0xDFFF))
        *u = NH_UTF8_INVALID;
    for (i = 1; *u > utfmax[i]; ++i)
        ;
    
    return i;
}

static NH_UNICODE_CODEPOINT Nh_decodeUTF8Byte(
    NH_BYTE c, size_t *i)
{
    for (*i = 0; *i < LEN(utfmask); ++(*i))
        if (((NH_UNSIGNED_BYTE)c & utfmask[*i]) == utfbyte[*i])
            return (NH_UNSIGNED_BYTE)c & ~utfmask[*i];

    return 0;
}

size_t Nh_decodeUTF8(
    const NH_BYTE *c, NH_UNICODE_CODEPOINT *u, size_t clen)
{
    size_t i, j, len, type;
    NH_UNICODE_CODEPOINT udecoded;
    
    *u = NH_UTF8_INVALID;
    if (!clen)
        return 0;
    udecoded = Nh_decodeUTF8Byte(c[0], &len);
    if (!BETWEEN(len, 1, NH_UTF8_SIZE))
        return 1;
    for (i = 1, j = 1; i < clen && j < len; ++i, ++j) {
        udecoded = (udecoded << 6) | Nh_decodeUTF8Byte(c[i], &type);
        if (type != 0)
            return j;
    }
    if (j < len)
        return 0;
    *u = udecoded;
    Nh_validateUTF8(u, len);
    
    return len;
}

static NH_BYTE Nh_encodeToUTF8Byte(
    NH_UNICODE_CODEPOINT u, size_t i)
{
    return utfbyte[i] | (u & ~utfmask[i]);
}

size_t Nh_encodeToUTF8(
    NH_UNICODE_CODEPOINT u, NH_BYTE *c)
{
    size_t len, i;
    
    len = Nh_validateUTF8(&u, 0);
    if (len > NH_UTF8_SIZE)
        return 0;
    
    for (i = len - 1; i != 0; --i) {
        c[i] = Nh_encodeToUTF8Byte(u, 0);
        u >>= 6;
    }
    c[0] = Nh_encodeToUTF8Byte(u, len);
    
    return len;
}

Nh_UnicodeString Nh_decodeUTF8Text(
    NH_BYTE *bytes_p)
{
NH_BEGIN()

    Nh_UnicodeString UnicodeCodepoints = Nh_initUnicodeString(128);

    int i = 0;
    while (i < strlen(bytes_p)) {
        NH_UNICODE_CODEPOINT *codepoint_p = Nh_incrementUnicodeString(&UnicodeCodepoints);
        i += Nh_decodeUTF8(&bytes_p[i], codepoint_p, sizeof(NH_UNICODE_CODEPOINT));
    }

NH_END(UnicodeCodepoints)
}

Nh_UTF8String Nh_encodeTextToUTF8(
    NH_UNICODE_CODEPOINT *codepoints_p, size_t length)
{
NH_BEGIN()

    Nh_UTF8String String = Nh_initString(64);

    for (int i = 0; i < length; ++i) {
        NH_BYTE bytes_p[4] = {'\0'};
        memset(bytes_p, 0, 4);
        size_t size = Nh_encodeToUTF8(codepoints_p[i], bytes_p);
        Nh_appendToArray(&String, bytes_p, size);
    }

NH_END(String)
}

// UNICODE STRING ==================================================================================

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

// ASCII ===========================================================================================

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
NH_END(Nh_isASCIIUpperAlpha(codepoint) && Nh_isASCIILowerAlpha(codepoint))
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

     for (;; str1_p++, str2_p++) {
        int d = tolower((unsigned char)*str1_p) - tolower((unsigned char)*str2_p);
        if (d != 0 || !*str1_p) {
            NH_END(NH_FALSE)
        }
    }
   
NH_END(NH_TRUE)
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

