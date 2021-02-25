#ifndef NH_GFX_LOG_H
#define NH_GFX_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Netzhaut/Netzhaut.h"

#endif

/** @addtogroup GraphicsFunctions Functions
 *  \ingroup Graphics 
 *  @{
 */

    void Nh_Gfx_logVulkan(
        char *message_p
    );

    NH_RESULT Nh_Gfx_logBegin(
        const char *file_p, const char *function_p
    );

    NH_RESULT Nh_Gfx_logEnd(
        const char *file_p, const char *function_p
    );

    NH_RESULT Nh_Gfx_logDiagnosticEnd(
        const char *file_p, const char *function_p, NH_RESULT result, int line
    );

/** @} */

#endif 
