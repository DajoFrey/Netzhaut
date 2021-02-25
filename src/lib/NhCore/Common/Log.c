// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"

#include "MACRO/FLOW.h"

#include "../Logger.h"
#include "../Config.h"

#include <stdio.h>

// LOG =============================================================================================

void Nh_logNewFont( // TODO multithreading
    NH_BYTE *absolutePath_p)
{
NH_BEGIN()

    NH_BYTE message_p[512] = {'\0'};
    sprintf(message_p, "[NhCore:Fonts]{%s}", absolutePath_p);
    Nh_sendLogMessage(message_p);

NH_SILENT_END()
}

void Nh_logUnicodeCodepoint( // TODO multithreading
    const NH_BYTE *codepoint_p)
{
NH_BEGIN()

    NH_BYTE message_p[512] = {'\0'};
    sprintf(message_p, "[NhCore:Unicode]{%s}", codepoint_p);
    Nh_sendLogMessage(message_p);

NH_SILENT_END()
}

void Nh_logSystemInfo( // TODO multithreading
    NH_BYTE *info_p, int line)
{
NH_BEGIN()

    NH_BYTE message_p[512] = {'\0'};
    sprintf(message_p, "[NhCore:System](REPLACE%d){%s}", line, info_p);
    Nh_sendLogMessage(message_p);

NH_SILENT_END()
}

// FLOW LOGGING ====================================================================================

NH_RESULT _Nh_logBegin(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_RESULT _Nh_logEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.core) {return NH_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_RESULT _Nh_logDiagnosticEnd(
    const NH_BYTE *file_p, const NH_BYTE *function_p, NH_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.core) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, NH_RESULTS_PP[result], line, result == NH_SUCCESS);
    return result;
}

