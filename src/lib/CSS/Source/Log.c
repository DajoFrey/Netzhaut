// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Macros.h"

#include "../../Netzhaut/String.h"
#include "../../Netzhaut/Memory.h"
#include "../../Netzhaut/Config.h"
#include "../../Netzhaut/Output.h"
#include "../../Netzhaut/Flow.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

void Nh_CSS_logSheet( // TODO multithreading
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    if (NH_CONFIG.Flags.Log.sheet)
    {
        Nh_String *String_p = Nh_allocateString(NULL);

        char *sheet_p = Nh_CSS_stringifySheet(Sheet_p);
        if (sheet_p == NULL) {NH_SILENT_END()}

        if (Nh_appendFormatToString(String_p, "%s\n", sheet_p) != NH_SUCCESS) {NH_SILENT_END()}

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);
        Nh_free(sheet_p);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);    
        }
    }

NH_SILENT_END()
}

NH_RESULT Nh_CSS_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.css) {return NH_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_RESULT Nh_CSS_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.css) {return NH_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_RESULT Nh_CSS_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.css) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, result, line);
    return result;
}

