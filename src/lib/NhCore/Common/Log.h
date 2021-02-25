#ifndef NH_LOG_H
#define NH_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"
#include "../String.h"

#endif

/** @addtogroup NhCoreFunctions
 *  @{
 */

    void Nh_logNewFont(
        NH_BYTE *absolutePath_p
    );

    void Nh_logUnicodeCodepoint(
        const NH_BYTE *codepoint_p
    );

    void Nh_logSystemInfo(
        NH_BYTE *info_p, int line
    );

    NH_RESULT _Nh_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_RESULT _Nh_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_RESULT _Nh_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_RESULT result, int line
    );

/** @} */

#endif 
