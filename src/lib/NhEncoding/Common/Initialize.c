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

#include NH_ENCODING_FLOW
#include NH_ENCODING_DEFAULT_CHECK

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_ENCODING_RESULT Nh_Encoding_initialize()
{
NH_ENCODING_BEGIN()

    NH_ENCODING_CHECK(Nh_Encoding_createIndexMap())

NH_ENCODING_DIAGNOSTIC_END(NH_ENCODING_SUCCESS)
}

