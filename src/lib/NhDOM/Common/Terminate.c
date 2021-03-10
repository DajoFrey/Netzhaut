// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Terminate.h"
#include "Macro.h"

#include NH_DOM_FLOW2
#include NH_DOM_DEFAULT_CHECK2

#include "../Document.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// DEBUG ===========================================================================================

NH_DOM_RESULT Nh_DOM_terminate()
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(Nh_DOM_freeDocumentModes())

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

