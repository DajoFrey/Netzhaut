#ifndef NH_WEB_LOG_H
#define NH_WEB_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#include "../Runtime/Tokenizer.h"
#include "../Runtime/Parser.h"
#include "../Runtime/Runtime.h"

#endif

/** @addtogroup NhWebFunctions
 *  @{
 */

    NH_WEB_RESULT _Nh_Web_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_WEB_RESULT _Nh_Web_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_WEB_RESULT _Nh_Web_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_WEB_RESULT result, int line
    );

    NH_WEB_RESULT Nh_Web_logTokens(
        NH_BYTE *fragmentName_p, Nh_Array *Tokens_p, NH_BOOL dirty
    );

    NH_WEB_RESULT Nh_Web_logParseTree(
        NH_BYTE *fragmentName_p, Nh_Web_ParseNode *ParseNode_p
    );

    NH_WEB_RESULT Nh_Web_logFragment(
        NH_BYTE *specification_p, Nh_Web_Fragment *Fragment_p
    );

/** @} */

#endif 
