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

#include NH_HTML_FLOW
#include NH_HTML_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_HTML_RESULT Nh_HTML_initialize()
{
NH_HTML_BEGIN()

    NH_HTML_CHECK(Nh_HTML_createIndexMap())

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

