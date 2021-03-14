#ifndef NH_WSI_LOG_H
#define NH_WSI_LOG_H

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

    void Nh_WSI_logMouse(
        Nh_WSI_MouseEvent *Event_p 
    );

    void Nh_WSI_logWindow(
        Nh_WSI_ConfigureEvent *Event_p
    );

    NH_WSI_RESULT _Nh_WSI_logBegin(
        const char *file_p, const char *function_p
    );
    
    NH_WSI_RESULT _Nh_WSI_logEnd(
        const char *file_p, const char *function_p
    );
    
    NH_WSI_RESULT _Nh_WSI_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_WSI_RESULT result, int line
    );

/** @} */

#endif 
