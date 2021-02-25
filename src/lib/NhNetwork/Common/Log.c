// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"

#include "../../NhCore/Config.h"
#include "../../NhCore/String.h"
#include "../../NhCore/Memory.h"
#include "../../NhCore/Logger.h"

#include "MACRO/FLOW.h"

#include <string.h>
#include <stdio.h>

// LOG =============================================================================================

void Nh_Net_logReceive(
    char *data_p)
{
NH_NET_BEGIN()

    if (NH_CONFIG.Flags.Log.receive == NH_TRUE)
    {
        Nh_String String = Nh_initString(128); 

        if (Nh_appendFormatToString(&String, "%s\n", data_p) != NH_SUCCESS) {NH_NET_SILENT_END()}

        Nh_sendLogMessage(String.bytes_p);
        Nh_freeString(&String);
    }

NH_NET_SILENT_END()
}

void Nh_Net_logSend(
    char *data_p)
{
NH_NET_BEGIN()

    if (NH_CONFIG.Flags.Log.send == NH_TRUE)
    {
        Nh_String String = Nh_initString(128); 

        if (Nh_appendFormatToString(&String, "%s\n", data_p) != NH_SUCCESS) {NH_NET_SILENT_END()}

        Nh_sendLogMessage(String.bytes_p);
        Nh_freeString(&String);
    }

NH_NET_SILENT_END()
}

NH_NET_RESULT _Nh_Net_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.net) {return NH_NET_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_NET_RESULT _Nh_Net_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.net) {return NH_NET_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_NET_RESULT _Nh_Net_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_NET_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.net) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, NH_NET_RESULTS_PP[result], line, result == NH_NET_SUCCESS);
    return result;
}

