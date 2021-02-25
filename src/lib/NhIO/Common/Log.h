#ifndef NH_IO_LOG_H
#define NH_IO_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../Input.h"
#include "../Window.h"

#endif

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    void Nh_IO_logMouse(
        Nh_IO_MouseEvent *Event_p 
    );

    void Nh_IO_logWindow(
        Nh_IO_ConfigureEvent *Event_p
    );

    NH_IO_RESULT _Nh_IO_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_IO_RESULT _Nh_IO_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_IO_RESULT _Nh_IO_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_IO_RESULT result, int line
    );

/** @} */

#endif 
