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
#include NH_IO_FLOW
#include NH_IO_DEFAULT_CHECK

#include "../NhCore/Common/Macro.h"
#include NH_FLOW
#include NH_CUSTOM_CHECK

// INIT ============================================================================================

static void Nh_IO_initKeyboardEventBuffer(
    Nh_RingIterator *Iterator_p, int itemCount)
{
NH_IO_BEGIN()

    for (int i = 0; i < itemCount; ++i) 
    {
        Nh_IO_KeyboardEvent *Event_p = Nh_advanceRingIterator(Iterator_p);
        Event_p->key     = NH_IO_KEY_UNDEFINED;
        Event_p->trigger = NH_IO_TRIGGER_UNDEFINED;
    }

NH_IO_SILENT_END()
}

NH_IO_RESULT Nh_IO_initKeyboard(
    Nh_IO_Keyboard *Keyboard_p, int bufferSize) 
{
NH_IO_BEGIN()

    NH_CHECK(NH_IO_ERROR_BAD_STATE, Nh_initRingIterator(
        &Keyboard_p->Events, bufferSize, sizeof(Nh_IO_KeyboardEvent), Nh_IO_initKeyboardEventBuffer
    ))
    Nh_initRingIteratorMark(&Keyboard_p->Mark);
    for (int i = 0; i < NH_IO_KEY_COUNT; ++i) {Keyboard_p->state_p[i] = false;}

NH_IO_DIAGNOSTIC_END(NH_IO_SUCCESS);
}

