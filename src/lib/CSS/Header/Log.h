#ifndef NH_CSS_LOG_H
#define NH_CSS_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Sheet.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_logSheet(
        Nh_CSS_Sheet *Sheet_p
    );

    NH_RESULT Nh_CSS_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_RESULT Nh_CSS_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_RESULT Nh_CSS_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_RESULT result, int line
    );

/** @} */

#endif 
