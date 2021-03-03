// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Terminate.h"

#include "../Memory.h"
#include "../Config.h"
#include "../Window.h"
#include "../Tab.h"
#include "../Process.h"
#include "../Thread.h"
#include "../Logger.h"
#include "../Unicode.h"

#include "IndexMap.h"
#include "Macro.h"

#include NH_FLOW2
#include NH_DEFAULT_CHECK2

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TERMINATE =======================================================================================

NH_RESULT Nh_terminate()
{
NH_BEGIN()

    puts("Terminate not yet implemented.");
    exit(0);

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

