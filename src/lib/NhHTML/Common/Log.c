// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"

#include "MACRO/DEFAULT_CHECK.h"
#include "MACRO/FLOW.h"

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Config.h"
#include "../../NhCore/Logger.h"

#include "../../NhDOM/NodeList.h"
#include "../../NhWebIDL/Runtime/Object.h"

#include <stdio.h>
#include <string.h>

// LOG =============================================================================================

NH_HTML_RESULT _Nh_HTML_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_HTML_RESULT _Nh_HTML_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_HTML_RESULT _Nh_HTML_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_HTML_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

#define MAX_DEPTH 1024

static NH_HTML_RESULT Nh_HTML_logDocumentRecursively(
    NH_BYTE *logName_p, Nh_WebIDL_Object *Object_p, NH_BOOL last, int depth, NH_BOOL *branch_p)
{
NH_HTML_BEGIN()

    if (depth >= MAX_DEPTH) {NH_HTML_END(NH_HTML_ERROR_BAD_STATE)}

    NH_BYTE indent_p[MAX_DEPTH] = {'\0'};
    NH_BYTE message_p[2048] = {'\0'};

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == NH_TRUE ? '|' : ' ';
    }

    sprintf(
        message_p, "[NhHTML:Parser:%s:DOMTree]{%s%s}", logName_p, indent_p, Object_p->Interface_p->name_p 
    );
    Nh_sendLogMessage(message_p);

    branch_p[depth] = NH_TRUE;
    if (last) {branch_p[depth - 1] = NH_FALSE;}

    Nh_WebIDL_Object *NodeList_p = Nh_WebIDL_get(Object_p, "childNodes");
    NH_HTML_CHECK_NULL(NodeList_p)
    NH_WEBIDL_UNSIGNED_LONG length = Nh_DOM_getNodeListLength(NodeList_p);

    for (NH_WEBIDL_UNSIGNED_LONG i = 0; i < length; ++i) {
        Nh_HTML_logDocumentRecursively(logName_p, Nh_DOM_getFromNodeList(NodeList_p, i), i == length - 1, depth + 1, branch_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT Nh_HTML_logDocument(
    NH_BYTE *logName_p, Nh_WebIDL_Object *Document_p)
{
NH_HTML_BEGIN()

    NH_BOOL branch_p[MAX_DEPTH];
    memset(branch_p, NH_FALSE, MAX_DEPTH);
    Nh_HTML_logDocumentRecursively(logName_p, Document_p, NH_FALSE, 0, branch_p);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

