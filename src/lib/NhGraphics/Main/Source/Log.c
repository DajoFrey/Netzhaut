// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Macros.h"

#include "../../../Netzhaut/Output.h"
#include "../../../Netzhaut/Config.h"
#include "../../../Netzhaut/Log.h"
#include "../../../Netzhaut/Flow.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

void Nh_Gfx_logVulkan( // TODO multithreading
    char *message_p)
{
NH_BEGIN()

    if (NH_CONFIG.Flags.Log.vulkan) {Nh_addOutput(NH_OUTPUT_CHARS, message_p, NH_FALSE);}

NH_SILENT_END()
}

NH_RESULT Nh_Gfx_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.gfx) {return NH_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_RESULT Nh_Gfx_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.gfx) {return NH_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_RESULT Nh_Gfx_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.gfx) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, result, line);
    return result;
}

