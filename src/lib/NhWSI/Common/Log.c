// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Log.h"
#include "../Mouse.h"

#include "../../NhCore/Config.h"
#include "../../NhCore/Memory.h"
#include "../../NhCore/Logger.h"

#include "MACRO/FLOW.h"

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

void Nh_WSI_logMouse( // TODO multithreading
    Nh_WSI_MouseEvent *Event_p)
{
NH_WSI_BEGIN()

    if (NH_CONFIG.Flags.Log.mouse)
    {
        char *chars_p = Nh_WSI_dumpMouse(Event_p);
        if (chars_p != NULL) {Nh_sendLogMessage(chars_p);}
        Nh_free(chars_p);
    }

NH_WSI_SILENT_END()
}

void Nh_WSI_logWindow( // TODO multithreading
    Nh_WSI_ConfigureEvent *Event_p)
{
NH_WSI_BEGIN()

    if (NH_CONFIG.Flags.Log.window)
    {
        char *chars_p = Nh_WSI_dumpConfigureEvent(Event_p);
        if (chars_p != NULL) {Nh_sendLogMessage(chars_p);}
        Nh_free(chars_p);
    }

NH_WSI_SILENT_END()
}

NH_WSI_RESULT _Nh_WSI_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_WSI_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_WSI_RESULT _Nh_WSI_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_WSI_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_WSI_RESULT _Nh_WSI_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_WSI_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, NH_WSI_RESULTS_PP[result], line, result == NH_WSI_SUCCESS);
    return result;
}

