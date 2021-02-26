// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "NetzhautTTY.h"
#include "Netzhaut.h"

#include "../NhLoader/Loader.h"
#include "../NhTTY/Terminal.h"
#include "../NhTTY/Program.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ADD =============================================================================================

Nh_TTY_Terminal *Nh_TTY_openTerminal()
{
    Nh_TTY_openTerminal_f openTerminal_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhTTY", 0, "Nh_TTY_openTerminal");
    return openTerminal_f ? openTerminal_f() : NULL;
}

NH_TTY_RESULT Nh_TTY_addDefaultProgram(
    Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p) 
{
    Nh_TTY_addDefaultProgram_f addDefaultProgram_f = !NH_LOADER_P || !Terminal_p || !name_p ? NULL : NH_LOADER_P->loadFunction_f("NhTTY", 0, "Nh_TTY_addDefaultProgram");
    return addDefaultProgram_f ? addDefaultProgram_f(Terminal_p, name_p) : NH_TTY_ERROR_BAD_STATE;
}

NH_TTY_RESULT Nh_TTY_addProgram(
    Nh_TTY_Terminal *Terminal_p, Nh_TTY_ProgramPrototype *Prototype_p)
{
    return 1;
}

