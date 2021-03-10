#ifndef NH_WEBIDL_LOG_H
#define NH_WEBIDL_LOG_H

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

/** @addtogroup NhWebIDLFunctions
 *  @{
 */

    NH_WEBIDL_RESULT _Nh_WebIDL_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_RESULT _Nh_WebIDL_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_WEBIDL_RESULT _Nh_WebIDL_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_WEBIDL_RESULT result, int line
    );

    NH_WEBIDL_RESULT Nh_WebIDL_logTokens(
        NH_BYTE *fragmentName_p, Nh_Array *Tokens_p, NH_BOOL dirty
    );

    NH_WEBIDL_RESULT Nh_WebIDL_logParseTree(
        NH_BYTE *fragmentName_p, Nh_WebIDL_ParseNode *ParseNode_p
    );

    NH_WEBIDL_RESULT Nh_WebIDL_logFragment(
        NH_BYTE *specification_p, Nh_WebIDL_Fragment *Fragment_p
    );

/** @} */

#endif 
