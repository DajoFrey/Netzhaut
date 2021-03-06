// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"

#include "../Common/Macro.h"
#include "../../NhCore/Unicode.h"

#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    Parser.Token_p = NULL;
    Parser.OpenElements = Nh_initStack();
//    Parser.Document_p = Document_p == NULL ? Nh_Web_createCompositeObject("DOM", "Document") : Document_p;

NH_HTML_END(Parser)
}

NH_HTML_RESULT Nh_HTML_parseDocument(
    Nh_UnicodeString Codepoints, Nh_Web_CompositeObject *Document_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser = Nh_initParser(Document_p);
    Nh_HTML_Tokenizer Tokenizer = Nh_initTokenizer();
    Tokenizer.Parser_p = &Parser;

    while (Tokenizer.index < Codepoints.length) {
        NH_HTML_CHECK(Nh_HTML_consumeNext(Tokenizer_p, Codepoints))
    }

NH_HTML_END(NH_HTML_SUCCESS)
}

