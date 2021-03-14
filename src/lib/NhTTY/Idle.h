#ifndef NH_TTY_IDLE_H
#define NH_TTY_IDLE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Program.h"
#include "Common/Result.h"

#include "../NhCore/Time.h"

#endif

/** @addtogroup NhTTYStructs
 *  @{
 */

    typedef struct Nh_TTY_Idle {
        Nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
        int current;
    } Nh_TTY_Idle;

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    void *Nh_TTY_initIdle(
    );

    NH_TTY_RESULT Nh_TTY_updateIdle(
        Nh_TTY_Program *Program_p
    );

    NH_TTY_RESULT Nh_TTY_handleIdleInput(
        Nh_TTY_Program *Program_p, NH_UNICODE_CODEPOINT c
    );

    NH_TTY_RESULT Nh_TTY_drawIdleRow(
        Nh_TTY_Program *Program_p, Nh_Encoding_UTF8String *Row_p, int width, int height, int row 
    );

/** @} */

#endif 
