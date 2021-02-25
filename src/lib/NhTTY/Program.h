#ifndef NH_TTY_PROGRAM_H
#define NH_TTY_PROGRAM_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Common/API.h"
#include "Common/Result.h"

#include "../NhCore/List.h"

typedef struct Nh_TTY_Terminal Nh_TTY_Terminal;

#endif

/** @addtogroup NhTTYTypedefs
 *  @{
 */

    typedef NH_TTY_RESULT (*Nh_TTY_addDefaultProgram_f)(
        Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p
    );

/** @} */

/** @addtogroup NhTTYFunctions
 *  @{
 */

    NH_TTY_RESULT Nh_TTY_addDefaultProgram(
        Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p
    );

    Nh_Array *Nh_TTY_createProgramInstances(
        Nh_List *Prototypes_p 
    );

/** @} */

#endif 

