// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"

#include "../Common/Macro.h"
#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include "../../NhWeb/Runtime/String.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// NORMALIZE NEWLINES ==============================================================================

static Nh_Web_USVString Nh_HTML_normalizeNewlines(
    Nh_Web_USVString Codepoints)
{
NH_HTML_BEGIN()

    Nh_Web_USVString Normalized = Nh_Web_initUSVString(Codepoints.length);
    NH_UNICODE_CODEPOINT lf = 0x0A;

    unsigned long index = 0;
    while (index < Codepoints.length) {
        if (index + 1 < Codepoints.length) {
            if (Codepoints.p[index] == 0x0D && Codepoints.p[index + 1] == 0x0A) {
                Nh_Web_appendToUSVString(&Normalized, &lf, 1);
                index += 2;
                continue;
            }
        }
        Nh_Web_appendToUSVString(&Normalized, &Codepoints.p[index], 1);
        index++;
    }

    index = 0;
    while (index < Codepoints.length) {
        if (Codepoints.p[index] == 0x0D) {Codepoints.p[index] = 0x0A;}
        index++; 
    }

NH_HTML_END(Normalized)
}

// PARSER ==========================================================================================

static Nh_HTML_Parser Nh_HTML_initParser(
    Nh_Web_CompositeObject *Document_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser;
    Parser.insertionMode = NH_HTML_INSERTION_MODE_INITIAL;
    Parser.originalInsertionMode = Parser.insertionMode;
    Parser.scriptingEnabled = NH_TRUE;
    Parser.framesetOk = NH_TRUE;
    Parser.OpenElements = Nh_initStack(128);
//    Parser.Document_p = Document_p == NULL ? Nh_Web_createCompositeObject("DOM", "Document") : Document_p;

NH_HTML_END(Parser)
}

NH_HTML_RESULT Nh_HTML_parseDocument(
    Nh_UnicodeString Codepoints, Nh_Web_CompositeObject *Document_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser = Nh_HTML_initParser(Document_p);

    Nh_Web_USVString NormalizedCodepoints = Nh_HTML_normalizeNewlines(Codepoints);

//    Nh_HTML_Tokenizer Tokenizer = Nh_initTokenizer();
//    Tokenizer.Parser_p = &Parser;
//
//    while (Tokenizer.index < Codepoints.length) {
//        NH_HTML_CHECK(Nh_HTML_consumeNext(Tokenizer_p, Codepoints))
//    }

NH_HTML_END(NH_HTML_SUCCESS)
}

