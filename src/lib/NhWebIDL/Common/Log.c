// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "Macro.h"

#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include "../Runtime/Definitions.h"
#include "../../NhCore/Logger.h"

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEBIDL_RESULT _Nh_WebIDL_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_WEBIDL_RESULT _Nh_WebIDL_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_WEBIDL_RESULT _Nh_WebIDL_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_WEBIDL_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

NH_WEBIDL_RESULT Nh_WebIDL_logTokens(
    NH_BYTE *fragmentName_p, Nh_Array *Tokens_p, NH_BOOL dirty)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < Tokens_p->length; ++i) 
    {
        Nh_WebIDL_Token *Token_p = &((Nh_WebIDL_Token*)Tokens_p->bytes_p)[i];
        NH_BYTE message_p[1024] = {'\0'};

        Nh_String String = Nh_initString(64);
        Nh_appendToString(&String, Token_p->String.bytes_p, Token_p->String.length);

        for (int i = 0; i < String.length; ++i) {
            if (String.bytes_p[i] == '\n' || String.bytes_p[i] == '\r') {String.bytes_p[i] = ' ';}
        }

        sprintf(
            message_p, dirty ? "[NhWebIDL:Parser:%s:TokensDirty]{%s [%s]}" : "[NhWebIDL:Parser:%s:TokensClean]{%s [%s]}",
            fragmentName_p, NH_WEBIDL_TOKENS_PP[Token_p->type], String.bytes_p 
        );

        Nh_sendLogMessage(message_p);

        Nh_freeString(&String);
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

#define MAX_DEPTH 1024

static NH_WEBIDL_RESULT Nh_WebIDL_logParseTreeRecursively(
    NH_BYTE *fragmentName_p, Nh_WebIDL_ParseNode *ParseNode_p, Nh_WebIDL_ParseNode *Parent_p, int depth, NH_BOOL *branch_p)
{
NH_WEBIDL_BEGIN()

    if (depth >= MAX_DEPTH) {NH_WEBIDL_END(NH_WEBIDL_ERROR_BAD_STATE)}

    NH_BYTE indent_p[MAX_DEPTH] = {'\0'};
    NH_BYTE message_p[2048] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == NH_TRUE ? '|' : ' ';
    }

    if (ParseNode_p->Token_p == NULL) {
        sprintf(
            message_p, "[NhWebIDL:Parser:%s:ParseTree]{%s%s}", fragmentName_p, indent_p, 
            NH_WEBIDL_PARSE_NODE_NAMES_PP[ParseNode_p->type]
        );
    }
    else {
        sprintf(
            message_p, "[NhWebIDL:Parser:%s:ParseTree]{%sTERMINAL %s}", fragmentName_p, indent_p, 
            ParseNode_p->Token_p->String.bytes_p 
        );
    }

    Nh_sendLogMessage(message_p);

    branch_p[depth] = NH_TRUE;
    if (Parent_p != NULL && ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = NH_FALSE;
    }

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        Nh_WebIDL_logParseTreeRecursively(fragmentName_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p);
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

NH_WEBIDL_RESULT Nh_WebIDL_logParseTree(
    NH_BYTE *fragmentName_p, Nh_WebIDL_ParseNode *ParseNode_p)
{
NH_WEBIDL_BEGIN()

    NH_BOOL branch_p[MAX_DEPTH];
    memset(branch_p, NH_FALSE, MAX_DEPTH);
    Nh_WebIDL_logParseTreeRecursively(fragmentName_p, ParseNode_p, NULL, 0, branch_p);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

NH_WEBIDL_RESULT Nh_WebIDL_logFragment(
    NH_BYTE *specification_p, Nh_WebIDL_Fragment *Fragment_p)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < Fragment_p->Interfaces.length; ++i) 
    {
        Nh_WebIDL_Interface *Interface_p = &((Nh_WebIDL_Interface*)Fragment_p->Interfaces.bytes_p)[i];
        NH_BYTE className_p[255] = {'\0'};
        sprintf(className_p, Interface_p->partial ? "%s (partial)" : "%s", Interface_p->name_p);

        if (Interface_p->Inheritance_p) 
        {
            NH_BYTE message_p[1024] = {'\0'}; 
            sprintf(
                message_p, "[NhWebIDL:%s:Interfaces:%s]{Inherits %s (%s)}", specification_p, className_p, Interface_p->Inheritance_p->interface_p, Interface_p->Inheritance_p->specification_p ? Interface_p->Inheritance_p->specification_p : Interface_p->Specification_p->name_p
            );
            Nh_sendLogMessage(message_p);
        }

        NH_BYTE message_p[1024] = {'\0'};
        sprintf(
            message_p, "[NhWebIDL:%s:Interfaces:%s]{Members:}", specification_p, className_p
        );
        Nh_sendLogMessage(message_p);

        for (int j = 0; j < Interface_p->Members.length; ++j) 
        {
            Nh_WebIDL_InterfaceMember *InterfaceMember_p = &((Nh_WebIDL_InterfaceMember*)Interface_p->Members.bytes_p)[j];
            NH_BYTE message_p[1024] = {'\0'};
            sprintf(
                message_p, "[NhWebIDL:%s:Interfaces:%s]{  %s %s}", specification_p, className_p, NH_WEBIDL_PARSE_NODE_NAMES_PP[InterfaceMember_p->Node_p->type], InterfaceMember_p->name_p == NULL ? "null" : InterfaceMember_p->name_p
            );
            Nh_sendLogMessage(message_p);
        }
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

