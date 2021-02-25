#ifndef NH_NET_LOG_H
#define NH_NET_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Result.h"

#endif

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    void Nh_Net_logReceive(
        char *data_p
    );

    void Nh_Net_logSend(
        char *data_p
    );

    NH_NET_RESULT _Nh_Net_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_NET_RESULT _Nh_Net_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_NET_RESULT _Nh_Net_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_NET_RESULT result, int line
    );

/** @} */

#endif 
