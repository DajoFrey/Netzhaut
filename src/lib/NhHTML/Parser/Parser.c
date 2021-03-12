// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Parser.h"

#include "../Objects/Document.h"
#include "../Common/Log.h"
#include "../Common/Macro.h"
#include NH_HTML_DEFAULT_CHECK
#include NH_HTML_FLOW

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Array.h"
#include "../../NhWebIDL/Runtime/String.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// PARSE ERROR =====================================================================================

NH_HTML_RESULT Nh_HTML_newParseError(
    Nh_HTML_Parser *Parser_p, unsigned long index, NH_HTML_PARSE_ERROR type)
{
NH_HTML_BEGIN()

    Nh_HTML_ParseError *Error_p = Nh_incrementArray(&Parser_p->Errors);
    NH_HTML_CHECK_MEM(Error_p)

    Error_p->type  = type;
    Error_p->index = index;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// PRE-PROCESS =====================================================================================

static Nh_WebIDL_USVString Nh_HTML_normalizeNewlines(
    Nh_WebIDL_USVString Codepoints)
{
NH_HTML_BEGIN()

    Nh_WebIDL_USVString Normalized = Nh_WebIDL_initUSVString(Codepoints.length);
    NH_UNICODE_CODEPOINT lf = 0x0A;

    unsigned long index = 0;
    while (index < Codepoints.length) {
        if (index + 1 < Codepoints.length) {
            if (Codepoints.p[index] == 0x0D && Codepoints.p[index + 1] == 0x0A) {
                Nh_WebIDL_appendToUSVString(&Normalized, &lf, 1);
                index += 2;
                continue;
            }
        }
        Nh_WebIDL_appendToUSVString(&Normalized, &Codepoints.p[index], 1);
        index++;
    }

    index = 0;
    while (index < Normalized.length) {
        if (Normalized.p[index] == 0x0D) {Normalized.p[index] = 0x0A;}
        index++; 
    }

NH_HTML_END(Normalized)
}

static NH_HTML_RESULT Nh_HTML_checkInputStream(
    Nh_HTML_Parser *Parser_p, Nh_UnicodeString InputStream)
{
NH_HTML_BEGIN()

    for (unsigned long i = 0; i < InputStream.length; ++i) 
    {
        if (Nh_isSurrogate(InputStream.p[i])) {
            NH_HTML_CHECK(Nh_HTML_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_SURROGATE_IN_INPUT_STREAM)) 
        }
        else if (Nh_isNonCharacter(InputStream.p[i])) {
            NH_HTML_CHECK(Nh_HTML_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_NONCHARACTER_IN_INPUT_STREAM))
        }
        else if (Nh_isControl(InputStream.p[i]) && !Nh_isASCIIWhitespace(InputStream.p[i]) && InputStream.p[i] != 0x00) {
            NH_HTML_CHECK(Nh_HTML_newParseError(Parser_p, i, NH_HTML_PARSE_ERROR_CONTROL_CHARACTER_IN_INPUT_STREAM))
        }
    } 

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// PARSER ==========================================================================================

static Nh_HTML_Parser Nh_HTML_initParser(
    Nh_WebIDL_Object *Document_p)
{
NH_HTML_BEGIN()

    Nh_HTML_Parser Parser;

    Parser.insertionMode            = NH_HTML_INSERTION_MODE_INITIAL;
    Parser.originalInsertionMode    = Parser.insertionMode;
    Parser.framesetOk               = NH_TRUE;
    Parser.pause                    = NH_FALSE;
    Parser.stop                     = NH_FALSE;
    Parser.scripting                = NH_TRUE;
    Parser.fosterParenting          = NH_FALSE;
    Parser.scriptNestingLevel       = 0;
    Parser.OpenElements             = Nh_initStack(255);
    Parser.TemplateInsertionModes   = Nh_initStack(255);
    Parser.ActiveFormattingElements = Nh_initList(64);
    Parser.Document_p               = Document_p;
    Parser.HeadElement_p            = NULL;
    Parser.FormElement_p            = NULL;
    Parser.Errors                   = Nh_initArray(sizeof(Nh_HTML_ParseError), 16);
    Parser.Token_p                  = NULL;
    Parser.Tokenizer_p              = NULL;

    if (!Parser.Document_p) {
        Parser.Document_p = Nh_HTML_createDocument();
    }

NH_HTML_END(Parser)
}

Nh_WebIDL_Object *Nh_HTML_parseDocument(
    Nh_UnicodeString InputStream, Nh_WebIDL_Object *Document_p)
{
NH_HTML_BEGIN()

#include NH_HTML_CUSTOM_CHECK

    Nh_HTML_Parser Parser = Nh_HTML_initParser(Document_p);
    NH_HTML_CHECK_MEM(NULL, Parser.Document_p)

    NH_HTML_CHECK(NULL, Nh_HTML_checkInputStream(&Parser, InputStream))
    Nh_WebIDL_USVString NormalizedInputStream = Nh_HTML_normalizeNewlines(InputStream);

    Nh_HTML_Tokenizer Tokenizer = Nh_HTML_initTokenizer(&Parser, NormalizedInputStream);
    Parser.Tokenizer_p = &Tokenizer;

    while (!Parser.stop) {
        NH_HTML_CHECK(NULL, Nh_HTML_consumeNext(&Tokenizer))
    }

    Nh_HTML_logDocument("bla", Parser.Document_p);
    Nh_WebIDL_freeUSVString(&Tokenizer.InputStream);

#include NH_HTML_DEFAULT_CHECK

NH_HTML_END(Document_p)
}

