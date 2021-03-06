// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Terminate.h"
#include "Macro.h"
#include "IndexMap.h"

#include NH_WEB_FLOW
#include NH_WEB_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEB_RESULT Nh_Web_terminate()
{
NH_WEB_BEGIN()

    Nh_Web_freeIndexMap();

NH_WEB_DIAGNOSTIC_END(NH_WEB_SUCCESS)
}

