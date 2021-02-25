// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "MACRO/FLOW.h"
#include <stdio.h>

// DEFINE ==========================================================================================

#define NH_LOADER_LOG_PREFIX "NhLoader"

// LOG =============================================================================================

NH_LOADER_RESULT _Nh_Loader_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_LOADER_RESULT _Nh_Loader_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_LOADER_RESULT _Nh_Loader_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_LOADER_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

