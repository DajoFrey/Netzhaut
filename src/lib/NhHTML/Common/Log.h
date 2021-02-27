#ifndef NH_HTML_LOG_H
#define NH_HTML_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#endif

/** @addtogroup NhHTMLFunctions
 *  @{
 */

    NH_HTML_RESULT _Nh_HTML_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_HTML_RESULT _Nh_HTML_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_HTML_RESULT _Nh_HTML_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_HTML_RESULT result, int line
    );

/** @} */

#endif 
