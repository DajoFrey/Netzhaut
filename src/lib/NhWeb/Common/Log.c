// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "Macro.h"

#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include "../Runtime/Definitions.h"
#include "../../NhCore/Logger.h"

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEB_RESULT _Nh_Web_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_WEB_RESULT _Nh_Web_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_WEB_RESULT _Nh_Web_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_WEB_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

NH_WEB_RESULT Nh_Web_logTokens(
    NH_BYTE *fragmentName_p, Nh_Array *Tokens_p, NH_BOOL dirty)
{
NH_WEB_BEGIN()

    for (int i = 0; i < Tokens_p->length; ++i) 
    {
        Nh_Web_Token *Token_p = &((Nh_Web_Token*)Tokens_p->bytes_p)[i];
        NH_BYTE message_p[1024] = {'\0'};

        Nh_String String = Nh_initString(64);
        Nh_appendToString(&String, Token_p->String.bytes_p, Token_p->String.length);

        for (int i = 0; i < String.length; ++i) {
            if (String.bytes_p[i] == '\n' || String.bytes_p[i] == '\r') {String.bytes_p[i] = ' ';}
        }

        sprintf(
            message_p, dirty ? "[NhWeb:Parser:%s:TokensDirty]{%s [%s]}" : "[NhWeb:Parser:%s:TokensClean]{%s [%s]}",
            fragmentName_p, NH_WEB_TOKENS_PP[Token_p->type], String.bytes_p 
        );

        Nh_sendLogMessage(message_p);

        Nh_freeString(&String);
    }

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

#define MAX_DEPTH 1024

static NH_WEB_RESULT Nh_Web_logParseTreeRecursively(
    NH_BYTE *fragmentName_p, Nh_Web_ParseNode *ParseNode_p, Nh_Web_ParseNode *Parent_p, int depth, NH_BOOL *branch_p)
{
NH_WEB_BEGIN()

    if (depth >= MAX_DEPTH) {NH_WEB_END(NH_WEB_ERROR_BAD_STATE)}

    NH_BYTE indent_p[MAX_DEPTH] = {'\0'};
    NH_BYTE message_p[2048] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == NH_TRUE ? '|' : ' ';
    }

    if (ParseNode_p->Token_p == NULL) {
        sprintf(
            message_p, "[NhWeb:Parser:%s:ParseTree]{%s%s}", fragmentName_p, indent_p, 
            NH_WEB_PARSE_NODE_NAMES_PP[ParseNode_p->type]
        );
    }
    else {
        sprintf(
            message_p, "[NhWeb:Parser:%s:ParseTree]{%sTERMINAL %s}", fragmentName_p, indent_p, 
            ParseNode_p->Token_p->String.bytes_p 
        );
    }

    Nh_sendLogMessage(message_p);

    branch_p[depth] = NH_TRUE;
    if (Parent_p != NULL && ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = NH_FALSE;
    }

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        Nh_Web_logParseTreeRecursively(fragmentName_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p);
    }

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

NH_WEB_RESULT Nh_Web_logParseTree(
    NH_BYTE *fragmentName_p, Nh_Web_ParseNode *ParseNode_p)
{
NH_WEB_BEGIN()

    NH_BOOL branch_p[MAX_DEPTH];
    memset(branch_p, NH_FALSE, MAX_DEPTH);
    Nh_Web_logParseTreeRecursively(fragmentName_p, ParseNode_p, NULL, 0, branch_p);

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

NH_WEB_RESULT Nh_Web_logFragment(
    NH_BYTE *specification_p, Nh_Web_Fragment *Fragment_p)
{
NH_WEB_BEGIN()

    for (int i = 0; i < Fragment_p->Classes.length; ++i) 
    {
        Nh_Web_Class *Class_p = &((Nh_Web_Class*)Fragment_p->Classes.bytes_p)[i];
        NH_BYTE className_p[255] = {'\0'};
        sprintf(className_p, Class_p->partial ? "%s (partial)" : "%s", Class_p->name_p);

        for (int j = 0; j < Class_p->Members.length; ++j) 
        {
            Nh_Web_ClassMember *ClassMember_p = &((Nh_Web_ClassMember*)Class_p->Members.bytes_p)[j];
            NH_BYTE message_p[1024] = {'\0'};
            sprintf(
                message_p, "[NhWeb:%s:Classes:%s]{%s}", specification_p, className_p, NH_WEB_PARSE_NODE_NAMES_PP[ClassMember_p->Node_p->type]
            );
            Nh_sendLogMessage(message_p);
        }
    }

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

