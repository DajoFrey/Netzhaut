#ifndef NH_ENCODING_LOG_H
#define NH_ENCODING_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "API.h"
#include "Result.h"

#endif

/** @addtogroup NhEncodingFunctions
 *  @{
 */

    NH_ENCODING_RESULT _Nh_Encoding_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ENCODING_RESULT _Nh_Encoding_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_ENCODING_RESULT _Nh_Encoding_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_ENCODING_RESULT result, int line
    );

/** @} */

#endif 
