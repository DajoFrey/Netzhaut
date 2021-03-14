// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "NetzhautWSI.h"
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

Nh_WSI_Window *Nh_WSI_openWindow()
{
    Nh_TTY_openWindow_f openWindow_f = !NH_LOADER_P ? NULL : NH_LOADER_P->loadFunction_f("NhWSI", 0, "Nh_WSI_openWindow");
    return openWindow_f ? openWindow_f() : NULL;
}

