// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "Object.h"
#include "Parser.h"

#include "../Common/Macro.h"
#include NH_WEBIDL_FLOW
#include NH_WEBIDL_DEFAULT_CHECK

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

typedef struct Nh_WebIDL_Object {
    Nh_WebIDL_ParseNode *Interfaces_pp[8];
    Nh_Array Values;
} Nh_WebIDL_Object;

// CREATE ==========================================================================================

Nh_WebIDL_Object *Nh_WebIDL_createObject(
    NH_BYTE *spec_p, NH_BYTE *class_p)
{
NH_WEBIDL_BEGIN()

NH_WEBIDL_END(NULL)
}

