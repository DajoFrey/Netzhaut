#ifndef NETZHAUT_TTY_H
#define NETZHAUT_TTY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "NhCore/Common/API.h"
#include "NhTTY/Common/API.h"

#endif

/** @addtogroup CFunctions Functions
 *  \ingroup CCore 
 *  @{
 */

    Nh_TTY_Terminal *Nh_TTY_openTerminal(
    );

    NH_TTY_RESULT Nh_TTY_addDefaultProgram(
        Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p
    ); 

/** @} */

#endif // NETZHAUT_TTY_H
