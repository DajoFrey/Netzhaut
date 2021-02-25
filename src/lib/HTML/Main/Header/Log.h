#ifndef NH_HTML_LOG_H
#define NH_HTML_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Document.h"

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_preLog(
        Nh_HTML_Tree *Tree_p
    );
    
    NH_RESULT Nh_HTML_postLog(
        Nh_HTML_Tree *Tree_p
    );

    NH_RESULT Nh_HTML_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_RESULT Nh_HTML_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_RESULT Nh_HTML_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_RESULT result, int line
    );

/** @} */

#endif 
