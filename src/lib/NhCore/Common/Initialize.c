// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Initialize.h"

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

// INIT ============================================================================================

static NH_RESULT Nh_startInternalWorkloads()
{
NH_BEGIN()

    Nh_activateWorkload(
        NULL, Nh_runSystemUpdater, NULL, NH_FALSE
    );

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

NH_RESULT Nh_initialize()
{
NH_BEGIN()

    NH_CHECK(Nh_initLogger())
    NH_CHECK(Nh_initFlags())
    NH_CHECK(Nh_initMemory())
    NH_CHECK(Nh_initProcessPool())
    NH_CHECK(Nh_initConfig())
    NH_CHECK(Nh_logUnicodeCodepoints())
    NH_CHECK(Nh_initFontManager())
    NH_CHECK(Nh_addInitialFonts())
    NH_CHECK(Nh_addInitialFontPrototypes())
    NH_CHECK(Nh_createIndexMap())
    NH_CHECK(Nh_initSystem())
    NH_CHECK(Nh_startInternalWorkloads())

NH_DIAGNOSTIC_END(NH_SUCCESS)
}

