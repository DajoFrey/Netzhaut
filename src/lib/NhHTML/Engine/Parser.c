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

Nh_HTML_Parser Nh_HTML_initParser()
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser;
    Parser.insertionMode = NH_HTML_INSERTION_MODE_INITIAL;
    Parser.originalInsertionMode = Parser.insertionMode;
    Parser.scriptingEnabled = NH_TRUE;
    Parser.framesetOk = NH_TRUE;

NH_HTML_END(Parser)
}

NH_HTML_RESULT Nh_HTML_parseDocument(
    Nh_UnicodeString Codepoints)
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser = Nh_initParser();
    Nh_HTML_Tokenizer Tokenizer = Nh_initTokenizer();
    Tokenizer.Parser_p = &Parser;

    unsigned int index = 0;
    while (Nh_HTML_tokenize(Tokenizer_p, Codepoints, index, )

NH_HTML_END(NH_HTML_SUCCESS)
}
