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

#include NH_ECMASCRIPT_FLOW
#include NH_ECMASCRIPT_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_ECMASCRIPT_RESULT Nh_ECMAScript_terminate()
{
NH_ECMASCRIPT_BEGIN()

    Nh_ECMAScript_freeIndexMap();

NH_ECMASCRIPT_DIAGNOSTIC_END(NH_ECMASCRIPT_SUCCESS)
}

