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

void Nh_IO_logMouse( // TODO multithreading
    Nh_IO_MouseEvent *Event_p)
{
NH_IO_BEGIN()

    if (NH_CONFIG.Flags.Log.mouse)
    {
        char *chars_p = Nh_IO_dumpMouse(Event_p);
        if (chars_p != NULL) {Nh_sendLogMessage(chars_p);}
        Nh_free(chars_p);
    }

NH_IO_SILENT_END()
}

void Nh_IO_logWindow( // TODO multithreading
    Nh_IO_ConfigureEvent *Event_p)
{
NH_IO_BEGIN()

    if (NH_CONFIG.Flags.Log.window)
    {
        char *chars_p = Nh_IO_dumpConfigureEvent(Event_p);
        if (chars_p != NULL) {Nh_sendLogMessage(chars_p);}
        Nh_free(chars_p);
    }

NH_IO_SILENT_END()
}

NH_IO_RESULT _Nh_IO_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_IO_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_IO_RESULT _Nh_IO_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return NH_IO_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_IO_RESULT _Nh_IO_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_IO_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.io) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, NH_IO_RESULTS_PP[result], line, result == NH_IO_SUCCESS);
    return result;
}

