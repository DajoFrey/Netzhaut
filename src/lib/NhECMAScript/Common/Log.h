#ifndef NH_ECMASCRIPT_LOG_H
#define NH_ECMASCRIPT_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#include "../Engine/Parser.h"

#endif

/** @addtogroup NhECMAScriptFunctions
 *  @{
 */

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_logDecoder(
        void *handle_p, Nh_Array *UnicodeCodePoints_p
    );

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_logLexer(
        void *handle_p, Nh_Array *InputElements_p, NH_BOOL dirty
    );

    NH_ECMASCRIPT_RESULT Nh_ECMAScript_logParseTree(
        void *handle_p, Nh_ECMAScript_ParseNode *ParseNode_p, Nh_ECMAScript_ParseNode *Parent_p, int depth, NH_BOOL *branch_p
    );

    NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ECMASCRIPT_RESULT _Nh_ECMAScript_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_ECMASCRIPT_RESULT result, int line
    );

/** @} */

#endif 
