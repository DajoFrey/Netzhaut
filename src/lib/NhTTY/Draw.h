#ifndef NH_TTY_DRAW_H
#define NH_TTY_DRAW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Terminal.h"

#endif

/** @addtogroup NhTTYFunctions
 *  @{
 */

    NH_TTY_RESULT Nh_TTY_refreshScreen(
        Nh_TTY_Terminal *Terminal_p
    ); 

    NH_TTY_RESULT Nh_TTY_refreshCursor(
        Nh_TTY_Terminal *Terminal_p
    ); 

/** @} */

#endif 
