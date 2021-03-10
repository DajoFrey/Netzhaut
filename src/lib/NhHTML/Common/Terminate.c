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

#include NH_HTML_FLOW
#include NH_HTML_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_HTML_RESULT Nh_HTML_terminate()
{
NH_HTML_BEGIN()

    Nh_HTML_freeIndexMap();

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

