// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "MACRO/FLOW.h"

#include "../Engine/Lexer.h"

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Config.h"
#include "../../NhCore/Logger.h"

#include <stdio.h>

// DEFINE ==========================================================================================

#define NH_ECMASCRIPT_LOG_PREFIX "NhECMAScript"
#define NH_ECMASCRIPT_LOG_CATEGORY_LOGGING "Logging"
#define NH_ECMASCRIPT_LOG_CATEGORY_DECODER "Decoder"

// LOG =============================================================================================

NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_ECMASCRIPT_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

//NH_ECMASCRIPT_RESULT Nh_ECMAScript_finishLogging(
//    Nh_ECMAScript_Script *Script_p, Nh_Array *UnicodeCodePoints_p)
//{
//NH_ECMASCRIPT_BEGIN()
//
//    NH_BYTE message_p[127] = {'\0'};
//    sprintf(message_p, "[%s:%d:%s](FINISH)", NH_ECMASCRIPT_LOG_PREFIX, NH_ECMASCRIPT_LOG_CATEGORY_LOGGING);
//    Nh_sendLogMessage(message_p);
//
//NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
//}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_logDecoder(
    void *handle_p, Nh_UnicodeString *Codepoints_p)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < Codepoints_p->length; ++i) 
    {
        NH_BYTE message_p[512] = {'\0'};
        sprintf(
            message_p, "[NhECMAScript:%p:Decoder]{%s}", handle_p,
            Nh_getUnicodeCodepointDescription(Codepoints_p->p[i])
        );
        Nh_sendLogMessage(message_p);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_logLexer(
    void *handle_p, Nh_Array *InputElements_p, NH_BOOL dirty)
{
NH_ECMASCRIPT_BEGIN()

    for (int i = 0; i < InputElements_p->length; ++i) 
    {
        Nh_ECMAScript_InputElement *InputElement_p = &((Nh_ECMAScript_InputElement*)InputElements_p->bytes_p)[i];
        NH_BYTE message_p[512] = {'\0'};

        Nh_String String = Nh_initString(64);
        Nh_appendToString(&String, InputElement_p->String.bytes_p, InputElement_p->String.length);

        for (int i = 0; i < String.length; ++i) {
            if (String.bytes_p[i] == '\n' || String.bytes_p[i] == '\r') {String.bytes_p[i] = ' ';}
        }

        sprintf(
            message_p, dirty ? "[NhECMAScript:%p:LexerDirty]{%s [%s]}" : "[NhECMAScript:%p:LexerClean]{%s [%s]}",
            handle_p, NH_ECMASCRIPT_INPUT_ELEMENTS_PP[InputElement_p->type], String.bytes_p 
        );

        Nh_sendLogMessage(message_p);
        Nh_freeString(&String);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

NH_ECMASCRIPT_RESULT Nh_ECMAScript_logParseTree(
    void *handle_p, Nh_ECMAScript_ParseNode *ParseNode_p, Nh_ECMAScript_ParseNode *Parent_p, int depth, NH_BOOL *branch_p)
{
NH_ECMASCRIPT_BEGIN()

    NH_BYTE message_p[1200] = {'\0'};
    NH_BYTE indent_p[1024] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == NH_TRUE ? '|' : ' ';
    }

    if (ParseNode_p->Value_p == NULL) {
        sprintf(
            message_p, "[NhECMAScript:%p:ParseTree]{%s%s}", handle_p, indent_p, 
            NH_ECMASCRIPT_PARSE_NODE_NAMES_PP[ParseNode_p->type]
        );
    }
    else {
        sprintf(
            message_p, "[NhECMAScript:%p:ParseTree]{%sTERMINAL %s}", handle_p, indent_p, 
            ParseNode_p->Value_p->String.bytes_p 
        );
    }

    Nh_sendLogMessage(message_p);

    branch_p[depth] = NH_TRUE;
    if (Parent_p != NULL &&  ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = NH_FALSE;
    }

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        Nh_ECMAScript_logParseTree(handle_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p);
    }

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

