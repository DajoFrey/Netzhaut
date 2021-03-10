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

#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include "../Runtime/Runtime.h"
#include "../Runtime/String.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_WEBIDL_RESULT Nh_WebIDL_initialize()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK(Nh_WebIDL_createIndexMap())
    NH_WEBIDL_CHECK(Nh_WebIDL_initRuntime())
    NH_WEBIDL_CHECK(Nh_WebIDL_initNamespaces())

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

