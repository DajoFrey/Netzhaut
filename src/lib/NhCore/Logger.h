#ifndef NH_LOGGER_H
#define NH_LOGGER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "String.h"
#include "List.h"

#include "Common/Result.h"
#include "Common/API.h"

#endif

/** @addtogroup NhCoreStructs
 *  @{
 */

    typedef struct Nh_LoggerNode {
        NH_BYTE *category_p;
        Nh_List Children;
        Nh_List Messages;
        struct Nh_LoggerNode *Parent_p;
    } Nh_LoggerNode;

    typedef struct Nh_Logger {
        Nh_LoggerNode Root;
    } Nh_Logger;

/** @} */

/** @addtogroup NhCoreVars
 *  @{
 */

    extern Nh_Logger NH_LOGGER;

/** @} */

/** @addtogroup NhCoreFunctions
 *  @{
 */

    NH_RESULT Nh_initLogger(
    );

    NH_RESULT _Nh_updateLogger(
        NH_BYTE *message_p
    );

    NH_RESULT Nh_addLogCallback(
        void (*logCallback_f)(NH_BYTE *bytes_p)
    );
    
    NH_RESULT _Nh_sendLogMessage(
        NH_BYTE *message_p
    );
    
    NH_RESULT Nh_sendLogMessage(
        NH_BYTE *message_p
    );

    NH_RESULT _Nh_begin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );

    NH_RESULT _Nh_end(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_RESULT _Nh_diagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, const NH_BYTE *result_p, int line, NH_BOOL success
    );

/** @} */

#endif 
