// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"

// DEBUG ===========================================================================================

NH_WEBIDL_RESULT _Nh_WebIDLSerializer_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_WEBIDL_RESULT _Nh_WebIDLSerializer_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_WEBIDL_RESULT _Nh_WebIDLSerializer_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_WEBIDL_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

