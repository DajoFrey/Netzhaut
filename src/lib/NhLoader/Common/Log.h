#ifndef NH_LOADER_LOG_H
#define NH_LOADER_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "API.h"
#include "Result.h"

#endif

/** @addtogroup LoaderFunctions Functions
 *  \ingroup Loader
 *  @{
 */

    NH_LOADER_RESULT _Nh_Loader_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_LOADER_RESULT _Nh_Loader_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_LOADER_RESULT _Nh_Loader_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_LOADER_RESULT result, int line
    );

/** @} */

#endif 
