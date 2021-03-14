// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "UTF8.h"

#include "../Common/Macro.h"
#include NH_ENCODING_FLOW
#include NH_ENCODING_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef struct Nh_Encoding_UTF8Decoder {
    NH_UNICODE_CODEPOINT codepoint;
    NH_UNSIGNED_BYTE bytesSeen;
    NH_UNSIGNED_BYTE bytesNeeded;
    NH_UNSIGNED_BYTE lowerBoundary;
    NH_UNSIGNED_BYTE upperBoundary;
} Nh_Encoding_UTF8Decoder;

// DECODER =========================================================================================

static Nh_Encoding_UTF8Decoder Nh_Encoding_initUTF8Decoder()
{
NH_ENCODING_BEGIN()

    Nh_Encoding_UTF8Decoder Decoder;
    
    Decoder.codepoint     = 0;
    Decoder.bytesSeen     = 0;
    Decoder.bytesNeeded   = 0;
    Decoder.lowerBoundary = 0x80;
    Decoder.upperBoundary = 0xBF;

NH_ENCODING_END(Decoder)
}

static int Nh_Encoding_decodeUTF8Codepoint(
    Nh_Encoding_UTF8Decoder *Decoder_p, NH_UNSIGNED_BYTE byte) 
{
NH_ENCODING_BEGIN()

    if (Decoder_p->bytesNeeded == 0) 
    {
        if (byte <= 0x7F) {
            Decoder_p->codepoint = byte;
            NH_ENCODING_END(1)
        }
        else if (byte >= 0xC2 && byte <= 0xDF) {
            Decoder_p->bytesNeeded = 1;
            Decoder_p->codepoint   = byte & 0x1F;
        }
        else if (byte >= 0xE0 && byte <= 0xEF) {
            if (byte == 0xE0) {Decoder_p->lowerBoundary = 0xA0;}
            if (byte == 0xED) {Decoder_p->upperBoundary = 0x9F;}
            Decoder_p->bytesNeeded = 2;
            Decoder_p->codepoint   = byte & 0xF;
        }
        else if (byte >= 0xF0 && byte <= 0xF4) {
            if (byte == 0xF0) {Decoder_p->lowerBoundary = 0x90;}
            if (byte == 0xF4) {Decoder_p->upperBoundary = 0x8F;}
            Decoder_p->bytesNeeded = 3;
            Decoder_p->codepoint   = byte & 0x7;
        }
        else {
            NH_ENCODING_END(2)
        }

        NH_ENCODING_END(0)
    }

    if (byte < Decoder_p->lowerBoundary || byte > Decoder_p->upperBoundary) 
    {
        *Decoder_p = Nh_Encoding_initUTF8Decoder();
        NH_ENCODING_END(2)
    }

    Decoder_p->lowerBoundary = 0x80;
    Decoder_p->upperBoundary = 0xBF;
    Decoder_p->codepoint = (Decoder_p->codepoint << 6) | (byte & 0x3F);
    Decoder_p->bytesSeen++;

    if (Decoder_p->bytesSeen != Decoder_p->bytesNeeded) {
        NH_ENCODING_END(0)
    }
    
NH_ENCODING_END(1)
}

Nh_UnicodeString Nh_Encoding_decodeUTF8(
    NH_UNSIGNED_BYTE *bytes_p, unsigned long length)
{
NH_ENCODING_BEGIN()

    Nh_UnicodeString Result = Nh_initUnicodeString(64);
    Nh_Encoding_UTF8Decoder Decoder = Nh_Encoding_initUTF8Decoder();

    for (unsigned long i = 0; i < length; ++i) 
    {
        int result = Nh_Encoding_decodeUTF8Codepoint(&Decoder, bytes_p[i]);
        if (result)
        {
            if (result == 2) {break;} // error

            Nh_appendToUnicodeString(&Result, &Decoder.codepoint, 1);
            Decoder_p->codepoint   = 0;
            Decoder_p->bytesNeeded = 0;
            Decoder_p->bytesSeen   = 0;
        }
    }

NH_ENCODING_END(Result)
}

NH_UNICODE_CODEPOINT Nh_Encoding_decodeUTF8Single(
    NH_UNSIGNED_BYTE *bytes_p, unsigned long length)
{
NH_ENCODING_BEGIN()

    Nh_UnicodeString String = Nh_Encoding_decodeUTF8(bytes_p, length);
    NH_UNICODE_CODEPOINT codepoint = String.length == 1 ? String.p[0] : 0;
    Nh_freeUnicodeString(&String);

NH_ENCODING_END(codepoint)
}

