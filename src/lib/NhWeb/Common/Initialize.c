// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Initialize.h"
#include "Macro.h"
#include "IndexMap.h"

#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include "../Runtime/Runtime.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEB_RESULT Nh_Web_initialize()
{
NH_WEB_BEGIN()

    NH_WEB_CHECK(Nh_Web_createIndexMap())
    NH_WEB_CHECK(Nh_Web_initRuntime())

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

