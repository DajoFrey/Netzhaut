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
    if (((Nh_Loader*)NH_LOADER_P)->load_f("NhTTY", 0, NH_TRUE) != NH_LOADER_SUCCESS) {return NULL;}
    return ((Nh_TTY_openTerminal_f)((Nh_Loader*)NH_LOADER_P)->loadFunction_f("NhTTY", 0, "Nh_TTY_openTerminal"))();
}

NH_TTY_RESULT Nh_TTY_addDefaultProgram(
    Nh_TTY_Terminal *Terminal_p, NH_BYTE *name_p) 
{
    if (!Terminal_p || !name_p) {return NH_TTY_ERROR_BAD_STATE;}
    if (((Nh_Loader*)NH_LOADER_P)->load_f("NhTTY", 0, NH_TRUE) != NH_LOADER_SUCCESS) {return NH_TTY_ERROR_BAD_STATE;}
    return ((Nh_TTY_addDefaultProgram_f)((Nh_Loader*)NH_LOADER_P)->loadFunction_f("NhTTY", 0, "Nh_TTY_addDefaultProgram"))(Terminal_p, name_p);
}

NH_TTY_RESULT Nh_TTY_addProgram(
    Nh_TTY_Terminal *Terminal_p, Nh_TTY_ProgramPrototype *Prototype_p)
{
    return 1;
}
