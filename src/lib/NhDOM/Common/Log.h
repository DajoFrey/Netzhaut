#ifndef NH_DOM_LOG_H
#define NH_DOM_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#endif

/** @addtogroup NhDOMFunctions
 *  @{
 */

    NH_DOM_RESULT _Nh_DOM_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_DOM_RESULT _Nh_DOM_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_DOM_RESULT _Nh_DOM_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_DOM_RESULT result, int line
    );

/** @} */

#endif 
