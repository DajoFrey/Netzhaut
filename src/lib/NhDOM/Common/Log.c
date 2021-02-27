// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "MACRO/FLOW.h"

#include "../../NhCore/Unicode.h"
#include "../../NhCore/Config.h"
#include "../../NhCore/Logger.h"

#include <stdio.h>

// LOG =============================================================================================

NH_DOM_RESULT _Nh_DOM_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_begin(file_p, function_p);
}

NH_DOM_RESULT _Nh_DOM_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _Nh_end(file_p, function_p);
}

NH_DOM_RESULT _Nh_DOM_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_DOM_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _Nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

