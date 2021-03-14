// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "Keyboard.h"

#include "../NhCore/Tab.h"
#include "../NhCore/Memory.h"

#include "Common/Macro.h"
#include NH_WSI_FLOW
#include NH_WSI_DEFAULT_CHECK

#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

// INIT ============================================================================================

static void Nh_WSI_initKeyboardEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_WSI_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_WSI_KeyboardEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->key     = NH_WSI_KEY_UNDEFINED;
        Event_p->trigger = NH_WSI_TRIGGER_UNDEFINED;
    }

NH_WSI_SILENT_END()
}

NH_WSI_RESULT Nh_WSI_initKeyboard(
    Nh_WSI_Keyboard *Keyboard_p, int bufferSize) 
{
NH_WSI_BEGIN()

    NH_CHECK(NH_WSI_ERROR_BAD_STATE, Nh_initRingIterator(
        &Keyboard_p->Events, bufferSize, sizeof(Nh_WSI_KeyboardEvent), Nh_WSI_initKeyboardEventBuffer
    ))
    Nh_initRingIteratorMark(&Keyboard_p->Mark);
    for (int i = 0; i < NH_WSI_KEY_COUNT; ++i) {Keyboard_p->state_p[i] = false;}

NH_WSI_DIAGNOSTIC_END(NH_WSI_SUCCESS);
}

