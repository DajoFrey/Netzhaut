// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Log.h"
#include "../Header/Macros.h"

#include "../../../Netzhaut/String.h"
#include "../../../Netzhaut/Memory.h"
#include "../../../Netzhaut/Config.h"
#include "../../../Netzhaut/Output.h"
#include "../../../Netzhaut/Flow.h"

#include NH_FLOW
#include NH_DEFAULT_CHECK

#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_RESULT Nh_HTML_preLog( // TODO multithreading
    Nh_HTML_Tree *Tree_p)
{
NH_BEGIN()

    if (NH_CONFIG.Flags.Log.prehtml)
    {
        Nh_String *String_p = Nh_allocateString(NULL);

        char *document_p = Nh_HTML_stringifyTree(Tree_p);
        NH_CHECK_NULL(document_p)

        NH_CHECK((Nh_appendFormatToString(String_p, "%s\n", document_p)))

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);
        Nh_free(document_p);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
            NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_postLog( // TODO multithreading
    Nh_HTML_Tree *Tree_p)
{
NH_BEGIN()

    if (NH_CONFIG.Flags.Log.posthtml)
    {
        Nh_String *String_p = Nh_allocateString(NULL);

        char *document_p = Nh_HTML_stringifyTree(Tree_p);
        NH_CHECK_NULL(document_p)

        NH_CHECK((Nh_appendFormatToString(String_p, "%s\n", document_p)))

        char *chars_p = Nh_getChars(String_p);
        Nh_freeString(String_p, false);
        Nh_free(document_p);

        if (Nh_addOutput(NH_OUTPUT_CHARS, chars_p, NH_FALSE) != NH_SUCCESS) {
            Nh_free(chars_p);
            NH_DIAGNOSTIC_END(NH_ERROR_BAD_STATE)
        }
    }

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_logBegin(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
    return _Nh_begin(file_p, function_p);
}

NH_RESULT Nh_HTML_logEnd(
    const char *file_p, const char *function_p)
{
    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
    return _Nh_end(file_p, function_p);
}

NH_RESULT Nh_HTML_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_RESULT result, int line)
{
    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
    _Nh_diagnosticEnd(file_p, function_p, result, line);
    return result;
}

