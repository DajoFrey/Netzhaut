// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "EventTarget.h"

#include "Common/Macro.h"
#include NH_DOM_FLOW
#include NH_DOM_DEFAULT_CHECK

#include "../NhCore/List.h"
#include "../NhCore/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DECLARE =========================================================================================

#define EVENT_LISTENERS EventTarget_p->Attributes.pp[0]

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeEventTarget(
    Nh_Web_Object *EventTarget_p)
{
NH_DOM_BEGIN()

    EVENT_LISTENERS = Nh_allocate(sizeof(Nh_List));
    NH_DOM_CHECK_MEM(EVENT_LISTENERS)
    *((Nh_List*)EVENT_LISTENERS) = Nh_initList(16);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

