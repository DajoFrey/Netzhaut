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

#define EVENT_LISTENERS EventTarget_p->internal_p

// INITIALIZE ======================================================================================

NH_DOM_RESULT Nh_DOM_initializeEventTarget(
    Nh_WebIDL_Object *EventTarget_p)
{
NH_DOM_BEGIN()

    EventTarget_p->internal_p = Nh_allocate(sizeof(Nh_List));
    NH_DOM_CHECK_MEM(EventTarget_p->internal_p)

    *((Nh_List*)EVENT_LISTENERS) = Nh_initList(16);

NH_DOM_DIAGNOSTIC_END(NH_DOM_SUCCESS)
}

// API =============================================================================================

